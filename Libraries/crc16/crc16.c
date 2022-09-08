#include "crc16.h"

long crc16CCITTEncode(char *bytes, unsigned long byteCount, long init) {
    long x = init;
    long y;
    int i;
    int j;
    
    for (i = 0 ; i < byteCount ; ++i) {
        x ^= bytes[i] << 8;
        
        for (j = 0 ; j < 7 ; ++j) {
            if (x >= 0) y = 2 * x;
            else y = (2 * x) ^ 0x1021;
            x = y;
        }
    }
    
    return x & 0xffff;
}