/*
 * Copyright 1995-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef OPENSSL_DES_H
# define OPENSSL_DES_H

typedef unsigned int DES_LONG;
typedef unsigned char DES_cblock[8];
typedef /* const */ unsigned char const_DES_cblock[8];
/*
 * With "const", gcc 2.8.1 on Solaris thinks that DES_cblock * and
 * const_DES_cblock * are incompatible pointer types.
 */

typedef struct DES_ks {
    union {
        DES_cblock cblock;
        /*
         * make sure things are correct size on machines with 8 byte longs
         */
        DES_LONG deslong[2];
    } ks[16];
} DES_key_schedule;

# define DES_KEY_SZ      (sizeof(DES_cblock))
# define DES_SCHEDULE_SZ (sizeof(DES_key_schedule))

# define DES_ENCRYPT     1
# define DES_DECRYPT     0

# define DES_ede2_cbc_encrypt(i,o,l,k1,k2,iv,e) \
        DES_ede3_cbc_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(e))

# define ITERATIONS 16
# define HALF_ITERATIONS 8

# define c2l(c,l)        (l =((DES_LONG)(*((c)++)))    , \
                         l|=((DES_LONG)(*((c)++)))<< 8L, \
                         l|=((DES_LONG)(*((c)++)))<<16L, \
                         l|=((DES_LONG)(*((c)++)))<<24L)

/* NOTE - c is not incremented as per c2l */
# define c2ln(c,l1,l2,n) { \
                        c+=n; \
                        l1=l2=0; \
                        switch (n) { \
                        case 8: l2 =((DES_LONG)(*(--(c))))<<24L; \
                        /* fall through */                          \
                        case 7: l2|=((DES_LONG)(*(--(c))))<<16L; \
                        /* fall through */                          \
                        case 6: l2|=((DES_LONG)(*(--(c))))<< 8L; \
                        /* fall through */                          \
                        case 5: l2|=((DES_LONG)(*(--(c))));      \
                        /* fall through */                          \
                        case 4: l1 =((DES_LONG)(*(--(c))))<<24L; \
                        /* fall through */                          \
                        case 3: l1|=((DES_LONG)(*(--(c))))<<16L; \
                        /* fall through */                          \
                        case 2: l1|=((DES_LONG)(*(--(c))))<< 8L; \
                        /* fall through */                          \
                        case 1: l1|=((DES_LONG)(*(--(c))));      \
                                } \
                        }

# define l2c(l,c)        (*((c)++)=(unsigned char)(((l)     )&0xff), \
                         *((c)++)=(unsigned char)(((l)>> 8L)&0xff), \
                         *((c)++)=(unsigned char)(((l)>>16L)&0xff), \
                         *((c)++)=(unsigned char)(((l)>>24L)&0xff))

/* NOTE - c is not incremented as per l2c */
# define l2cn(l1,l2,c,n) { \
                        c+=n; \
                        switch (n) { \
                        case 8: *(--(c))=(unsigned char)(((l2)>>24L)&0xff); \
                        /* fall through */                                     \
                        case 7: *(--(c))=(unsigned char)(((l2)>>16L)&0xff); \
                        /* fall through */                                     \
                        case 6: *(--(c))=(unsigned char)(((l2)>> 8L)&0xff); \
                        /* fall through */                                     \
                        case 5: *(--(c))=(unsigned char)(((l2)     )&0xff); \
                        /* fall through */                                     \
                        case 4: *(--(c))=(unsigned char)(((l1)>>24L)&0xff); \
                        /* fall through */                                     \
                        case 3: *(--(c))=(unsigned char)(((l1)>>16L)&0xff); \
                        /* fall through */                                     \
                        case 2: *(--(c))=(unsigned char)(((l1)>> 8L)&0xff); \
                        /* fall through */                                     \
                        case 1: *(--(c))=(unsigned char)(((l1)     )&0xff); \
                                } \
                        }


#  define ROTATE(a,n)     (((a)>>(n))+((a)<<(32-(n))))

#  define LOAD_DATA_tmp(a,b,c,d,e,f) LOAD_DATA(a,b,c,d,e,f,g)
#  define LOAD_DATA(R,S,u,t,E0,E1,tmp) \
        u=R^s[S  ]; \
        t=R^s[S+1]

/*
 * It recently occurred to me that 0^0^0^0^0^0^0 == 0, so there is no reason
 * to not xor all the sub items together.  This potentially saves a register
 * since things can be xored directly into L
 */

