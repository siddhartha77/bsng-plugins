#ifndef __ARACRYPT_H__
#define __ARACRYPT_H__

void ARACryptInit();
void ARACryptTransformString(unsigned char* csKey, unsigned char* csTarget, int targetLen);
void ARACryptSetKey(signed char* csKey);
void ARACryptTransformChar(unsigned char* cwsTarget);

#endif
