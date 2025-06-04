/*
 * $Workfile:: standard utils.c                                               $
 * $Revision:: 1                                                              $
 *
 * $Author:: Buck Rogers                                                      $
 * $Modtime:: 30.09.1997 17:49 Uhr                                            $
 *
 * $History:: standard utils.c                                                $
 * 
 * *****************  Version 1  *****************
 * User: Buck Rogers  Date: 30.09.1997   Time: 18:31 Uhr
 * Created in $/BSNG/Plugins/BSNG SDK/Libraries/standard utils
 * Adding subproject 'BSNG' to '$/'
 *
 * $NoKeywords::                                                              $
 */


#include "UltraU.h"
#include "standard utils.h"


/* ************************************************************************************************ */
//RANDOM NUMBER GENERATION UTILITIES

long myOSRandom(long seed)
{
#define MODULUS     0x7FFFFFFF  // 2^31 - 1 (prime number)
#define MULTIPLIER  0x41A7      // The constant multiplier
#define QUOTIENT    0x1F31D     // (MODULUS / MULTIPLIER)
#define REMAINDER   0xB14       // (MODULUS % MULTIPLIER)

    seed = MULTIPLIER * (seed % QUOTIENT) - REMAINDER * (seed / QUOTIENT);

    if (seed > 0) {
       return seed;
    } else {
       return seed + MODULUS;
    }
}

Boolean myBooleanRandom(void)
{
	return ((Boolean) Ultra_short1());
}

unsigned short myShortRangedRandom(unsigned short min,unsigned short max)
{
register unsigned short		qdRdm;		//treat return value as 0-65536
register unsigned long		range,t;

	qdRdm =(unsigned short) Ultra_short16();
	range =max-min;
	if (range != 1) t = (qdRdm * range) >> 16;		//now 0 <= t <= range
	else		    t = (qdRdm % 2);
	return (t+min);
}

unsigned long myLongRangedRandom(unsigned long min,unsigned long max)
{
register unsigned long	t,range=max-min;
	
	if (range < (1L << 16)) t=min + (unsigned long) ((unsigned short) myShortRangedRandom(0,range));
	else
	{
		do {
			t=(((unsigned long)myShortRangedRandom(0,range>>16))<<16) | myShortRangedRandom(0,range&0xFFFF);
			t+=min;
		} while ((t<min)||(t>max));
	}
	return (t);
}

long long myLongLongRangedRandom(long long min,long long max)
{
long long	t,range=max-min;
	
	if (range < (1LL << 32)) t=min + (long long) ((unsigned short) myShortRangedRandom(0,range));
	else
	{
		do {
			t=(((long long)myShortRangedRandom(0,range>>16))<<32) | myShortRangedRandom(0,range&0xFFFF);
			t+=min;
		} while ((t<min)||(t>max));
	}
	return (t);
}

/* ************************************************************************************************ */
//INTEGER/WHOLE NUMBER MATHEMATICAL/MANIPULATION ROUTINES

unsigned long myABSL(signed long n)
{
	return ((n >= 0) ? n : -n);
}

unsigned short myABSW(signed short n)
{
	return ((n >= 0) ? n : -n);
}

//warning in the bit rotation routines I have not bothered to do arguement reduction on b
//so if you pass an out of range b, you will always get zero.  I may change this at some point, so
//do not depend upon this 'functionality'

//the next two calls provide particularily wierd yet commonly used operation that is identical to
//the rotate operation with the important differences being that the sign is used to borrow when
//doing the right shifting components of the two operations and instead of oring we exclusive-or

unsigned char myArithROLB(unsigned char n,unsigned short b)
{
	register signed char	v=(signed char) n;
	
	return ((v << b) ^ (v >> (8 - b)));
}

unsigned char myArithRORB(unsigned char n,unsigned short b)
{
	register signed char	v=(signed char) n;
	
	return ((v >> b) ^ (v << (8 - b)));
}

unsigned char myROLB(unsigned char n,unsigned short b)
{
	register unsigned char	v=n;
	
	return ((v << b) | (v >> (8 - b)));
}

unsigned char myRORB(unsigned char n,unsigned short b)
{
	register unsigned char	v=n;
	
	return ((v >> b) | (v << (8 - b)));
}

unsigned short myROLW(unsigned short n,unsigned short b)
{
	register unsigned short	v=n;
	
	return ((v << b) | (v >> (16 - b)));
}

