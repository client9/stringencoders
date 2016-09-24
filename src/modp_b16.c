/**
 * \file
 * <PRE>
 * MODP_B16 - High performance base16 encoder/decoder
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2005-2016  Nick Galbreath
 * All rights reserved.
 * Released under MIT license. See LICENSE for details.
 * </PRE>
 */
#include "modp_b16.h"
#include "config.h"
#include "modp_b16_data.h"
#include "modp_stdint.h"

size_t modp_b16_encode(char* dest, const char* str, size_t len)
{
    size_t i;
    const size_t buckets = len >> 2; /* i.e. i / 4 */
    const size_t leftover = len & 0x03; /* i.e. i % 4 */
    const uint8_t* srcChar;
    uint8_t* p = (uint8_t*)dest;
    uint8_t t1, t2, t3, t4;
    const uint32_t* srcInt = (const uint32_t*)str;
    uint32_t x;
    for (i = 0; i < buckets; ++i) {
        x = *srcInt++;
/*      t1 = *s++; t2 = *s++; t3 = *s++; t4 = *s++; */
#ifdef WORDS_BIGENDIAN
        t1 = (uint8_t)(x >> 24);
        t2 = (uint8_t)(x >> 16);
        t3 = (uint8_t)(x >> 8);
        t4 = (uint8_t)x;
#else
        t4 = (uint8_t)(x >> 24);
        t3 = (uint8_t)(x >> 16);
        t2 = (uint8_t)(x >> 8);
        t1 = (uint8_t)x;
#endif
        *p++ = gsHexEncodeC1[t1];
        *p++ = gsHexEncodeC2[t1];
        *p++ = gsHexEncodeC1[t2];
        *p++ = gsHexEncodeC2[t2];
        *p++ = gsHexEncodeC1[t3];
        *p++ = gsHexEncodeC2[t3];
        *p++ = gsHexEncodeC1[t4];
        *p++ = gsHexEncodeC2[t4];
    }

    srcChar = (const uint8_t*)srcInt;
    switch (leftover) {
    case 0:
        break;
    case 1:
        t1 = (uint8_t)*srcChar;
        *p++ = gsHexEncodeC1[t1];
        *p++ = gsHexEncodeC2[t1];
        break;
    case 2:
        t1 = (uint8_t)*srcChar++;
        t2 = (uint8_t)*srcChar;
        *p++ = gsHexEncodeC1[t1];
        *p++ = gsHexEncodeC2[t1];
        *p++ = gsHexEncodeC1[t2];
        *p++ = gsHexEncodeC2[t2];
        break;
    default: /* case 3 */
        t1 = (uint8_t)*srcChar++;
        t2 = (uint8_t)*srcChar++;
        t3 = (uint8_t)*srcChar;
        *p++ = gsHexEncodeC1[t1];
        *p++ = gsHexEncodeC2[t1];
        *p++ = gsHexEncodeC1[t2];
        *p++ = gsHexEncodeC2[t2];
        *p++ = gsHexEncodeC1[t3];
        *p++ = gsHexEncodeC2[t3];
    }
    *p = '\0';
    return (size_t)(p - (uint8_t*)dest);
}

size_t modp_b16_decode(char* dest, const char* str, size_t len)
{
    size_t i;
    uint8_t t0, t1, t2, t3;
    uint8_t* p = (uint8_t*)dest;
    uint32_t val1, val2;
    const uint8_t* s = (const uint8_t*)str;
    const size_t buckets = len >> 2; /* i.e. len / 4 */
    const size_t leftover = len & 0x03; /* i.e. len % 4 */
    if (leftover & 0x01) { /* i.e if leftover is odd,      */
        /* leftover==1 || leftover == 3 */
        return (size_t)-1;
    }

    /* read 4 bytes, output 2.
     * Note on PPC G4, GCC 4.0, it's quite a bit faster to
     * NOT use t0,t1,t2,t3, and just put the *s++ in the gsHexDecodeMap
     * lookup
     */
    for (i = 0; i < buckets; ++i) {
        t0 = *s++;
        t1 = *s++;
        t2 = *s++;
        t3 = *s++;
        val1 = gsHexDecodeD2[t0] | gsHexDecodeMap[t1];
        val2 = gsHexDecodeD2[t2] | gsHexDecodeMap[t3];
        if (val1 > 0xff || val2 > 0xff) {
            return (size_t)-1;
        }
        *p++ = (uint8_t)val1;
        *p++ = (uint8_t)val2;
    }

    if (leftover == 2) {
        val1 = gsHexDecodeD2[s[0]] | gsHexDecodeMap[s[1]];
        if (val1 > 0xff) {
            return (size_t)-1;
        }
        *p++ = (uint8_t)val1;
    }

    return (size_t)(p - (uint8_t*)dest);
}
