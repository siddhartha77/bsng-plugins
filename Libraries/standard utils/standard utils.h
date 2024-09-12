/*
 * $Workfile:: standard utils.h                                               $
 * $Revision:: 1                                                              $
 *
 * $Author:: Buck Rogers                                                      $
 * $Modtime:: 30.09.1997 17:49 Uhr                                            $
 *
 * $History:: standard utils.h                                                $
 * 
 * *****************  Version 1  *****************
 * User: Buck Rogers  Date: 30.09.1997   Time: 18:31 Uhr
 * Created in $/BSNG/Plugins/BSNG SDK/Libraries/standard utils
 * Adding subproject 'BSNG' to '$/'
 *
 * $NoKeywords::                                                              $
 */


#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define LSN(A)			 ((A)&0x0000000F)
#define MSN(A)			(((A)&0x000000F0)>>4)
#define LSB(A)			 ((A)&0x000000FF)
#define MSBLoWord(A)	(((A)&0x0000FF00)>>8)
#define LSBHiWord(A)	(((A)&0x00FF0000)>>16)
#define MSB(A)			(((A)&0xFF000000)>>24)


typedef unsigned char Str3[4];
typedef unsigned char Str4[5];
typedef unsigned char Str8[9];


// RANDOM NUMBER/STRING GENERATION UTILITIES

long            myOSRandom              (long seed);
Boolean			myBooleanRandom			(void);
unsigned short	myShortRangedRandom		(unsigned short min,unsigned short max);
unsigned long	myLongRangedRandom		(unsigned long  min,unsigned  long max);
long long		myLongLongRangedRandom	(long long  min,long  long max);
void			myBaseBRandomPStr		(Str255	   s,unsigned short b,unsigned short numDigits);
void			myBaseBRandomDigits		(StringPtr s,unsigned short b,unsigned short numDigits);
void			mySFBaseBRandomPStr		(Str255	   s,unsigned short b,unsigned short numSigDigits);
void			mySFBaseBRandomDigits	(StringPtr s,unsigned short b,unsigned short numSigDigits);
unsigned char	myRandom09Char			(void);
unsigned char	myRandomAZChar			(void);
unsigned char	myRandom09AZChar		(void);

// INTEGER/WHOLE NUMBER MATHEMATICAL/MANIPULATION ROUTINES

unsigned long	myABSL		(signed long	n);
unsigned short	myABSW		(signed short	n);
unsigned char	myArithROLB	(unsigned char	n,unsigned short b);							// similar to myROLB but shifting is done arithmetically and we EOR instead of OR
unsigned char	myArithRORB	(unsigned char	n,unsigned short b);							// similar to myRORB but shifting is done arithmetically and we EOR instead of OR
unsigned char	myROLB		(unsigned char	n,unsigned short b);							// Rotate 8 bit value n, b bits to the left
unsigned char	myRORB		(unsigned char	n,unsigned short b);							// Rotate 8 bit value n, b bits to the right
unsigned short	myROLW		(unsigned short	n,unsigned short b);							// Rotate 16 bit value n, b bits to the left
unsigned short	myRORW		(unsigned short	n,unsigned short b);							// Rotate 16 bit value n, b bits to the right
unsigned long	myROLL		(unsigned long	n,unsigned short b);							// Rotate 32 bit value n, b bits to the left
unsigned long	myROLLByteX	(unsigned long	n,unsigned short theByte,unsigned short b);
unsigned long	myROLLWordX	(unsigned long	n,unsigned short theWord,unsigned short b);
unsigned long	myRORL		(unsigned long	n,unsigned short b);							// Rotate 32 bit value n, b bits to the right
unsigned long	myRORLByteX	(unsigned long	n,unsigned short theByte,unsigned short b);
unsigned long	myRORLWordX	(unsigned long	n,unsigned short theWord,unsigned short b);

unsigned long	myIntegerSquareRoot(unsigned long n);										// extremely poor algorithm
unsigned long	myClosestIntegerSquareRoot(unsigned long n);								// extremely poor algorithm


// NUMERIC STRING and NUMBER <--> STRING ROUTINES

unsigned short	myBaseXCharToVal	(unsigned char c);										// c should be 0..9, A..Z, a..z

long			myPStrToNum			(const Str255	 s);
long			myCharsToNum		(const StringPtr s,unsigned short numDigits);
long long		myDigitsToNum		(const StringPtr s,unsigned short numDigits);
long long		myBaseBPStrToNum 	(const Str255	 s,unsigned short b);
void            myBase16PStrToHexArray(const Str255 s, char *result);
long long		myBaseBDigitsToNum	(const StringPtr s,unsigned short b,unsigned short numDigits);

