/*
 * $Workfile:: UltraU.h                                                       $
 * $Revision:: 1                                                              $
 *
 * $Author:: Buck Rogers                                                      $
 * $Modtime:: 30.09.1997 17:50 Uhr                                            $
 *
 * $History:: UltraU.h                                                        $
 * 
 * *****************  Version 1  *****************
 * User: Buck Rogers  Date: 30.09.1997   Time: 18:31 Uhr
 * Created in $/BSNG/Plugins/BSNG SDK/Libraries/UltraLibU
 * Adding subproject 'BSNG' to '$/'
 *
 * $NoKeywords::                                                              $
 */


#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* UltraU.h -- This header file is an essential component of the UltraLibU library.

 ***** WARNING! *************** WARNING! *************** WARNING! ******

 You MUST call Ultra_Init() before using UltraLibU!  Also, you MUST supply
 values for Ultra_seed1 and Ultra_seed2 (both > 0) BEFORE calling
 Ultra_Init(). Ultra_Init() ends by calling Ultra_SaveStart(). */
 
extern unsigned long Ultra_seed1,Ultra_seed2;
 
Boolean Ultra_Init();

/* Ultra_Remember is really a non-long structure of 324 bytes.  It is
   defined here to facilitate saving to a file and restoring later. */

extern unsigned long Ultra_Remember[81];

/**********************************************************************/

/****** Function                            	Return value **********/      

long Ultra_long32();					/* U[-2147483648,2147483647] */
long Ultra_long31();					/* U[0,2147483647] */

short Ultra_short16();					/* U[-32768,32767] */
short Ultra_short15();					/* U[0,32767] */

short Ultra_short8();					/* (short) U[-128,127] */
short Ultra_short8u();					/* (short) U[0,255] */
short Ultra_short7();					/* (short) U[0,127] */

short Ultra_short1();					/* (short) U[0,1] */

float Ultra_uni();						/* U(0,1) with ³ 25-bit mantissa */
float Ultra_vni();						/* U(-1,1) but excluding zero */

double Ultra_duni();					/* U[0,1) with ² 63-bit mantissa */
double Ultra_dvni();					/* U(-1,1) with ² 63-bit mantissa */

float Ultra_norm(float mu,float sigma);	/* Normal(mean,std. dev. > 0) */
float Ultra_expo(float mu);				/* Exponential(mean > 0) */

/**********************************************************************/

/* The following functions are provided in order to allow you to reproduce
a sequence of random numbers, e.g., for duplicating a Monte Carlo simulation
exactly.  Ultra_Init() must be called BEFORE either of these two functions. */

void Ultra_SaveStart();					/* also called by Ultra_Init() */
void Ultra_RecallStart();

#ifdef __cplusplus
}
#endif
