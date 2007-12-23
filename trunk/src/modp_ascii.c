/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file modp_ascii.c
 * <PRE>
 * MODP_ASCII - Ascii transformations (upper/lower, etc)
 * http://code.google.com/p/stringencoders/
 *
 * Copyright &copy; 2007  Nick Galbreath -- nickg [at] modp [dot] com
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

#include <stdint.h>
#include "modp_ascii.h"
#include "modp_ascii_data.h"

void modp_toupper_copy(char* dest, const char* str, int len)
{
    int i;
    uint32_t eax, ebx;
    const uint8_t* ustr = (const uint8_t*) str;
    const int leftover = len % 4;
    const int imax = len / 4;
    const uint32_t* s = (const uint32_t*) str;
    uint32_t* d = (uint32_t*) dest;
    for (i = 0; i != imax; ++i) {
        eax = s[i];
        /*
         * This is based on the algorithm by Paul Hsieh
         * http://www.azillionmonkeys.com/qed/asmexample.html
         */
        ebx = (0x7f7f7f7ful & eax) + 0x05050505ul;
        ebx = (0x7f7f7f7ful & ebx) + 0x1a1a1a1aul;
        ebx = ((ebx & ~eax) >> 2)  & 0x20202020ul;
        *d++ = eax - ebx;
    }

    i = imax*4;
    dest = (char*) d;
    switch (leftover) {
    case 3: *dest++ = gsToUpperMap[ustr[i++]];
    case 2: *dest++ = gsToUpperMap[ustr[i++]];
    case 1: *dest++ = gsToUpperMap[ustr[i]];
    case 0: *dest = '\0';
    }
}

void modp_tolower_copy(char* dest, const char* str, int len)
{
    int i;
    uint32_t eax, ebx;
    const uint8_t* ustr = (const uint8_t*) str;
    const int leftover = len % 4;
    const int imax = len / 4;
    const uint32_t* s = (const uint32_t*) str;
    uint32_t* d = (uint32_t*) dest;
    for (i = 0; i != imax; ++i) {
        eax = s[i];
        /*
         * This is based on the algorithm by Paul Hsieh
         * http://www.azillionmonkeys.com/qed/asmexample.html
         */
        ebx = (0x7f7f7f7ful & eax) + 0x25252525ul;
        ebx = (0x7f7f7f7ful & ebx) + 0x1a1a1a1aul;
        ebx = ((ebx & ~eax) >> 2)  & 0x20202020ul;
        *d++ = eax + ebx;
    }

    i = imax*4;
    dest = (char*) d;
    switch (leftover) {
    case 3: *dest++ = gsToLowerMap[ustr[i++]];
    case 2: *dest++ = gsToLowerMap[ustr[i++]];
    case 1: *dest++ = gsToLowerMap[ustr[i]];
    case 0: *dest = '\0';
    }
}

void modp_toupper(char* str, int len)
{
    modp_toupper_copy(str, str, len);
}

void modp_tolower(char* str, int len)
{
    modp_tolower_copy(str,str,len);
}

void modp_toprint_copy(char* dest, const char* str, int len)
{
    int i;
    uint8_t c1,c2,c3,c4;

    const int leftover = len % 4;
    const int imax = len - leftover;
    const uint8_t* s = (const uint8_t*) str;
    for (i = 0; i != imax ; i+=4) {
        /*
         * it's important to make these variables
         * it helps the optimizer to figure out what to do
         */
        c1 = s[i]; c2=s[i+1]; c3=s[i+2]; c4=s[i+3];
        dest[0] = gsToPrintMap[c1];
        dest[1] = gsToPrintMap[c2];
        dest[2] = gsToPrintMap[c3];
        dest[3] = gsToPrintMap[c4];
        dest += 4;
    }

    switch (leftover) {
    case 3: *dest++ = gsToPrintMap[s[i++]];
    case 2: *dest++ = gsToPrintMap[s[i++]];
    case 1: *dest++ = gsToPrintMap[s[i]];
    case 0: *dest = '\0';
    }
}

void modp_toprint(char* str, int len)
{
    modp_toprint_copy(str,str,len);
}
