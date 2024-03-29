/*
 * $Workfile:: UltraLibU.c                                                    $
 * $Revision:: 1                                                              $
 *
 * $Author:: Buck Rogers                                                      $
 * $Modtime:: 30.09.1997 17:50 Uhr                                            $
 *
 * $History:: UltraLibU.c                                                     $
 * 
 * *****************  Version 1  *****************
 * User: Buck Rogers  Date: 30.09.1997   Time: 18:31 Uhr
 * Created in $/BSNG/Plugins/BSNG SDK/Libraries/UltraLibU
 * Adding subproject 'BSNG' to '$/'
 *
 * $NoKeywords::                                                              $
 */


/**************************   ULTRALibU(niversal)   **********************

This is a Macintosh/Motorola implementation of the Ultra pseudo-random
number generator --  the state-of-the-art as of 11/92.

References --

	Creator:	Prof. George Marsaglia and Co-workers
				Dept. of Statistics
				Florida State University
				
	Code by:	Dr. Michael P. McLaughlin
				MITRE Corporation
				MS W337
				1820 Dolley Madison Blvd.
				McLean, VA  22102
				
				E-mail: mpmcl@mitre.org
				
	Please acknowledge the work of others by leaving this header intact.
	
**************************************************************************
	
			---------------  General Remarks  ---------------
			
The code was written in a Metrowerks™ environment and is compatible with
	either 68K (68020 and higher) or PowerPC environments.
Assembly language was used for one critical low-level function.
Things to watch out for include the following:

	sizeof(double) is compiler-option dependent and is here unspecified.
	Longs and floats are 4 bytes.
	Shorts are 2 bytes.
	
	Functions returning bytes or bits are pre-cast to short.
	
	Registers not saved:
	
		68K:	D0-D2, A0-A1

		PPC:	r3-r10
	
	WARNING!  Be sure to compile and run UltraUTest in your desired environment.
	Ascertain that your output is identical to UltraLibUTest.out.org, supplied
	with this package.
 	
				---------------  Timings  ---------------

The execution times for the functions supplied are somewhat variable and
are increased by factors such as cache overflow and program segmentation.
The timings below are average values for a standard Macintosh Quadra 800™
and a PowerMac 9500/120™.  The compiler was Metrowerks CW8™ with full
optimization for speed and both the calling module and UltraLibU in the same
segment.  68040 "Native Floating Point" was used in the 68K case.  To
facilitate comparisons, the calling overhead (to a void function with no
parameters) has been subtracted out.  Your timings may differ.

			  Function								Microseconds per Call
			  												68K			PPC   

			Ultra_long32								1.5			0.27
			Ultra_long31						 		1.5			0.27
		
			Ultra_short16						 	 	1.0			0.24
			Ultra_short15						 	 	1.0			0.23

			Ultra_short8						 		0.77			0.21
			Ultra_short8u							 	0.72			0.21
			Ultra_short7						 	 	0.75			0.21			

			Ultra_short1						 	 	0.65			0.23

			Ultra_uni							 		4.7			0.57
			Ultra_vni							 		4.8			0.61

			Ultra_duni							 		6.4			0.91
			Ultra_dvni							 		6.4			0.90

			Ultra_norm								  24.0			2.74
			Ultra_expo								  28.1			2.53

*************************************************************************/

#include <math.h>

struct {					/* to restart Ultra from a known beginning */
		float				Ugauss;
		unsigned long	UFSR[37],USWB[37],Ubrw,Useed1,Useed2;
		long				Ubits;
		short				Ubyt,Ubit;
		char				*Uptr;
	} Ultra_Remember;	/* 324 bytes */

double			Ultra_2n63,Ultra_2n31,Ultra_2n7;
float				Ultra_gauss;		/* remaining Ultra_norm variate */
unsigned long	Ultra_FSR[37],		/* final random numbers */
					Ultra_SWB[37],		/* subtract-with-borrow output */
					Ultra_brw,			/* either borrow(68K) or ~borrow(PPC) */
					Ultra_seed1,		/* seeds MUST be initialized with */
					Ultra_seed2;		/*    values > 0 */