unsigned short myRORW(unsigned short n,unsigned short b)
{
	register unsigned short	v=n;
	
	return ((v >> b) | (v << (16 - b)));
}

unsigned long myROLL(unsigned long n,unsigned short b)
{
	register unsigned long	v=n;
	
	return ((v << b) | (v >> (32 - b)));
}

unsigned long myROLLByteX(unsigned long n,unsigned short theByte,unsigned short b)
{
	register unsigned short	j=theByte * 8;
	register unsigned long	u=n;
	register unsigned char	v=(unsigned char) ((u >> j) & 0x000000FF);
	
	return ((u ^ (v << j)) | ((((v << b) | (v >> (8 - b))) & 0x000000FF) << j));
}

unsigned long myROLLWordX(unsigned long n,unsigned short theWord,unsigned short b)
{
	register unsigned short	j=theWord * 16;
	register unsigned long	u=n;
	register unsigned short	v=(unsigned short) ((u >> j) & 0x0000FFFF);
	
	return ((u ^ (v << j)) | ((((v << b) | (v >> (16 - b))) & 0x0000FFFF) << j));
}

unsigned long myRORL(unsigned long n,unsigned short b)
{
	register unsigned long	v=n;
	
	return ((v >> b) | (v << (32 - b)));
}

unsigned long myRORLByteX(unsigned long n,unsigned short theByte,unsigned short b)
{
	register unsigned short	j=theByte * 8;
	register unsigned long	u=n;
	register unsigned char	v=(unsigned char) ((u >> j) & 0x000000FF);
	
	return ((u ^ (v << j)) | ((((v >> b) | (v << (8 - b))) & 0x000000FF) << j));
}

unsigned long myRORLWordX(unsigned long n,unsigned short theWord,unsigned short b)
{
	register unsigned short	j=theWord * 16;
	register unsigned long	u=n;
	register unsigned short	v=(unsigned short) ((u >> j) & 0x0000FFFF);
	
	return ((u ^ (v << j)) | ((((v >> b) | (v << (16 - b))) & 0x0000FFFF) << j));
}

unsigned long myIntegerSquareRoot(unsigned long n)	//extremely poor sqrRoot algorithm
{
register unsigned long v,sqr;

	v=n/2;
	sqr=v*v;
	while (sqr>n)
	{
		v=v/2;
		sqr=v*v;
	}
	while (sqr<n)
	{
		v+=1;
		sqr=v*v;
	}
	if (sqr>n) v--;
	return v;
}

unsigned long myClosestIntegerSquareRoot(unsigned long n)	//extremely poor algorithm
{
register unsigned long v,sqr;

	v=myIntegerSquareRoot(n);									//now	v*v <= n < (v+1)*(v+1)
	sqr=v*v;
	if ((n-sqr) > (sqr + (v<<1) +1 - n)) v++;				//v=v+1 iff (n-v*v) > ((v+1)*(v+1) - n)
	return v;	
	//note (sqr + (v<<1) +1) = v*v + 2*v + 1 = (v+1) * (v+1)
}

/* ************************************************************************************************ */
//NUMERIC STRING and NUMBER<-->STRING ROUTINES

unsigned short myBaseXCharToVal(unsigned char c)	//c should be '0'..'9','A'..'Z','a'..'z'
{
	if (c<='9') return (c-'0');
	if (c<='Z') return (c-'A'+10);
	if (c<='z') return (c-'a'+36);
	return (0);
}

/*
	StringToNum() is well defined even for non-numeric strings and is equivalent to:
*/
long myPStrToNum(const Str255 s)
{
	return myCharsToNum((StringPtr) &s[1],s[0]);
}

long myCharsToNum(const StringPtr s,unsigned short numDigits)
{
register long	v;
register short	i,len=numDigits;
register short	sign=s[0];

	v=0;
	if (len)
	{
		if ((sign=='+')||(sign=='-'))
		{
			for (i=1;i<len;i++) v=(v*10)+(s[i]&0x0F);	//note &0x0F==%16 but &0x0F is faster
			if (sign=='-') v*=-1;
		}
		else for (i=0;i<len;i++) v=(v*10)+(s[i]&0x0F);	//note &0x0F==%16 but &0x0F is faster	
	}
	return v;
}

long long myDigitsToNum(const StringPtr s,unsigned short numDigits)
{
	return myBaseBDigitsToNum(s,10,numDigits);
}

