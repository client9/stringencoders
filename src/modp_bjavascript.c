/**
 * \file
 * <pre>
 * modp_bjavascript.c High performance URL encoder/decoder
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2006-2016  Nick Galbreath
 * All rights reserved.
 * Released under MIT license. See LICENSE for details.
 * </PRE>
 */
#include "modp_bjavascript.h"
#include "modp_bjavascript_data.h"
#include "modp_stdint.h"

size_t modp_bjavascript_encode(char* dest, const char* src, size_t len)
{
    const char* deststart = dest;
    const uint8_t* s = (const uint8_t*)src;
    const uint8_t* srcend = s + len;
    uint8_t x;
    uint8_t val;

    /* if 0, do nothing
     * if 'A', hex escape
     * else, \\ + value
     */
    while (s < srcend) {
        x = *s++;
        val = gsJavascriptEncodeMap[x];
        if (val == 0) {
            *dest++ = (char)x;
        } else if (val == 'A') {
            *dest++ = '\\';
            *dest++ = 'x';
            *dest++ = (char)(gsHexEncodeMap1[x]);
            *dest++ = (char)(gsHexEncodeMap2[x]);
        } else {
            *dest++ = '\\';
            *dest++ = (char)val;
        }
    }
    *dest = '\0';
    return (size_t)(dest - deststart);
}

size_t modp_bjavascript_encode_strlen(const char* src, size_t len)
{
    const uint8_t* s = (const uint8_t*)src;
    const uint8_t* srcend = s + len;
    size_t count = 0;
    uint8_t val;

    while (s < srcend) {
        val = gsJavascriptEncodeMap[*s++];
        if (val == 0) {
            count++;
        } else if (val == 'A') {
            count += 4;
        } else {
            count += 2;
        }
    }
    return count;
}
