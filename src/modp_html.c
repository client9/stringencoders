/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/*
 * <pre>
 * modp_html xml decoders
 * http://code.google.com/p/stringencoders/
 *
 * Copyright &copy; 2013  Nick Galbreath -- nickg [at] client9 [dot] com
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
#include "modp_html.h"
#include "modp_html_named_entities.h"

static const int gsHexDecodeMap[256] = {
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
  0,   1,   2,   3,   4,   5,   6,   7,   8,   9, 256, 256,
256, 256, 256, 256, 256,  10,  11,  12,  13,  14,  15, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256,  10,  11,  12,  13,  14,  15, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
256, 256, 256, 256
};

int modp_html_decode_char_at(const char* src, size_t len, size_t* consumed)
{
    if (len == 0 || src == NULL) {
        *consumed = 0;
        return -1;
    }

    *consumed = 1;
    if (*src != '&' || len < 2) {
        return (unsigned char)(*src);
    }

    int val = 0;
    size_t i;
    int ch;
    if (*(src+1) == '#') {
        val = 0;
        if (*(src+2) == 'x' || *(src+2) == 'X') {
            i = 3;
            ch = (unsigned char) (*(src+3));
            ch = gsHexDecodeMap[ch];
            if (ch == 256) {
                // degenerate case  '&#[?]'
                return '&';
            }
            val = ch;
            i = 4;
            while (i < len) {
                ch = (unsigned char) src[i];
                if (ch == ';') {
                    *consumed = i + 1;
                    return val;
                }
                ch = gsHexDecodeMap[ch];
                if (ch == 256) {
                    *consumed = i;
                    return val;
                }
                val = (val * 16) + ch;
                if (val > 0x1000FF) {
                    return '&';
                }
                ++i;
            }
            *consumed = i;
            return val;
        } else {
            i = 2;
            ch = (unsigned char) src[i];
            if (ch < '0' || ch > '9') {
                return '&';
            }
            val = ch - '0';
            i += 1;
            while (i < len) {
                ch = (unsigned char) src[i];
                if (ch == ';') {
                    *consumed = i + 1;
                    return val;
                }
                if (ch < '0' || ch > '9') {
                    *consumed = i;
                    return val;
                }
                val = (val * 10) + (ch - '0');
                if (val > 0x1000FF) {
                    return '&';
                }
                ++i;
            }
            *consumed = i;
            return val;
        }
    } else {
        /* case &[^#] -- check for named entity */
        /* named entities are surprisingly complicated
         * * case sensitive
         * * may or maybe end in ";"
         * * longest match
         */
        int codepoint = html_named_entity_decode(src + 1, len - 1, consumed);
        if (codepoint == 0) {
            *consumed = 1;
            return '&';
        } else {
            *consumed += 1;
            return codepoint;
        }
    }
}
