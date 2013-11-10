/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */
#include <stdio.h>
/**
 * \file modp_utf8.c
 * <PRE>
 * MODP_UTF8 - UTF8 string utilities
 * http://code.google.com/p/stringencoders/
 *
 * Copyright 2013  Nick Galbreath -- nickg [at] client9 [dot] com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the modp.com nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This is the standard "new" BSD license:
 * http://www.opensource.org/licenses/bsd-license.php
 * </PRE>
 */

#include <string.h>
#include "config.h"
#include "modp_utf8.h"

int modp_utf8_validate(const char* src_orig, size_t len)
{
    const unsigned char* src = (const unsigned char*) src_orig;
    const unsigned char* srcend = src + len;
    unsigned char c,c1,c2,c3;
    int d;
    while (src < srcend) {
        c = *src;
        if (c < 0x80) {
            src += 1;
        } else if (c < 0xE0) {
            /* c starts with 110 */
            if (srcend - src < 2) { return MODP_UTF8_SHORT; }
            c1 = *(src + 1);
            if ((c1 & 0xC0) != 0x80) { return MODP_UTF8_INVALID; }
            d = ((c & 0x1F) << 6) | (c1 & 0x3F);
            if (d < 0x80) { return MODP_UTF8_OVERLONG; }
            src += 2;
        } else if (c < 0xF0) {
            if (srcend - src < 3) { return MODP_UTF8_SHORT; }
            c1 = *(src+1);
            c2 = *(src+2);
            if ((c1 & 0xC0) != 0x80) { return MODP_UTF8_INVALID; }
            if ((c2 & 0xC0) != 0x80) { return MODP_UTF8_INVALID; }
            d = ((c & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
            if (d < 0x0800) { return MODP_UTF8_OVERLONG; }
            if (d >= 0xD800 && d <= 0xDFFF) { return MODP_UTF8_CODEPOINT; }
            src += 3;
        } else if (c < 0xF8) {
            if (srcend - src < 4) { return MODP_UTF8_SHORT; }
            c1 = *(src+1);
            c2 = *(src+2);
            c3 = *(src+3);
            if ((c1 & 0xC0) != 0x80) { return MODP_UTF8_INVALID; }
            if ((c2 & 0xC0) != 0x80) { return MODP_UTF8_INVALID; }
            if ((c3 & 0xC0) != 0x80) { return MODP_UTF8_INVALID; }
            d = ((c & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) < 6) | (c3 & 0x3F);
            if (d < 0x010000) { return MODP_UTF8_OVERLONG; }
            src += 4;
        } else {
            return MODP_UTF8_CODEPOINT;
        }
    }
    return MODP_UTF8_OK;
}
