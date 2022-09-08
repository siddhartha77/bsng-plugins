UltraLibU  --  A pseudo-random-number library for Macintosh platforms

This library is a new C/Assembly implementation of the Ultra PRNG developed by Marsaglia, et al. (see leading reference in source).  It is designed primarily for Macintosh platforms and replaces an earlier (non-PPC) library, by this author, that is currently available in a number of public archives.

UltraLibU contains one low-level assembly routine, in a conditional-compilation block, for MC68020 (and higher) or PowerPC processors.  (Note: Pure C source for this routine is already available in the usual Internet archives.)  The assembly syntax is that of Metrowerks' CodeWarrior<TM> but adaptation to other compilers should be trivial.

Requirements:

Either an MC68020 (or higher) processor or a PowerPC processor.
A C compiler with an assembler for the processor used.

Public functions:

7 functions returning various forms of uniform long/short variates
1 function  returning Boolean variates
2 functions returning (float) U(0,1) and (float) U(-1,1) variates
2 functions returning (double) U(0,1) and (double) U(-1,1) variates
1 function  returning (float) Normal(mu,sigma) variates [exact, not approximate]
1 function  returning (float) Exponential(mu) variates  [exact, not approximate]

1 function to initialize the library
2 functions to save and restore the status of the PRNG [to reproduce a sequence exactly]

Special features:

Variates are HIGHLY RANDOM.  This PRNG uses a compound generator and produces numbers that are random even at the bit level.  Moreover, all the bits in each variate are random, not just the most sigificant bits.

Execution is VERY FAST, e.g.,
Booleans  -->   0.65 and 0.23 microseconds each on a Quadra 800 and PowerMac 9500/120, resp.
Normals   -->  24.0  and 2.74 microseconds each on a Quadra 800 and PowerMac 9500/120, resp.
[complete timing list in source]

The (float) U(0,1) and (float) U(-1,1) variates are automatically scaled.  Their mantissas always have at least 25 bits of precision no matter how small the return values.  Neither returns zero.

The period of this compound generator exceeds 10^356.

--------------------------------------------------------------------------------------------------

This package contains the following files:

(this file)
UltraLibU.c         (the library)
UltraU.h            (an essential header file)
UltraUTest.c        (a demonstration/test program)
UltraUTest.out.org  (the expected output of the test program, with Quadra 800 timing)

--------------------------------------------------------------------------------------------------

Please direct comments and queries regarding UltraLibU to

Michael P. McLaughlin
MITRE Corporation
MS W337
1820 Dolley Madison Blvd.
McLean, VA  22102
U.S.A.

Tel.   (703) 883-7658
FAX    (703) 883-1251
Email  mpmcl@mitre.org