long				Ultra_bits;			/* bits for Ultra_short1 */
short				Ultra_byt,			/* # bytes left in Ultra_FSR[37] */
					Ultra_bit;			/* # bits left in Ultra_bits */
char				*Ultra_ptr;			/* running pointer to Ultra_FSR[] */

/*	Ultra_Refill() is the core of Ultra (see Marsaglia and Zaman, 1991,
	"A New Class of Random Number Generators", Annals of Applied Probability,
	vol. 1(3), 426-480). It refills Ultra_SWB[37] via a subtract-with-borrow
	PRNG then superimposes a multiplicative congruential PRNG to produce
	Ultra_FSR[37] which supplies all of the Ultra-random bytes.
	
	The contents of Ultra_FSR[] are EXTREMELY random, even at the bit level,
	and ALL bits appear to be random, not just the most significant bits.
	The period of this compound generator exceeds 10^356. */

/** Public Prototypes ***************************************************/

long Ultra_long32();
long Ultra_long31();

short Ultra_short16();
short Ultra_short15();

short Ultra_short8();
short Ultra_short8u();
short Ultra_short7();

short Ultra_short1();

float Ultra_uni();
float Ultra_vni();

double Ultra_duni();
double Ultra_dvni();

float Ultra_norm(float mu,float sigma);
float Ultra_expo(float mu);

Boolean Ultra_Init();

void Ultra_SaveStart();
void Ultra_RecallStart();

/** Private Prototype ***************************************************/

void Ultra_Refill();

/************************************************************************/

#ifdef __POWERPC__
#define	ULTRABRW	0xFFFFFFFF
asm void Ultra_Refill()
{
		lwz		r3,Ultra_brw		/* fetch global addresses from TOC */
		lwz		r6,Ultra_SWB
		lwz		r4,0(r3)				/* ~borrow */
		la			r7,48(r6)			/* &Ultra_SWB[12] */
		
		sub		r5,r5,r5				/* clear entire word */
		mr			r8,r5					/* counter */
		li			r5,1
		sraw		r4,r4,r5				/* restore XER|CA */
		
		li			r8,24
		mtctr		r8
		la			r4,-4(r6)
UR1:	lwzu		r9,4(r7)
		lwz		r10,4(r4)
		subfe		r9,r10,r9			/* r9 -= r10 */
		stwu		r9,4(r4)
		bdnz+		UR1
		mr			r7,r6					/* &Ultra_SWB */
		li			r8,13
		mtctr		r8
		la			r7,-4(r6)
UR2:	lwzu		r9,4(r7)
		lwz		r10,4(r4)
		subfe		r9,r10,r9			/* r9 -= r10 */
		stwu		r9,4(r4)
		bdnz+		UR2

		lwz		r4,0(r3)				/* ~borrow again */
		addme		r4,r5					/* r5 = 1 */
		neg		r4,r4
		stw		r4,0(r3)				/* new ~borrow */

		la			r6,-4(r6)			/* &SWB[-1] */
		lwz		r7,Ultra_FSR
		lwz		r5,Ultra_ptr
		lwz		r4,Ultra_seed1
		stw		r7,0(r5)				/* reset running pointer to FSR */
		la			r7,-4(r7)			/* overlay congruential PRNG */
		lis		r10,1					/* r10 = 69069 */
		addi		r10,r10,3533
		lwz		r5,0(r4)				/* Ultra_seed1 */
		li			r8,37
		mtctr		r8
UR3:	lwzu		r9,4(r6)				/* SWB */
		mullw		r5,r5,r10			/* Ultra_seed1 *= 69069 */
		xor		r9,r9,r5
		stwu		r9,4(r7)				
		bdnz+		UR3
		stw		r5,0(r4)				/* save Ultra_seed1 for next time */
		lwz		r7,Ultra_byt
		li			r5,148				/* 4*37 bytes */
		sth		r5,0(r7)				/* reinitialize */
		blr
}
#else
#define	ULTRABRW	0x00000000
asm void Ultra_Refill()
{
		machine	68020

		MOVE.L	A2,-(SP)				/* not scratch */
		LEA		Ultra_SWB,A2		/* Ultra_SWB[0] */
		LEA		52(A2),A1			/* Ultra_SWB[13] */
		MOVEQ		#0,D0					/* restore extend bit */
		SUB.L		Ultra_brw,D0
		MOVEQ		#23,D2				/* 24 of these */
UR1:	MOVE.L	(A1)+,D0
		MOVE.L	(A2),D1
		SUBX.L	D1,D0
		MOVE.L	D0,(A2)+
		DBRA		D2,UR1
		LEA		Ultra_SWB,A1
		MOVEQ		#12,D2				/* 13 of these */
UR2:	MOVE.L	(A1)+,D0
		MOVE.L	(A2),D1
		SUBX.L	D1,D0					/* subtract-with-borrow */
		MOVE.L	D0,(A2)+
		DBRA		D2,UR2
		MOVEQ		#0,D0
		MOVE.L	D0,D1
		ADDX		D1,D0					/* get borrow bit */
		MOVE.L	D0,Ultra_brw		/*    and save it */
		LEA		Ultra_SWB,A1
		LEA		Ultra_FSR,A2
		MOVE.L	A2,Ultra_ptr		/* reinitialize running pointer */
		MOVE.L	Ultra_seed1,D0
		MOVE.L	#69069,D1			/* overlay congruential PRNG */
		MOVEQ		#36,D2				/* 37 of these */
UR3:	MOVE.L	(A1)+,(A2)
		MULU.L	D1,D0
		EOR.L		D0,(A2)+
		DBRA		D2,UR3
		MOVE.L	D0,Ultra_seed1		/* save global for next time */
		MOVE		#148,Ultra_byt		/* 4*37 bytes left */
		MOVE.L	(SP)+,A2				/* restore */
		RTS
}
#endif

