/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file modp_b2.c
 * <PRE>
 * MODP_B2 - Ascii Binary string encode/decode
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2005-2016  Nick Galbreath
 * All rights reserved.
 *
 * Released under MIT license. See LICENSE for details.
 *
 */
#include "modp_b2.h"
#include "config.h"
#include "modp_b2_data.h"
#include "modp_stdint.h"
#include <string.h>

size_t modp_b2_encode(char* dest, const char* str, size_t len)
{
    const uint8_t* orig = (const uint8_t*)str;
#if 0
    /* THIS IS A STANDARD VERSION */
    static const uint8_t gsBinaryChars[] = "01";
    int i,j;
    for (i = 0; i < len; ++i) {
        for (j = 0; j <= 7; ++j) {
            *dest++ = gsBinaryChars[(orig[i] >> (7-j)) & 1];
        }
    }
#else
    /* THIS IS 10X FASTER */
    size_t i;
    for (i = 0; i < len; ++i) {
        memcpy((void*)dest, modp_b2_encodemap[orig[i]], (size_t)8);
        dest += 8;
    }
#endif
    *dest = '\0';
    return len * 8;
}

size_t modp_b2_decode(char* dest, const char* str, size_t len)
{
    char d;
    size_t i;
    int j;
    const size_t buckets = len / 8;
    const size_t leftover = len % 8;
    if (leftover != 0) {
        return (size_t)-1;
    }

    for (i = 0; i < buckets; ++i) {
        d = 0;
        for (j = 0; j <= 7; ++j) {
            char c = *str++;
            if (c == '1') {
                d ^= (char)(1 << (7 - j));
            }
        }
        *dest++ = d;
    }

    return buckets;
}
