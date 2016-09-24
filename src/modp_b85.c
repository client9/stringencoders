/**
 * \file
 * <PRE>
 * MODP_B85 - High performance base85 encoder/decoder
 * https://github.com/client9/stringencoders/
 *
 * Copyright &copy; 2006-2016  Nick Galbreath -- nickg [at] client9 [dot] com
 * All rights reserved.
 * Released under MIT license. See LICENSE for details.
 * </pre>
 */

/* exported public header */
#include "modp_b85.h"
#include "modp_stdint.h"
/* private header */
#include "modp_b85_data.h"

/*
 * Might need changing depending on platform
 * we need htonl, and ntohl
 */
#include <arpa/inet.h>

/**
 * you can decode IN PLACE!
 * no memory allocated
 */
size_t modp_b85_decode(char* out, const char* data, size_t len)
{
    size_t i;
    int j;
    uint32_t tmp;
    uint32_t digit;
    uint32_t* o2;
    const size_t buckets = len / 5;
    const uint8_t* d2 = (const uint8_t*)data;

    if (len % 5 != 0) {
        return (size_t)-1;
    }

    o2 = (uint32_t*)out;
    for (i = 0; i < buckets; ++i) {
        tmp = 0;
        for (j = 0; j < 5; ++j) {
            digit = gsCharToInt[(uint32_t)*d2++];
            if (digit >= 85) {
                return (size_t)-1;
            }
            tmp = tmp * 85 + digit;
        }
        *o2++ = htonl(tmp);
    }
    return 4 * buckets;
}

/**
 * src != out
 */
size_t modp_b85_encode(char* out, const char* src, size_t len)
{
    size_t i;
    uint32_t tmp;
    const uint32_t* sary = (const uint32_t*)src;
    const size_t buckets = len / 4;
    if (len % 4 != 0) {
        return (size_t)-1;
    }

    for (i = 0; i < buckets; ++i) {
        tmp = *sary++;
        tmp = htonl(tmp);

/* this crazy function */
#if 1
        *out++ = (char)gsIntToChar[(tmp / 52200625)]; /* don't need % 85 here, always < 85 */
        *out++ = (char)gsIntToChar[(tmp / 614125) % 85];
        *out++ = (char)gsIntToChar[(tmp / 7225) % 85];
        *out++ = (char)gsIntToChar[(tmp / 85) % 85];
        *out++ = (char)gsIntToChar[tmp % 85];
#else
        /* is really this */
        *(out + 4) = gsIntToChar[tmp % 85];
        tmp /= 85;
        *(out + 3) = gsIntToChar[tmp % 85];
        tmp /= 85;
        *(out + 2) = gsIntToChar[tmp % 85];
        tmp /= 85;
        *(out + 1) = gsIntToChar[tmp % 85];
        tmp /= 85;
        *out = gsIntToChar[tmp];
        out += 5;
#endif
        /* NOTES
         * Version 1 under -O3 is about 10-20 PERCENT faster than version 2
         * BUT Version 1 is 10 TIMES SLOWER when used with -Os !!!
         * Reason: gcc does a lot of tricks to remove the divisions
         *  op with multiplies and shift.
         * In V1 with -O3 this works.  Under -Os it reverts to very
         *   slow division.
         * In V2 -O3 it does the same thing, but under Os, it's smart
         * enough to know we want the quotient and remainder and only
         * one div call per line.
         */
    }
    *out = 0; /* final null */
    return buckets * 5;
}
