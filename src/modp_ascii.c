/*
 * modp_ascii.c
 * <PRE>
 * MODP_ASCII - Ascii transformations (upper/lower, etc)
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2007-16  Nick Galbreath -- nickg [at] client9 [dot] com
 * MIT LICENSE
 *
 * </PRE>
 */

#include "modp_ascii.h"
#include "modp_ascii_data.h"
#include "modp_stdint.h"

void modp_toupper_copy(char* dest, const char* str, size_t len)
{
    size_t i;
    uint32_t eax, ebx;
    const uint8_t* ustr = (const uint8_t*)str;
    const size_t leftover = len % 4;
    const size_t imax = len / 4;
    const uint32_t* s = (const uint32_t*)str;
    uint32_t* d = (uint32_t*)dest;
    for (i = 0; i != imax; ++i) {
        eax = s[i];
        /*
         * This is based on the algorithm by Paul Hsieh
         * http://www.azillionmonkeys.com/qed/asmexample.html
         */
        ebx = (0x7f7f7f7fu & eax) + 0x05050505u;
        ebx = (0x7f7f7f7fu & ebx) + 0x1a1a1a1au;
        ebx = ((ebx & ~eax) >> 2) & 0x20202020u;
        *d++ = eax - ebx;
    }

    i = imax * 4;
    dest = (char*)d;
    switch (leftover) {
    case 3:
        *dest++ = (char)gsToUpperMap[ustr[i++]];
        /* fall through */
    case 2:
        *dest++ = (char)gsToUpperMap[ustr[i++]];
        /* fall through */
    case 1:
        *dest++ = (char)gsToUpperMap[ustr[i]];
        /* fall through */
    case 0:
        *dest = '\0';
    }
}

void modp_tolower_copy(char* dest, const char* str, size_t len)
{
    size_t i;
    uint32_t eax, ebx;
    const uint8_t* ustr = (const uint8_t*)str;
    const size_t leftover = len % 4;
    const size_t imax = len / 4;
    const uint32_t* s = (const uint32_t*)str;
    uint32_t* d = (uint32_t*)dest;
    for (i = 0; i != imax; ++i) {
        eax = s[i];
        /*
         * This is based on the algorithm by Paul Hsieh
         * http://www.azillionmonkeys.com/qed/asmexample.html
         */
        ebx = (0x7f7f7f7fu & eax) + 0x25252525u;
        ebx = (0x7f7f7f7fu & ebx) + 0x1a1a1a1au;
        ebx = ((ebx & ~eax) >> 2) & 0x20202020u;
        *d++ = eax + ebx;
    }

    i = imax * 4;
    dest = (char*)d;
    switch (leftover) {
    case 3:
        *dest++ = (char)gsToLowerMap[ustr[i++]];
        /* fall through */
    case 2:
        *dest++ = (char)gsToLowerMap[ustr[i++]];
        /* fall through */
    case 1:
        *dest++ = (char)gsToLowerMap[ustr[i]];
        /* fall through */
    case 0:
        *dest = '\0';
    }
}

void modp_toupper(char* str, size_t len)
{
    modp_toupper_copy(str, str, len);
}

void modp_tolower(char* str, size_t len)
{
    modp_tolower_copy(str, str, len);
}

void modp_toprint_copy(char* dest, const char* str, size_t len)
{
    size_t i;
    uint8_t c1, c2, c3, c4;

    const size_t leftover = len % 4;
    const size_t imax = len - leftover;
    const uint8_t* s = (const uint8_t*)str;
    for (i = 0; i != imax; i += 4) {
        /*
         * it's important to make these variables
         * it helps the optimizer to figure out what to do
         */
        c1 = s[i];
        c2 = s[i + 1];
        c3 = s[i + 2];
        c4 = s[i + 3];
        dest[0] = (char)gsToPrintMap[c1];
        dest[1] = (char)gsToPrintMap[c2];
        dest[2] = (char)gsToPrintMap[c3];
        dest[3] = (char)gsToPrintMap[c4];
        dest += 4;
    }

    switch (leftover) {
    case 3:
        *dest++ = (char)gsToPrintMap[s[i++]];
        /* fall through */
    case 2:
        *dest++ = (char)gsToPrintMap[s[i++]];
        /* fall through */
    case 1:
        *dest++ = (char)gsToPrintMap[s[i]];
        /* fall through */
    case 0:
        *dest = '\0';
    }
}

void modp_toprint(char* str, size_t len)
{
    modp_toprint_copy(str, str, len);
}

size_t modp_rtrim(char* str, size_t len)
{
    while (len) {
        char c = str[len - 1];
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            str[len - 1] = '\0';
            len -= 1;
        } else {
            break;
        }
    }
    return len;
}