/*	Ultra_long32() returns a four-byte integer, U[-2147483648,2147483647]. It may,
	of course, be cast to unsigned long. */

long Ultra_long32()
{
	register long	result;
	
	if (Ultra_byt < 4) Ultra_Refill();
	result = *((long *) Ultra_ptr);
	Ultra_ptr += 4; Ultra_byt -= 4;
	return(result);
}

/*	Ultra_long31() returns a four-byte integer, U[0,2147483647]. */

long Ultra_long31()
{
	register long	result;
	
	if (Ultra_byt < 4) Ultra_Refill();
	result = *((long *) Ultra_ptr);
	Ultra_ptr += 4; Ultra_byt -= 4;
	return(result & 0x7FFFFFFF);
}

/*	Ultra_short16() returns a two-byte integer, U[-32768,32767]. */

short Ultra_short16()
{
	register short	result;
	
	if (Ultra_byt < 2) Ultra_Refill();
	result = *((short *) Ultra_ptr);
	Ultra_ptr += 2; Ultra_byt -= 2;
	return(result);
}

/*	Ultra_short15() returns a two-byte integer, U[0,32767]. */

short Ultra_short15()
{
	register short	result;
	
	if (Ultra_byt < 2) Ultra_Refill();
	result = *((short *) Ultra_ptr);
	Ultra_ptr += 2; Ultra_byt -= 2;
	return(result & 0x7FFF);
}

/*	Ultra_short8() returns a two-byte integer, U[-128,127].  It gets a random
	byte and casts it to short.  This operation extends the sign bit.
	Consequently, you may NOT cast this function to unsigned short/long (see 
	Ultra_short8u() below). */

short Ultra_short8()
{
	register short	result;
	
	if (Ultra_byt < 1) Ultra_Refill();
	result = (short) *Ultra_ptr;
	Ultra_ptr += 1; Ultra_byt -= 1;
	return(result);
}

/*	Ultra_short8u() returns a two-byte integer, U[0,255].  It proceeds as in
	Ultra_short8() but clears the high byte instead of extending the sign bit. */

short Ultra_short8u()
{
	register short	result;
	
	if (Ultra_byt < 1) Ultra_Refill();
	result = (short) *Ultra_ptr;
	Ultra_ptr += 1; Ultra_byt -= 1;
	return(result & 0xFF);
}