long long myBaseBPStrToNum(const Str255 s,unsigned short b)
{
	return myBaseBDigitsToNum((StringPtr) &s[1],b,s[0]);
}

void myBase16PStrToHexArray(const Str255 s, char *result)
{
    int i, j;
    
    for (i = 1, j = 0; i <= s[0] ; i += 2, ++j)
    {
        result[j] = (myBaseXCharToVal(s[i]) << 4) + myBaseXCharToVal(s[i+1]);
    }
}

long long myBaseBDigitsToNum(const StringPtr s,unsigned short b,unsigned short numDigits)
{
register long long		v = 0;				//defined so v can hold a positive 64 bit number
register short			i,len=numDigits;
register short			sign=s[0];

	if (len)
	{
		if ((sign == '+') || (sign == '-'))
		{
			for (i = 1; i < len; i++)  v = (v * b) + myBaseXCharToVal(s[i]);
			if (sign == '-') return (- ((long long) v));
		}
		else for (i = 0; i < len; i++) v = (v * b) + myBaseXCharToVal(s[i]);
	}
	return ((long long) v);
}

void myBaseBRandomPStr(Str255 s,unsigned short b,unsigned short numDigits)
{
	s[0]=numDigits;
	myBaseBRandomDigits(&s[1],b,numDigits);
}

void myBaseBRandomDigits(StringPtr s,unsigned short b,unsigned short numDigits)
{
register short	i;
register short	max = b - 1;

	//for (i=numDigits-1;i>=0;i--) s[i] = myValToBaseXChar(myShortRangedRandom(0,max));
    for (i=numDigits;i>=0;i--) s[i] = myValToBaseXChar(myShortRangedRandom(0,max));
    s[0] = numDigits;
}

void mySFBaseBRandomPStr(Str255 s,unsigned short b,unsigned short numSigDigits)
{
	s[0]=numSigDigits;
	mySFBaseBRandomDigits(&s[1],b,numSigDigits);
}

void mySFBaseBRandomDigits(StringPtr s,unsigned short b,unsigned short numSigDigits)
{
register short	i;
register short	max = b - 1;

	for (i=numSigDigits-1;i>=1;i--) s[i] = myValToBaseXChar(myShortRangedRandom(0,max));
	s[0] = myValToBaseXChar(myShortRangedRandom(1,max));
}

unsigned char myRandom09Char(void)
{
	return ((unsigned char) myShortRangedRandom('0','9'));
}

unsigned char myRandomAZChar(void)
{
	return ((unsigned char) myShortRangedRandom('A','Z'));
}

unsigned char myRandom09AZChar(void)
{
unsigned char	c;

	c = myShortRangedRandom(0,35);
	c+= (c < 10) ? '0' : 'A'-10;
	return (c);
}

void myZeroPadPStr(Str255 s,unsigned short paddedLength)
{
register short	paddingLength=paddedLength-s[0];

	if (paddingLength>0)
	{
		BlockMove((Ptr) &s[1],(Ptr) &s[paddingLength+1],s[0]);
		s[0]+=paddingLength;
		while (paddingLength>0) s[paddingLength--]='0';
	}
}

unsigned char myValToBaseXChar(unsigned short v)
{
	if (v<10) return ('0'+v);		//00..09  -> '0'..'9'
	if (v<36) return ('A'-10+v);	//10..35  -> 'A'..'Z'
	return ('a'-36+v);				//36..61+ -> 'a'..'z'+
}

void mySNumToPStr(long num,Str255 s,unsigned short minDigits)
{
	if (num >= 0)
	{
		NumToString(num,s);
		myZeroPadPStr(s,minDigits);
	}
	else
	{
		NumToString(-num,s);
		myZeroPadPStr(s,minDigits);
		if (s[1]=='0') s[1]='-';
		else myPrefixPStr(s,"\p-");
	}

}

void myUNumToPStr(unsigned long n,Str255 s,unsigned short minDigits)
{
	s[0] = myUNumToDigits(n,&s[1],minDigits);
}

void myLLNumToPStr(long long n,Str255 s,unsigned short minDigits)
{
	s[0] = myLLNumToDigits(n,&s[1],minDigits);
}

void myULLNumToPStr(unsigned long long n,Str255 s,unsigned short minDigits)
{
	s[0] = myULLNumToDigits(n,&s[1],minDigits);
}

