/**
 * \file
 * <PRE>
 * MODP_B36 - High performance base36 encoder/decoder
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2006-2016  Nick Galbreath
 * All rights reserved.
 *
 * Released under MIT license. See LICENSE for details.
 * </pre>
 */

/* exported public header */
#include "modp_b36.h"
#include "modp_stdint.h"
/* private header */
#include "modp_b36_data.h"

/*
 * Might need changing depending on platform
 * we need htonl, and ntohl
 */
#include <arpa/inet.h>

/**
 * you can decode IN PLACE!
 * no memory allocated
 */
size_t modp_b36_decode(char* out, const char* data, size_t len)
{
    size_t i;
    int j;
    const size_t buckets = len / 7;
    const uint8_t* d2 = (const uint8_t*)data;
    if (len % 7 != 0) {
        return (size_t)-1;
    }

    uint32_t* o2 = (uint32_t*)out;
    for (i = 0; i < buckets; ++i) {
        uint32_t tmp = 0;
        for (j = 0; j < 7; ++j) {
            uint32_t digit = gsCharToInt[(uint32_t)*d2++];
            if (digit >= 36) {
                return (size_t)-1;
            }
            tmp = tmp * 36 + digit;
        }
        *o2++ = htonl(tmp);
    }
    return 4 * buckets;
}

/**
 * src != out
 */
size_t modp_b36_encode(char* out, const char* src, size_t len)
{
    const uint32_t* sary = (const uint32_t*)src;
    const size_t buckets = len / 4;
    if (len % 4 != 0) {
        return (size_t)-1;
    }

    size_t i;
    for (i = 0; i < buckets; ++i) {
        uint32_t tmp = *sary++;
        tmp = htonl(tmp);

/* this crazy function */
#if 0
        *out++ =  (char)gsIntToChar[(tmp / 52200625)]; // don't need % 36 here, always < 36
        *out++ =  (char)gsIntToChar[(tmp / 614125) % 36];
        *out++ =  (char)gsIntToChar[(tmp / 7225) % 36];
        *out++ =  (char)gsIntToChar[(tmp / 36) % 36];
        *out++ =  (char)gsIntToChar[tmp % 36];
#else
        /* is really this */
        *(out + 6) = gsIntToChar[tmp % 36];
        tmp /= 36;
        *(out + 5) = gsIntToChar[tmp % 36];
        tmp /= 36;
        *(out + 4) = gsIntToChar[tmp % 36];
        tmp /= 36;
        *(out + 3) = gsIntToChar[tmp % 36];
        tmp /= 36;
        *(out + 2) = gsIntToChar[tmp % 36];
        tmp /= 36;
        *(out + 1) = gsIntToChar[tmp % 36];
        tmp /= 36;
        *out = gsIntToChar[tmp];
        out += 7;
#endif
        // NOTES
        // Version 1 under -O3 is about 10-20 PERCENT faster than version 2
        // BUT Version 1 is 10 TIMES SLOWER when used with -Os !!!
        // Reason: gcc does a lot of tricks to remove the divisions
        //  op with multiplies and shift.
        // In V1 with -O3 this works.  Under -Os it reverts to very
        //   slow division.
        // In V2 -O3 it does the same thing, but under Os, it's smart
        // enough to know we want the quotient and remainder and only
        // one div call per line.
    }
    *out = 0; // final null
    return buckets * 7;
}