/*	Ultra_short7() returns a two-byte integer, U[0,127]. */

short Ultra_short7()
{
	register short	result;
	
	if (Ultra_byt < 1) Ultra_Refill();
	result = (short) (*Ultra_ptr & 0x7F);
	Ultra_ptr += 1; Ultra_byt -= 1;
	return(result);
}

/*	Ultra_short1() returns a two-byte integer, U[0,1], i.e., a Boolean variate.
	It calls Ultra_long32() and returns the bits one at a time. */

short Ultra_short1()
{
	register short	result;
	
	if (Ultra_bit <= 0) {
		Ultra_bits = Ultra_long32();
		Ultra_bit = 32;
	}
	result = (short)(Ultra_bits < 0);			/* return sign bit */
	Ultra_bits += Ultra_bits;						/* shift left by one */
	Ultra_bit--;
	return(result);
}

/*	Ultra_uni() returns a four-byte float, U(0,1), with >= 25 bits of precision.  This
	precision is achieved by continually testing the leading byte, b, of the mantissa.
	If b == 0, it is replaced with a new random byte and the decimal point readjusted.
	This procedure simultaneously ensures that Ultra_uni() never returns zero. */

float Ultra_uni()
{
	register double	fac = Ultra_2n31;
	register long		along;
	register short		extra;
	
	along = Ultra_long31();
	if (along >= 0x01000000) return((float)(fac*along));
	for (extra=0;!extra;) {						/* will not be an infinite loop */
		 extra = Ultra_short7();
		 fac *= Ultra_2n7;
	}
	along |= (((long)extra) << 24);
	return((float)(fac*along));
}

/*	Ultra_vni() returns a four-byte float, U(-1,1), with the same features as
	described above for Ultra_uni(). */
	
float Ultra_vni()
{
	register double	fac = Ultra_2n31;
	register long		along,limit = 0x01000000;
	register short		extra;
	
	if ((along = Ultra_long32()) >= limit)
		return((float)(fac*along));
	else if (-along >= limit)
		return((float)(fac*along));
	for (extra=0;!extra;) {
		 extra = Ultra_short7();
		 fac *= Ultra_2n7;
	}
	if (along >= 0) {
		along |= (((long)extra) << 24);
		return((float)(fac*along));
	}
	along = -along;
	along |= (((long)extra) << 24);
	return((float)(-fac*along));
}

/*	Ultra_duni() and Ultra_dvni() return double-precision U[0,1) and U(-1,1).  In
	both cases, zero IS a remote possibility.  These functions are intended for
	those occasions when seven significant figures are not enough.  If you need
	TYPE double, but not double PRECISION, then it is much faster to use
	Ultra_uni() or Ultra_vni() and cast -- implicitly or explicitly. */

double Ultra_duni()
{
	return(Ultra_long31()*Ultra_2n31 + ((unsigned long) Ultra_long32())*Ultra_2n63);
}

double Ultra_dvni()
{
	return(Ultra_long32()*Ultra_2n31 + ((unsigned long) Ultra_long32())*Ultra_2n63);
}

/*	Ultra_norm() returns a four-byte float, N(mu,sigma), where mu and sigma are the
	mean and standard deviation, resp., of the parent population.  The normal variates
	returned are exact, not approximate.  Ultra_norm() uses Ultra_vni() so there is no
	possibility of a result exactly equal to mu.  Note that mu and sigma must also be
	floats, not doubles. */

float Ultra_norm(float mu, float sigma)
{
	register float	fac,rsq,v1,v2;

	if ((v1 = Ultra_gauss) != 0.0) {		/* Is there one left? */
		Ultra_gauss = 0.0;
		return(sigma*v1 + mu);
	}
	do {
		v1 = Ultra_vni();
		v2 = Ultra_vni();
		rsq = v1*v1 + v2*v2;
	} while (rsq >= 1.0);
	fac = sqrt(-2.0*log(rsq)/rsq);
	Ultra_gauss = fac*v2;					/* Save the first N(0,1) */
	return(sigma*fac*v1 + mu);				/* and return the second. */
}