void myUNumToBaseBPStr(unsigned long n,Str255 s,unsigned short b,unsigned short minDigits)
{ 
	s[0] = myUNumToBaseBDigits(n,&s[1],b,minDigits);
}

void myLLNumToBaseBPStr(unsigned long long n,Str255 s,unsigned short b,unsigned short minDigits)
{ 
	s[0] = myLLNumToBaseBDigits(n,&s[1],b,minDigits);
}

unsigned short myLLNumToDigits(long long n,StringPtr s,unsigned short minDigits)
{
register char	numStr[19];		//only 19 decimal digits are possible from 64 bits
register short	start,i,j=0;

	do
	{
		numStr[j++]='0'+(n%10);
		n/=10;
	} while (n>0);
	start=(j<minDigits) ? (minDigits-j) : 0;
	for (i=0;i<start;i++) s[i]='0';
	for (i=start;j>0;i++) s[i]=numStr[--j];
	return (i);
}

unsigned short myULLNumToDigits(unsigned long long n,StringPtr s,unsigned short minDigits)
{
register char	numStr[20];		//only 20 decimal digits are possible from 64 bits
register short	start,i,j=0;

	do
	{
		numStr[j++]='0'+(n%10);
		n/=10;
	} while (n>0);
	start=(j<minDigits) ? (minDigits-j) : 0;
	for (i=0;i<start;i++) s[i]='0';
	for (i=start;j>0;i++) s[i]=numStr[--j];
	return (i);
}

unsigned short myUNumToDigits(unsigned long n,StringPtr s,unsigned short minDigits)
{
register char	numStr[10];		//only 10 decimal digits are possible from unsigned 32 bits
register short	start,i,j=0;

	do
	{
		numStr[j++]='0'+(n%10);
		n/=10;
	} while (n>0);
	start=(j<minDigits) ? (minDigits-j) : 0;
	for (i=0;i<start;i++) s[i]='0';
	for (i=start;j>0;i++) s[i]=numStr[--j];
	return (i);
}

unsigned short myUNumToBaseBDigits(unsigned long n,StringPtr s,unsigned short b,unsigned short minDigits)
{
register char	numStr[32];		//32 binary digits are possible from unsigned 32 bits
register short	start,i,j=0;

	if ((b<2) || (b>62)) return (0);	//error - illegal base was passed
	do
	{
		numStr[j++]=myValToBaseXChar(n%b);
		n/=b;
	} while (n>0);
	start=(j<minDigits) ? (minDigits-j) : 0;
	for (i=0;i<start;i++) s[i]='0';
	for (i=start;j>0;i++) s[i]=numStr[--j];
	return (i);
}

unsigned short myLLNumToBaseBDigits(unsigned long long n,StringPtr s,unsigned short b,unsigned short minDigits)
{
register char	numStr[64];		//32 binary digits are possible from unsigned 32 bits
register short	start,i,j=0;

	if ((b<2) || (b>62)) return (0);	//error - illegal base was passed
	do
	{
		numStr[j++]=myValToBaseXChar(n%b);
		n/=b;
	} while (n>0);
	start=(j<minDigits) ? (minDigits-j) : 0;
	for (i=0;i<start;i++) s[i]='0';
	for (i=start;j>0;i++) s[i]=numStr[--j];
	return (i);
}

unsigned long mySumOfPStr(const Str255 s)						//does not include the length byte
{
register unsigned long	result=0;
register unsigned short	i;
	
	for (i = s[0]; i > 0; i--) result+=s[i];
	return (result);
}

unsigned long myIndexWeightedSumOfPStr(const Str255 s)			//does not include the length byte
{
register unsigned long	result=0;
register unsigned short	i;
	
	for (i = s[0]; i > 0; i--) result+=i * s[i];
	return (result);
}

unsigned long mySumOfChars(const StringPtr s,unsigned short numChars)
{
register unsigned long	result=0;
register unsigned short	i;
	
	for (i = 0; i < numChars; i++) result+=s[i];
	return (result);
}

unsigned long mySumOfBaseXPStr(const Str255 s)					//does not include the length byte
{
register unsigned long	result=0;
register unsigned short	i;
	
	for (i = s[0]; i > 0; i--) result+=myBaseXCharToVal(s[i]);
	return (result);
}

