/*
 * <pre>
 * modp_html xml decoders
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2013-2016  Nick Galbreath
 * All rights reserved.
 * Released under MIT license. See LICENSE for details.
 * </PRE>
 */
#include "modp_html.h"
#include "modp_html_named_entities.h"

static const int gsHexDecodeMap[256] = {
    256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 256, 256,
    256, 256, 256, 256, 256, 10, 11, 12, 13, 14, 15, 256,
    256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
    256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
    256, 10, 11, 12, 13, 14, 15, 256, 256, 256, 256, 256,
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
        return (uint8_t)(*src);
    }

    int val = 0;
    size_t i;
    int ch;
    if (*(src + 1) == '#') {
        val = 0;
        if (*(src + 2) == 'x' || *(src + 2) == 'X') {
            i = 3;
            ch = (uint8_t)(*(src + 3));
            ch = gsHexDecodeMap[ch];
            if (ch == 256) {
                /* degenerate case  '&#[?]' */
                return '&';
            }
            val = ch;
            i = 4;
            while (i < len) {
                ch = (uint8_t)src[i];
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
            ch = (uint8_t)src[i];
            if (ch < '0' || ch > '9') {
                return '&';
            }
            val = ch - '0';
            i += 1;
            while (i < len) {
                ch = (uint8_t)src[i];
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
