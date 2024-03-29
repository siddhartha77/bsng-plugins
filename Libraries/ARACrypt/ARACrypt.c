// ARACrypt.cpp : implementation file
//
// Note: A Special Thanks to Mr. Warren Ward for his Sept. 1998 CUJ article:
// "Stream Encryption" Copyright (c) 1998 by Warren Ward

#include "ARACrypt.h"

static unsigned int    m_LFSR_A;
static unsigned int    m_LFSR_B;
static unsigned int    m_LFSR_C;
static unsigned int    m_Mask_A;
static unsigned int    m_Mask_B;
static unsigned int    m_Mask_C;
static unsigned int    m_Rot0_A;
static unsigned int    m_Rot0_B;
static unsigned int    m_Rot0_C;
static unsigned int    m_Rot1_A;
static unsigned int    m_Rot1_B;
static unsigned int    m_Rot1_C;

/////////////////////////////////////////////////////////////////////////////
// ARACrypt
void ARACryptInit() {

	// Initialize the shift registers to non-zero 
	// values. If the shift register contains all 
	// 0's when the generator starts, it will not 
	// produce a usable sequence of bits. The 
	// numbers used here have no special features 
	// except for being non-zero.
	m_LFSR_A = 0x13579BDF;
	m_LFSR_B = 0x2468ACE0;
	m_LFSR_C = 0xFDB97531;

	// Initialize the masks to magic numbers. 
	// These values are primitive polynomials mod 
	// 2, described in Applied Cryptography, 
	// second edition, by Bruce Schneier (New York: 
	// John Wiley and Sons, 1994). See Chapter 15: 
	// Random Sequence Generators and Stream 
	// Ciphers, particularly the discussion on 
	// Linear Feedback Shift Registers.
	//
	// The primitive polynomials used here are:
	// Register A:	( 32, 7, 6, 2, 0 )
	// Register B:	( 31, 6, 0 )
	// Register C:	( 29, 2, 0 )
	//
	// The bits that must be set to "1" in the 
	// XOR masks are:
	// Register A:	( 31, 6, 5, 1 )
	// Register B:	( 30, 5 )
	// Register C:	( 28, 1 )
	//
	// Developer's Note
	// DO NOT CHANGE THESE NUMBERS WITHOUT 
	// REFERRING TO THE DISCUSSION IN SCHNEIER'S 
	// BOOK. They are some of very few 
	// near-32-bit values that will act as 
	// maximal-length random generators.
	m_Mask_A = 0x80000062;
	m_Mask_B = 0x40000020;
	m_Mask_C = 0x10000002;

	// Set up LFSR "rotate" masks.
	// These masks limit the number of bits 
	// used in the shift registers. Each one 
	// provides the most-significant bit (MSB) 
	// when performing a "rotate" operation. Here 
	// are the shift register sizes and the byte 
	// mask needed to place a "1" bit in the MSB 
	// for Rotate-1, and a zero in the MSB for 
	// Rotate-0. All the shift registers are stored
	// in an unsigned 32-bit integer, but these 
	// masks effectively make the registers 32 
	// bits (A), 31 bits (B), and 29 bits (C).
	//
	//	Bit	  |  3            2             1            0
	//	Pos'n | 1098 7654  3210 9876  5432 1098  7654 3210
	//	===== | ==========================================
	//	Value | 8421-8421  8421-8421  8421-8421  8421-8421
	//	===== | ==========================================
	//		  | 
	// A-Rot0 | 0111 1111  1111 1111  1111 1111  1111 1111  
	// A-Rot1 | 1000 0000  0000 0000  0000 0000  0000 0000 
	//		  | 
	// B-Rot0 | 0011 1111  1111 1111  1111 1111  1111 1111  
	// B-Rot1 | 1100 0000  0000 0000  0000 0000  0000 0000  
	//		  | 
	// C-Rot0 | 0000 1111  1111 1111  1111 1111  1111 1111  
	// C-Rot1 | 1111 0000  0000 0000  0000 0000  0000 0000  
	//
	//	
	// Reg Size	MSB Position	Rotate-0 Mask	Rotate-1 Mask
	//	A	32		31			0x7FFFFFFF		0x80000000
	//	B	31		30			0x3FFFFFFF		0xC0000000
	//	C	29		28			0x0FFFFFFF		0xF0000000
	//
	m_Rot0_A = 0x7FFFFFFF;
	m_Rot0_B = 0x3FFFFFFF;
	m_Rot0_C = 0x0FFFFFFF;
	m_Rot1_A = 0x80000000;
	m_Rot1_B = 0xC0000000;
	m_Rot1_C = 0xF0000000;
}

/////////////////////////////////////////////////////////////////////////////
// ARACrypt operations