unsigned long mySumOfBaseXDigits(const StringPtr s,unsigned short numDigits)
{
register unsigned long	result=0;
register unsigned short	i;
	
	for (i = 0; i < numDigits; i++) result+=myBaseXCharToVal(s[i]);
	return (result);
}

unsigned long my4BytesAsUNum(const StringPtr s)
{
register unsigned long	v;
register StringPtr		c=s;

	v =*(c++)<<8;
	v|=*(c++);
	v<<=8;
	v|=*(c++);
	v<<=8;
	v|=*c;
	return (v);
}

unsigned long my2BytesAsUNum(const StringPtr s)
{
register unsigned long	v;
register StringPtr		c=s;
	
	v =*(c++)<<8;
	v|=*c;
	return (v);
}

/* ************************************************************************************************ */
//STRING MANIPULATION ROUTINES

unsigned long cStrLen(const Ptr s,unsigned long maxLen)
{
register Ptr c=s;
register unsigned long len=0;
	
	while ((len < maxLen) && ((Byte *) *(c++)!=0x00)) len++;
	return len;
}

void myCStrToPStr(unsigned char *s)
{
unsigned char	*t = s;
unsigned char	*p = s;
unsigned long	len;

	while (*t != 0) t++;
	len = t - s;
	p = t;
	while (p != s) *(p--) = *(--t);
	if (len > 255)
	{
		DebugStr("\pmyCStrToPStr: string too long");
		len = 255;
	}
	s[0] = len;
}

void myPStrToCStr(Str255 s)
{
    short count = s[0];
    short i;
    
    for (i = 0 ; i < count ; ++i) {
        s[i] = s[i + 1];
    }
    
    s[count] = '\0';
}

void myCopyPStr(const Str255 s,Str255 t)
{
	BlockMove((Ptr) s,(Ptr) t,s[0]+1);
}

Boolean myEqualPStr(const Str255 s,const Str255 t)
{
register short	i=s[0];

	if (s[0]==t[0])
	{
		while ((i>0) && (s[i]==t[i])) i--;
		return ((Boolean)(i==0));
	}
	return (FALSE);
}

short myFindFirstInPStr(const Str255 s,unsigned char c)
{
	register short	i	=1;
	register short	len	=s[0];
	
	while ((i <= len) && (s[i] != c)) i++;
	return ((i > len) ? 0 : i);
}

short myFindLastInPStr(const Str255 s,unsigned char c)
{
	register short	i	=s[0];
	
	while ((i > 0) && (s[i] != c)) i--;
	return (i);
}

void myGetPStrSubString(const Str255 s,short first,unsigned short numChars,Str255 subStr)
{
register unsigned short	i,len=s[0];
register unsigned short	j;
register unsigned short	end;

	if (first == 0) DebugStr("\pBad parameter passsed to myGetPStrSubString");
	if (first < 0)								//first is an offset from the end of s
	{
		first+=len + 1;							//map (-n..-1) to (1..len)
		if (first <= 0) first = 1;				//requesting more chars than available - truncate
	}
	if ((len==0) || (first>len)) subStr[0]=0;	//s is null or first out of range -- return empty s
	else
	{
		if (numChars == 0) numChars=len-first+1;//if (numChars == 0) subStr[1..n]=s[first..len]
		end=first + numChars - 1;
		if (end>len)							//requesting more chars than available -- truncate
		{
			numChars-=end-len;
			end=len;
		}
		for (i=end,j=numChars;i>=first;i--,j--) subStr[j]=s[i];
		subStr[0]=numChars;
	}
}

void myPrefixPStr(Str255 s,const Str255 prefixStr)
{
register unsigned short	i=s[0];
register unsigned short	j=prefixStr[0];
	
	if (j)
	{
		if ((i+j) <= 255)								//if room to prefix
		{
			BlockMove((Ptr) &s[1],(Ptr) &s[j+1],i);
			BlockMove((Ptr) &prefixStr[1],(Ptr) &s[1],j);	
			s[0]+=j;
		}
		else DebugStr("\pOverflow");
	}
}

void myAppendPStr(Str255 s,const Str255 suffixStr)
{
register unsigned short	i=s[0];
register unsigned short	j=suffixStr[0];
	
	if (j)
	{
		if ((i+j) <= 255)								//if room to append
		{
			BlockMove((Ptr) &suffixStr[1],(Ptr) &s[i+1],j);
			s[0]+=j;
		}
		else DebugStr("\pOverflow");
	}
}