void			myZeroPadPStr		(Str255 s,unsigned short paddedLength);
unsigned char	myValToBaseXChar 	(unsigned short v);
void			mySNumToPStr		(		  long n,Str255    s,unsigned short minDigits);
void			myUNumToPStr		(unsigned long n,Str255    s,unsigned short minDigits);
void			myLLNumToPStr		(	 long long n,Str255    s,unsigned short minDigits);
void			myULLNumToPStr		(unsigned long long n,Str255    s,unsigned short minDigits);
void			myUNumToBaseBPStr	(unsigned long n,Str255	   s,unsigned short b,unsigned short minDigits);
void			myLLNumToBaseBPStr	(unsigned long long n,Str255	   s,unsigned short b,unsigned short minDigits);
unsigned short	myUNumToDigits		(unsigned long n,StringPtr s,unsigned short minDigits);
unsigned short	myLLNumToDigits		(	 long long n,StringPtr s,unsigned short minDigits);
unsigned short	myULLNumToDigits	(unsigned long long n,StringPtr s,unsigned short minDigits);
unsigned short	myUNumToBaseBDigits	(unsigned long n,StringPtr s,unsigned short b,unsigned short minDigits);
unsigned short	myLLNumToBaseBDigits(unsigned long long n,StringPtr s,unsigned short b,unsigned short minDigits);

unsigned long	mySumOfPStr		  	(const Str255 s);										// does not include the length byte
unsigned long	myIndexWeightedSumOfPStr(const Str255 s);									// does not include the length byte
unsigned long	mySumOfChars		(const StringPtr s,unsigned short numChars);
unsigned long	mySumOfBaseXPStr  	(const Str255 s);										// does not include the length byte
unsigned long	mySumOfBaseXDigits	(const StringPtr s,unsigned short numDigits);

unsigned long	my4BytesAsUNum		(const StringPtr s);									// gets unsigned value of the 4 bytes at *s
unsigned long	my2BytesAsUNum		(const StringPtr s);									// gets unsigned value of the 2 bytes at *s

// STRING MANIPULATION ROUTINES

unsigned long	cStrLen(const Ptr s,unsigned long maxLen);
void			myCStrToPStr(unsigned char *s);
void            myPStrToCStr(Str255 s);

void 			myCopyPStr				(const Str255 orig,Str255 copy);
Boolean			myEqualPStr				(const Str255 s,const Str255 t);

short			myFindFirstInPStr		(const Str255 s,unsigned char c);					// returns 0 iff c is not in s
short			myFindLastInPStr		(const Str255 s,unsigned char c);					// returns 0 iff c is not in s

void 			myGetPStrSubString		(const Str255 s,short offset,unsigned short numChars,Str255 subStr);
void			myPrefixPStr			(Str255 s,const Str255 prefixStr);
void			myAppendPStr			(Str255 s,const Str255 suffixStr);
void			myAppendCharToPStr		(Str255 s,unsigned char c);
void			myAppendQuotedPStr		(Str255 s,const Str255 suffixStr);
void			myInsertInPStr			(Str255 s,const Str255 insertStr,short offset);		// does an 'insert before'
void 			myBracketPStrWithChar	(Str255 s,unsigned char c,signed short numBefore,signed short numAfter);
void			myUnBracketPStrWithChar	(Str255 s,unsigned char c,Boolean doBefore,Boolean doAfter);
void			myStripCharFromPStr		(Str255 s,unsigned char c);
void			myDeleteElementFromPStr	(Str255 s,unsigned short index);
void            myDeleteSubStringFromPStr(Str255 s, unsigned short index, short len);
void            myAlphaNumericPStr      (Str255 s);
void            myNumericPStr           (Str255 s);

void 			myUpCasePStr			(Str255 s);
void 			myLoCasePStr			(Str255 s);
void 			myTitleCasePStr			(Str255 s);
void			myTruncatePStr			(Str255 s,unsigned short maxLength);
void 			myReversePStr			(Str255 s);

void 			myMapPStr				(Str255 s,unsigned short *mapIndexes);
void			myInversePermutatePStr	(Str255 s,unsigned short *permIndexes);


// RESOURCE ROUTINES

Boolean		mySaveResource	(const Handle theResource);


// STANDARD FILE ROUTINES

Boolean		myPutFile		(FSSpec *theFSSPtr,OSType type,OSType creator,const Str255 prompt,const Str255 defaultName);
Boolean     myGetFile       (FSSpec *theFSSPtr, int numTypes, SFTypeList typeList);


// FILE ROUTINES 

OSErr		myCloseFile		(short *fileRefNum,FSSpec *fileFSSpec);
OSErr		myOpenResFile	(FSSpec *inputFSSPtr,short permission,short *inputRefNumPtr);


// TEXT FILE OUTPUT ROUTINES

void	myOutputPStr			(short outputRefNum,const Str255 s);
void	myOutputSuffixedPStr	(short outputRefNum,const Str255 s,const Str255 suffixStr);
void	myOutputText			(short outputRefNum,const Ptr TextPtr,unsigned long textLen);
void	myOutputSuffixedText	(short outputRefNum,const Ptr TextPtr,unsigned long textLen,const Str255 suffixStr);
void	myOutputResHeader		(short outputRefNum,OSType theType,short theID,const Str255 theName);

#ifdef __cplusplus
}
#endif