/*	Ultra_expo() returns a four-byte float, Exponential(mu).  The parameter, mu, is
	both the mean and standard deviation of the parent population.  It must be a
	float greater than zero. */

float Ultra_expo(float mu)
{
	return(-mu*log(Ultra_uni()));
}

/*	Ultra_SaveStart() and Ultra_RecallStart() save and restore, respectively, the
	global variables of UltraLibU, from Ultra_gauss through Ultra_ptr.  If you
	think it may be necessary to recall a sequence of random numbers exactly, FIRST
	call Ultra_SaveStart().  To recover the sequence later, call Ultra_RecallStart().
	If you wish to terminate the program and still recover the same random numbers,
	you must save the array Ultra_Remember[] to a file and read it back upon restart.
	For convenience, Ultra_Remember[] is declared in UltraU.h as an array of unsigned
	longs even though this is not really the case. */

void Ultra_SaveStart()
{
	register int	i;

	Ultra_Remember.Ugauss = Ultra_gauss;
	Ultra_Remember.Ubits = Ultra_bits;
	Ultra_Remember.Useed1 = Ultra_seed1;
	Ultra_Remember.Useed2 = Ultra_seed2;
	Ultra_Remember.Ubrw = Ultra_brw;
	Ultra_Remember.Ubyt = Ultra_byt;
	Ultra_Remember.Ubit = Ultra_bit;
	Ultra_Remember.Uptr = Ultra_ptr;
	for (i=0;i<37;i++) {
		Ultra_Remember.UFSR[i] = Ultra_FSR[i];
		Ultra_Remember.USWB[i] = Ultra_SWB[i];
	}
}

void Ultra_RecallStart()
{
	register int	i;

	Ultra_gauss = Ultra_Remember.Ugauss;
	Ultra_bits = Ultra_Remember.Ubits;
	Ultra_seed1 = Ultra_Remember.Useed1;
	Ultra_seed2 = Ultra_Remember.Useed2;
	Ultra_brw = Ultra_Remember.Ubrw;
	Ultra_byt = Ultra_Remember.Ubyt;
	Ultra_bit = Ultra_Remember.Ubit;
	Ultra_ptr = Ultra_Remember.Uptr;
	for (i=0;i<37;i++) {
		Ultra_FSR[i] = Ultra_Remember.UFSR[i];
		Ultra_SWB[i] = Ultra_Remember.USWB[i];
	}
}

/*	Ultra_Init() computes a few global constants, initializes others and fills
	in the initial Ultra_SWB array using the user-supplied seeds which must be
	non-zero unsigned longs.  It terminates by calling Ultra_SaveStart() so that
	you may recover the whole sequence of random numbers by calling Ultra_Init(),
	with the same seeds, then calling Ultra_RecallStart(). */

Boolean Ultra_Init()
{
	unsigned long tempbits,ul;
	short 		  i,j;
	
/* Two ulong seeds > 0 MUST be supplied by user.  There is, INTENTIONALLY, no default. */
	if ((Ultra_seed1 == 0) || (Ultra_seed2 == 0)) return(false);
	
 	for (i=0;i<37;i++) {
		tempbits = 0;
		for (j=32;j>0;j--) {
			Ultra_seed1 *= 69069;
			Ultra_seed2 ^= (Ultra_seed2 >> 15);
			Ultra_seed2 ^= (Ultra_seed2 << 17);
			ul = Ultra_seed1 ^ Ultra_seed2;
			tempbits = (tempbits >> 1) | (0x80000000 & ul);
		}
		Ultra_SWB[i] = tempbits;
	}
	Ultra_2n31 = ((2.0/65536)/65536);
	Ultra_2n63 = 0.5*Ultra_2n31*Ultra_2n31;
	Ultra_2n7 = 1.0/128;
	Ultra_gauss = 0.0;
	Ultra_byt = Ultra_bit = 0;
	Ultra_brw = ULTRABRW;			/* no borrow yet */
	Ultra_SaveStart();
	return(true);
}