void myAppendCharToPStr(Str255 s,unsigned char c)
{
	if (s[0] < 255) s[++s[0]] = c;
	else DebugStr("\pOverflow");
}

void myAppendQuotedPStr(Str255 s,const Str255 suffixStr)
{
register short		j=suffixStr[0];
	
	if ((j) && ((s[0]+2+j)<=255))	//if room to append
	{
		s[++s[0]] ='\'';
		BlockMove((Ptr) &suffixStr[1],(Ptr) &s[++s[0]],j);	//note this pre-adds 1 to s[0]
		s[s[0]+=j]='\'';									//index calculation also sets s[0]
	}
}

void myInsertInPStr(Str255 s,const Str255 insertStr,short offset)	//does an 'insert before'
{
register short	start,insertLen=insertStr[0];
register short	len=s[0];

	if (insertLen > 0)
	{
		if (offset <= 0)						//insert at 'offset' chars from end of s
		{
			start = len + offset + 1;
			if (start < 1) start = 1;			//underflow - prefix		
		}	
		else									//insert at 'offset' chars from start of s
		{
			if (offset > len) offset = len + 1;	//overflow - suffix
			start = offset;				
		}
		if (start <= len) BlockMove((Ptr) &s[start],(Ptr) &s[start+insertLen],len-start+1);	//make room for insertStr
		BlockMove((Ptr) &insertStr[1],(Ptr) &s[start],insertLen); 							//copy inserStr into s
		s[0]+=insertLen;
	}
}

void myBracketPStrWithChar(Str255 s,unsigned char c,signed short numBefore,signed short numAfter)
{
register short	i,j,len=s[0];
	
	if (numBefore < 0) numBefore = 0;
	if (numAfter  < 0) numAfter = 0;	
	if ((len + numBefore + numAfter) < 256)
	{
		if (numBefore)
		{
			i=len;												//i=initial length
			len+=numBefore;										//set len=prefixed length
			for (j=len; (i>0); i--) s[j--]=s[i];				//slide existing string forward
			while (j>0) s[j--]=c;								//write prefix
		}
		for (i=len,j=numAfter; j > 0 ; j--) s[++i]=c;			//write suffix
		s[0]=i;													//adjust length;
	}
}

void myUnBracketPStrWithChar(Str255 s,unsigned char c,Boolean doBefore,Boolean doAfter)
{
register unsigned short	len	= s[0];
register unsigned char	*t	=&s[1];
	
	if (doAfter)
	{
		while ((len > 0) && (s[len] == c)) len--;
		s[0]=len;
	}
	if (doBefore)
	{
		while ((len > 0) && (*(t++) == c)) len--;	
		//note: len != s[0] implies that *(--t) is the first non-c character in s	
		if (len != s[0])	
		{
			BlockMove((Ptr) --t,(Ptr) &s[1],len);
			s[0]=len;
		}
	}
}

void myStripCharFromPStr(Str255 s,unsigned char c)
{
register unsigned short		i,j,len = s[0];

	for (i = 1, j = 0; i <= len; i++) if (s[i] != c) s[++j] = s[i];
	s[0] = j;
}

void myDeleteElementFromPStr(Str255 s,unsigned short index)
{
register unsigned short		i,len = s[0];

	if (index <= len)
	{
		for (i = index; i < len; i++) s[i] = s[i + 1];
		s[0]--;
	}
}

void myDeleteSubStringFromPStr(Str255 s, unsigned short index, short len)
{
    int i;
    
    for (i = 0 ; i < len ; i++) {
        myDeleteElementFromPStr(s, index);
    }
}

void myAlphaNumericPStr(Str255 s) {
    Str255  buffer;
    int i;
    
    myCopyPStr("\p", buffer);
    
    for (i = 1 ; i <= *s ; ++i) {
        if ((s[i] >= 'a' && s[i] <= 'z') ||
            (s[i] >= 'A' && s[i] <= 'Z') ||
            (s[i] >= '0' && s[i] <= '9')) {
            myAppendCharToPStr(buffer, s[i]);
        }
    }
    
    myCopyPStr(buffer, s);
}

void myNumericPStr(Str255 s) {
    Str255  buffer;
    int i;
    
    myCopyPStr("\p", buffer);
    
    for (i = 1 ; i <= *s ; ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            myAppendCharToPStr(buffer, s[i]);
        }
    }
    
    myCopyPStr(buffer, s);
}

