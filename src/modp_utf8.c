#include <stdio.h>
/**
 * \file modp_utf8.c
 * <PRE>
 * MODP_UTF8 - UTF8 string utilities
 * https://github.com/client9/stringencoders/
 *
 * Copyright 2013-2016  Nick Galbreath -- nickg [at] client9 [dot] com
 * All rights reserved.
 * Released under MIT license. See LICENSE for details.
 * </PRE>
 */

#include "config.h"
#include "modp_utf8.h"
#include <string.h>

int modp_utf8_validate(const char* src_orig, size_t len)
{
    const uint8_t* src = (const uint8_t*)src_orig;
    const uint8_t* srcend = src + len;
    uint8_t c, c1, c2, c3;
    int d;
    while (src < srcend) {
        c = *src;
        if (c < 0x80) {
            src += 1;
        } else if (c < 0xE0) {
            /* c starts with 110 */
            if (srcend - src < 2) {
                return MODP_UTF8_SHORT;
            }
            c1 = *(src + 1);
            if ((c1 & 0xC0) != 0x80) {
                return MODP_UTF8_INVALID;
            }
            d = ((c & 0x1F) << 6) | (c1 & 0x3F);
            if (d < 0x80) {
                return MODP_UTF8_OVERLONG;
            }
            src += 2;
        } else if (c < 0xF0) {
            if (srcend - src < 3) {
                return MODP_UTF8_SHORT;
            }
            c1 = *(src + 1);
            c2 = *(src + 2);
            if ((c1 & 0xC0) != 0x80) {
                return MODP_UTF8_INVALID;
            }
            if ((c2 & 0xC0) != 0x80) {
                return MODP_UTF8_INVALID;
            }
            d = ((c & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
            if (d < 0x0800) {
                return MODP_UTF8_OVERLONG;
            }
            if (d >= 0xD800 && d <= 0xDFFF) {
                return MODP_UTF8_CODEPOINT;
            }
            src += 3;
        } else if (c < 0xF8) {
            if (srcend - src < 4) {
                return MODP_UTF8_SHORT;
            }
            c1 = *(src + 1);
            c2 = *(src + 2);
            c3 = *(src + 3);
            if ((c1 & 0xC0) != 0x80) {
                return MODP_UTF8_INVALID;
            }
            if ((c2 & 0xC0) != 0x80) {
                return MODP_UTF8_INVALID;
            }
            if ((c3 & 0xC0) != 0x80) {
                return MODP_UTF8_INVALID;
            }
            d = ((c & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) < 6) | (c3 & 0x3F);
            if (d < 0x010000) {
                return MODP_UTF8_OVERLONG;
            }
            src += 4;
        } else {
            return MODP_UTF8_CODEPOINT;
        }
    }
    return MODP_UTF8_OK;
}