# define D_ENCRYPT(LL,R,S) { \
        LOAD_DATA_tmp(R,S,u,t,E0,E1); \
        t=ROTATE(t,4); \
        LL^= \
            DES_SPtrans[0][(u>> 2L)&0x3f]^ \
            DES_SPtrans[2][(u>>10L)&0x3f]^ \
            DES_SPtrans[4][(u>>18L)&0x3f]^ \
            DES_SPtrans[6][(u>>26L)&0x3f]^ \
            DES_SPtrans[1][(t>> 2L)&0x3f]^ \
            DES_SPtrans[3][(t>>10L)&0x3f]^ \
            DES_SPtrans[5][(t>>18L)&0x3f]^ \
            DES_SPtrans[7][(t>>26L)&0x3f]; }

 /*-
  * IP and FP
  * The problem is more of a geometric problem that random bit fiddling.
  0  1  2  3  4  5  6  7      62 54 46 38 30 22 14  6
  8  9 10 11 12 13 14 15      60 52 44 36 28 20 12  4
 16 17 18 19 20 21 22 23      58 50 42 34 26 18 10  2
 24 25 26 27 28 29 30 31  to  56 48 40 32 24 16  8  0

 32 33 34 35 36 37 38 39      63 55 47 39 31 23 15  7
 40 41 42 43 44 45 46 47      61 53 45 37 29 21 13  5
 48 49 50 51 52 53 54 55      59 51 43 35 27 19 11  3
 56 57 58 59 60 61 62 63      57 49 41 33 25 17  9  1

 The output has been subject to swaps of the form
 0 1 -> 3 1 but the odd and even bits have been put into
 2 3    2 0
 different words.  The main trick is to remember that
 t=((l>>size)^r)&(mask);
 r^=t;
 l^=(t<<size);
 can be used to swap and move bits between words.

 So l =  0  1  2  3  r = 16 17 18 19
         4  5  6  7      20 21 22 23
         8  9 10 11      24 25 26 27
        12 13 14 15      28 29 30 31
 becomes (for size == 2 and mask == 0x3333)
    t =   2^16  3^17 -- --   l =  0  1 16 17  r =  2  3 18 19
          6^20  7^21 -- --        4  5 20 21       6  7 22 23
         10^24 11^25 -- --        8  9 24 25      10 11 24 25
         14^28 15^29 -- --       12 13 28 29      14 15 28 29

 Thanks for hints from Richard Outerbridge - he told me IP&FP
 could be done in 15 xor, 10 shifts and 5 ands.
 When I finally started to think of the problem in 2D
 I first got ~42 operations without xors.  When I remembered
 how to use xors :-) I got it to its final state.
 */
# define PERM_OP(a,b,t,n,m) ((t)=((((a)>>(n))^(b))&(m)),\
        (b)^=(t),\
        (a)^=((t)<<(n)))

# define IP(l,r) \
        { \
        register DES_LONG tt; \
        PERM_OP(r,l,tt, 4,0x0f0f0f0fL); \
        PERM_OP(l,r,tt,16,0x0000ffffL); \
        PERM_OP(r,l,tt, 2,0x33333333L); \
        PERM_OP(l,r,tt, 8,0x00ff00ffL); \
        PERM_OP(r,l,tt, 1,0x55555555L); \
        }

# define FP(l,r) \
        { \
        register DES_LONG tt; \
        PERM_OP(l,r,tt, 1,0x55555555L); \
        PERM_OP(r,l,tt, 8,0x00ff00ffL); \
        PERM_OP(l,r,tt, 2,0x33333333L); \
        PERM_OP(r,l,tt,16,0x0000ffffL); \
        PERM_OP(l,r,tt, 4,0x0f0f0f0fL); \
        }

extern const DES_LONG DES_SPtrans[8][64];

/*
 * This is the DES encryption function that gets called by just about every
 * other DES routine in the library.  You should not use this function except
 * to implement 'modes' of DES.  I say this because the functions that call
 * this routine do the conversion from 'char *' to long, and this needs to be
 * done to make sure 'non-aligned' memory access do not occur.  The
 * characters are loaded 'little endian'. Data is a pointer to 2 unsigned
 * long's and ks is the DES_key_schedule to use.  enc, is non zero specifies
 * encryption, zero if decryption.
 */

void DES_encrypt1(DES_LONG *data, DES_key_schedule *ks, int enc);

/*
 * This functions is the same as DES_encrypt1() except that the DES initial
 * permutation (IP) and final permutation (FP) have been left out.  As for
 * DES_encrypt1(), you should not use this function. It is used by the
 * routines in the library that implement triple DES. IP() DES_encrypt2()
 * DES_encrypt2() DES_encrypt2() FP() is the same as DES_encrypt1()
 * DES_encrypt1() DES_encrypt1() except faster :-).
 */

void DES_encrypt2(DES_LONG *data, DES_key_schedule *ks, int enc);

void DES_encrypt3(DES_LONG *data, DES_key_schedule *ks1, DES_key_schedule *ks2,
                  DES_key_schedule *ks3);

void DES_decrypt3(DES_LONG *data, DES_key_schedule *ks1, DES_key_schedule *ks2,
                  DES_key_schedule *ks3);

void DES_ede3_cbc_encrypt(const unsigned char *input, unsigned char *output,
                          long length, DES_key_schedule *ks1,
                          DES_key_schedule *ks2, DES_key_schedule *ks3,
                          DES_cblock *ivec, int enc);
                          

void DES_set_key(const_DES_cblock *key, DES_key_schedule *schedule);

#  endif