void myUpCasePStr(Str255 s)
{
short			i;

	for (i=s[0];i>0;i--) if ((s[i]>='a') && (s[i]<='z')) s[i]-='a'-'A';
}

void myLoCasePStr(Str255 s)
{
register short	i;

	for (i=s[0];i>0;i--) if ((s[i]>='A') && (s[i]<='Z')) s[i]+='a'-'A';
}

void myTitleCasePStr(Str255 s)
{
register short	i=1;
register short	len=s[0];

	if (len)
	{
		myLoCasePStr(s);
		while (i<=len)
		{
			if ((s[i]>='a') && (s[i]<='z')) s[i]-='a'-'A';	//capitalize the ith character
			while ((i<=len)  && (s[i]!=' ')) i++;			//skip until space
			while ((i<=len)  && (s[i]==' ')) i++;			//skip spaces
		}
	}
}

void myTruncatePStr(Str255 s,unsigned short maxLength)
{
	if (s[0] > (unsigned short) maxLength) s[0]=(unsigned char) maxLength;
}

void myReversePStr(Str255 s)
{
register short			i,j;
register unsigned char	t;

	for (i=s[0],j=1 ;i>j; i--,j++)	//note this algorithm will not waste time exchanging
	{								//a len=1 string or middle element with itself
		t=s[i];
		s[i]=s[j];
		s[j]=t;
	} 
}

void myMapPStr(Str255 s,unsigned short *permIndexes)
{
register unsigned short	i,len = s[0];
register Str255			t;

	//do the permutation
	for (i = 0; i <= len; i++) t[i] = s[permIndexes[i]];
	//t may or may not be a PStr now, so do a direct copy
	BlockMove( (Ptr) t, (Ptr) s, len + 1);
}

void myInversePermutatePStr(Str255 s,unsigned short *permIndexes)
{
register unsigned short	i,len = s[0];
register Str255			t;

	//do the permutation
	for (i = 0; i <= len; i++) t[permIndexes[i]] = s[i];
	//t may or may not be a PStr now, so do a direct copy
	BlockMove( (Ptr) t, (Ptr) s, len + 1);
}

/* ************************************************************************************************ */
//RESOURCE ROUTINES

Boolean mySaveResource(const Handle theResource)
{
	short	resAttributes;
	Boolean itWorked;
	
	resAttributes=GetResAttrs(theResource);
	if (resAttributes&resProtected)
	{
		SetResAttrs(theResource,resAttributes ^ resProtected);	//set as unprotected
		ChangedResource(theResource);
		itWorked=(Boolean) (ResError()==noErr); 
		if (itWorked) WriteResource(theResource);
		SetResAttrs(theResource,resAttributes);	//set as protected
		//this setting will be saved even though we don't call changed/write resource
	}
	else 
	{
		ChangedResource(theResource);
		itWorked=(Boolean) (ResError()==noErr); 
		if (itWorked) WriteResource(theResource);
	}
	if (!itWorked) DebugStr("\pError returned by ChangedResource - cannot save");
	return itWorked;
}

/* ************************************************************************************************ */
//STANDARD FILE ROUTINES

Boolean myPutFile(FSSpec *theFSSPtr,OSType type,OSType creator,const Str255 prompt,const Str255 defaultName)
{
	StandardFileReply 	outputReply;
	OSErr				errorCode=noErr;
	
putFile:
	StandardPutFile(prompt,defaultName,&outputReply);
	if (outputReply.sfGood)
	{
		if (outputReply.sfReplacing)
		{
			errorCode=FSpDelete(&(outputReply.sfFile));
			if (errorCode!=noErr)
			{
				SysBeep(10);	
				goto putFile;
			}	
		}
		errorCode=FSpCreate(&(outputReply.sfFile),creator,type,outputReply.sfScript);
		*theFSSPtr=outputReply.sfFile;									//copy the FSS
	}
	return ((outputReply.sfGood)&&(errorCode==noErr));
}

Boolean myGetFile(FSSpec *theFSSPtr, int numTypes, SFTypeList typeList)
{
	StandardFileReply 	reply;
	
	StandardGetFile(nil,numTypes,typeList,&reply);
	*theFSSPtr=reply.sfFile;											//copy the FSS 
	return ((reply.sfGood)&&(!reply.sfIsFolder)&&(!reply.sfIsVolume));	//file was selected?
}