void ARACryptSetKey(signed char* csKey) {
	int nIdx;

	// LFSR A, B, and C get the first, second, and
	// third four bytes of the seed, respectively.

	for (nIdx = 0; nIdx < 4; nIdx++) {
		m_LFSR_A = (m_LFSR_A <<= 8) | csKey[nIdx + 0];
		m_LFSR_B = (m_LFSR_B <<= 8) | csKey[nIdx + 4];
		m_LFSR_C = (m_LFSR_C <<= 8) | csKey[nIdx + 8];
	}

	// If any LFSR contains 0x00000000, load a 
	// non-zero default value instead. There is
	// no particular "good" value. The ones here
	// were selected to be distinctive during
	// testing and debugging.

	if (!m_LFSR_A)
		m_LFSR_A = 0x13579BDF;

	if (!m_LFSR_B)
		m_LFSR_B = 0x2468ACE0;

	if (!m_LFSR_C)
		m_LFSR_C = 0xFDB97531;

	return;
}

//***********************************************
// ARACryptTransformChar
//***********************************************
// Transform a single character. If it is 
// plaintext, it will be encrypted. If it is
// encrypted, and if the LFSRs are in the same
// state as when it was encrypted (that is, the
// same keys loaded into them and the same number
// of calls to TransformChar after the keys
// were loaded), the character will be decrypted
// to its original value.
//
// DEVELOPER'S NOTE
// This code contains corrections to the LFSR
// operations that supercede the code examples
// in Applied Cryptography (first edition, up to
// at least the 4th printing, and second edition,
// up to at least the 6th printing). More recent
// errata sheets may show the corrections.
//***********************************************

void ARACryptTransformChar(unsigned char* cTarget) {
	int		            Counter = 0;
	unsigned char		Crypto = 0;
	unsigned char		Out_B = (m_LFSR_B & 1);
	unsigned char		Out_C = (m_LFSR_C & 1);

	// Cycle the LFSRs eight times to get eight 
	// pseudo-random bits. Assemble these into 
	// a single random character (Crypto).

	for (Counter = 0; Counter < 8; Counter++) {
		if (m_LFSR_A & 1) {
			// The least-significant bit of LFSR 
			// A is "1". XOR LFSR A with its 
			// feedback mask.
			m_LFSR_A = ((m_LFSR_A ^ m_Mask_A) >> 1) | m_Rot1_A;

			// Clock shift register B once.
			if (m_LFSR_B & 1) {
				// The LSB of LFSR B is "1". XOR 
				// LFSR B with its feedback mask.
				m_LFSR_B = ((m_LFSR_B ^ m_Mask_B) >> 1) | m_Rot1_B;
				Out_B = 1;
			}
			else {
				// The LSB of LFSR B is "0". Rotate 
				// the LFSR contents once.
				m_LFSR_B = (m_LFSR_B >> 1) & m_Rot0_B;
				Out_B = 0;
			}
		}
		else {
			// The LSB of LFSR A is "0". 
			// Rotate the LFSR contents once.
			m_LFSR_A = (m_LFSR_A >> 1) & m_Rot0_A;

			// Clock shift register C once.
			if (m_LFSR_C & 1) {
				// The LSB of LFSR C is "1". 
				// XOR LFSR C with its feedback mask.
				m_LFSR_C = ((m_LFSR_C ^ m_Mask_C) >> 1) | m_Rot1_C;
				Out_C = 1;
			}
			else {
				// The LSB of LFSR C is "0". Rotate 
				// the LFSR contents once.
				m_LFSR_C = (m_LFSR_C >> 1) & m_Rot0_C;
				Out_C = 0;
			}
		}

		// XOR the output from LFSRs B and C and 
		// rotate it into the right bit of Crypto.
		Crypto = (Crypto << 1) | (Out_B ^ Out_C);
	}

	// XOR the resulting character with the 
	// input character to encrypt/decrypt it.
	*cTarget ^=  Crypto;

	return;
}

//***********************************************
// ARACryptTransformString
//***********************************************
// Encrypt or decrypt a standard string in place.
// The string to transform is passed in as 
// Target.
//***********************************************

void ARACryptTransformString(unsigned char* csKey, unsigned char* csTarget, int targetLen) {
	unsigned char cBuff;
	int nPos;
	
	// Reset the shift registers.
	ARACryptSetKey((signed char *)csKey);

	// Transform each character in the string.
	// 
	// DEVELOPER'S NOTE
	// ==========================================
	// DO NOT TREAT THE OUTPUT STRING AS A NULL-
	// TERMINATED STRING. 
	// ==========================================
	// The transformation process can create '\0' 
	// characters in the output string. Always 
	// use the length() method to retrieve the full 
	// string when it has been transformed.

	// bek NOTE: The above note does not apply to this
	// implementation of Warren Ward's method.
	// ARACrypt knows about NULLs and transforms them
	// into XOR NULLs so the entire result can be
	// treated as a 'normal' NULL terminated string.

	for (nPos = 0; nPos < targetLen; nPos++) {
		cBuff = csTarget[nPos];
		ARACryptTransformChar(&cBuff);
		csTarget[nPos] = cBuff;
	}

	return;
}