/* ************************************************************************************************ */
//FILE ROUTINES 

OSErr myCloseFile(short *fileRefNum,FSSpec *fileFSSpec)
{
	OSErr	errorCode;
	
	errorCode=FSClose(*fileRefNum);
	*fileRefNum=0;							//safety to prevent double closing which can trash HD's
	if (errorCode!=noErr) DebugStr("\pError closing file");
	errorCode=FlushVol(nil,fileFSSpec->vRefNum);  					//ensure file data is written
	if (errorCode!=noErr) DebugStr("\pError flushing volume");
	return (errorCode);
}

OSErr myOpenResFile(FSSpec *inputFSSPtr,short permission,short *inputRefNumPtr)
{
	short	oldResFile;
	OSErr	errorCode=noErr;
	
	oldResFile=CurResFile();
	SetResLoad(FALSE);							//stop resources in users file from auto preloading
	*inputRefNumPtr=FSpOpenResFile(inputFSSPtr,permission);
	if (*inputRefNumPtr==-1) errorCode=ResError();
	SetResLoad(TRUE);							//resume normal loading of resources
	UseResFile(oldResFile);
	return errorCode;
}

/* ************************************************************************************************ */
//TEXT FILE OUTPUT ROUTINES

void myOutputPStr(short outputRefNum,const Str255 s)
{
	long	inOutCount=(long) s[0];
	OSErr	errorCode;
	
	if (inOutCount!=0)
	{
		errorCode = FSWrite(outputRefNum,&inOutCount,&s[1]);
		if (errorCode!=noErr) DebugStr("\pError during write");
	}
}

void myOutputSuffixedPStr(short outputRefNum,const Str255 s,const Str255 suffixStr)
{
	long	inOutCount=(long) s[0];
	OSErr	errorCode;
	
	if (inOutCount!=0)
	{
		errorCode = FSWrite(outputRefNum,&inOutCount,&s[1]);
		if (errorCode!=noErr) DebugStr("\pError during write");
		else
		{
			inOutCount=(long) suffixStr[0];
			if (inOutCount!=0)
			{
				errorCode = FSWrite(outputRefNum,&inOutCount,&suffixStr[1]);
				if (errorCode!=noErr) DebugStr("\pError during write");
			}
		}
	}
}

void myOutputText(short outputRefNum,const Ptr TextPtr,unsigned long textLen)
{
	long	inOutCount=(long) textLen;
	OSErr	errorCode;
	
	if (inOutCount!=0)
	{
		errorCode = FSWrite(outputRefNum,&inOutCount,TextPtr);
		if (errorCode!=noErr) DebugStr("\pError during write");
	}
}

void myOutputSuffixedText(short outputRefNum,const Ptr TextPtr,unsigned long textLen,const Str255 suffixStr)
{
	long	inOutCount=(long) textLen;
	OSErr	errorCode;
	
	if (textLen!=0)
	{
		errorCode = FSWrite(outputRefNum,&inOutCount,TextPtr);
		if (errorCode!=noErr) DebugStr("\pError during write");
		else
		{
			inOutCount=(long) suffixStr[0];
			if (inOutCount!=0)
			{
				errorCode = FSWrite(outputRefNum,&inOutCount,&suffixStr[1]);
				if (errorCode!=noErr) DebugStr("\pError during write");
			}
		}
	}
}

void myOutputResHeader(short outputRefNum,OSType theType,short theID,const Str255 theName)
{
	Str255		idStr,headerStr={"\p'????' #"};
	const short	offsetToOSType=2;	//offset into headerStr at which to copy the OSType
	long		inOutCount;
	OSErr		errorCode;
	
	BlockMove((Ptr) &theType,(Ptr) (headerStr+offsetToOSType),4);	//copy OSType into headerStr
	NumToString((long) theID,idStr);
	myAppendPStr(headerStr,idStr);
	if (theName[0]!=0)
	{
		myAppendPStr(headerStr,"\p '");
		myAppendPStr(headerStr,theName);
		myAppendPStr(headerStr,"\p'");
	}
	myAppendPStr(headerStr,"\p\r\r");
	inOutCount=headerStr[0];
	errorCode = FSWrite(outputRefNum,&inOutCount,(Ptr) &headerStr[1]);
	if (errorCode!=noErr) DebugStr("\pError during write");
}
