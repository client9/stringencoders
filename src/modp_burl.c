/*
 * <pre>
 * BFASTURL.c High performance URL encoder/decoder
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2006-2016  Nick Galbreath
 * All rights reserved.
 * Released under MIT license. See LICENSE for details.
 * </PRE>
 */

#include "modp_stdint.h"

#include "modp_burl.h"
#include "modp_burl_data.h"

size_t modp_burl_encode(char* dest, const char* src, size_t len)
{

    const char* deststart = dest;
    const uint8_t* s = (const uint8_t*)src;
    const uint8_t* srcend = s + len;
    char c;
    uint8_t x;

    if (len == 0) {
        *dest = '\0';
        return (size_t)0;
    }

    while (s < srcend) {
        x = *s++;
        c = (char)gsUrlEncodeMap[x];
        if (c) {
            *dest++ = c;
        } else {
            *dest++ = '%';
            *dest++ = (char)gsHexEncodeMap1[x];
            *dest++ = (char)gsHexEncodeMap2[x];
            /*
              is the equiv of this
              static const char sHexChars[] = "0123456789ABCDEF";
              *dest++ = (char)sHexChars[x >> 4];
              *dest++ = (char)sHexChars[x & 0x0F];
              */
        }
    }
    *dest = '\0';
    return (size_t)(dest - deststart); /* compute "strlen" of dest. */
}

/**
 * The implementation is identical except it uses a
 * different array
 */
size_t modp_burl_min_encode(char* dest, const char* src, size_t len)
{

    const char* deststart = dest;
    const uint8_t* s = (const uint8_t*)src;
    const uint8_t* srcend = s + len;
    char c;
    uint8_t x;

    if (len == 0) {
        *dest = '\0';
        return (size_t)0;
    }

    while (s < srcend) {
        x = *s++;
        c = (char)(gsUrlEncodeMinMap[x]); /** CHANGE HERE **/
        if (c) {
            *dest++ = c;
        } else {
            *dest++ = '%';
            *dest++ = (char)gsHexEncodeMap1[x];
            *dest++ = (char)(gsHexEncodeMap2[x]);
            /*
              is the equiv of this
              static const char sHexChars[] = "0123456789ABCDEF";
              *dest++ = sHexChars[x >> 4];
              *dest++ = sHexChars[x & 0x0F];
              */
        }
    }
    *dest = '\0';
    return (size_t)(dest - deststart); /* compute "strlen" of dest. */
}

/**
 * Give exact size of encoded output string
 * without doing the encoding
 */
size_t modp_burl_encode_strlen(const char* src, const size_t len)
{
    size_t count = 0;
    const char* srcend = src + len;

    if (len == 0) {
        return (size_t)0;
    }

    while (src < srcend) {
        if (gsUrlEncodeMap[(uint8_t)*src++]) {
            count++;
        } else {
            count += 3;
        }
    }
    return count;
}

/**
 * Give exact size of encoded output string
 * without doing the encoding
 */
size_t modp_burl_min_encode_strlen(const char* src, const size_t len)
{
    size_t count = 0;
    const char* srcend = src + len;

    if (len == 0) {
        return (size_t)0;
    }

    while (src < srcend) {
        if (gsUrlEncodeMinMap[(uint8_t)*src++]) {
            count++;
        } else {
            count += 3;
        }
    }
    return count;
}

size_t modp_burl_decode(char* dest, const char* s, size_t len)
{
    uint32_t d = 0; /* used for decoding %XX */
    const uint8_t* src = (const uint8_t*)s;
    const char* deststart = dest;
    const uint8_t* srcend = (const uint8_t*)(src + len);
    const uint8_t* srcendloop = (const uint8_t*)(srcend - 2);

    if (len == 0) {
        *dest = '\0';
        return (size_t)0;
    }

    while (src < srcendloop) {
        switch (*src) {
        case '+':
            *dest++ = ' ';
            src++;
            break;
        case '%':
            d = (gsHexDecodeMap[(uint32_t)(*(src + 1))] << 4) | gsHexDecodeMap[(uint32_t)(*(src + 2))];
            if (d < 256) { /* if one of the hex chars is bad,  d >= 256 */
                *dest = (char)d;
                dest++;
                src += 3;
            } else {
                *dest++ = '%';
                src++;
            }
            break;
        default:
            *dest++ = (char)*src++;
        }
    }

    /* handle last two chars
     * dont decode "%XX"
     */
    while (src < srcend) {
        switch (*src) {
        case '+':
            *dest++ = ' ';
            src++;
            break;
        default:
            *dest++ = (char)(*src++);
        }
    }

    *dest = '\0';
    return (size_t)(dest - deststart); /* compute "strlen" of dest. */
}

size_t modp_burl_decode_raw(char* dest, const char* s, size_t len)
{
    uint32_t d = 0; /* used for decoding %XX */
    const uint8_t* src = (const uint8_t*)s;
    const char* deststart = dest;
    const uint8_t* srcend = (const uint8_t*)(src + len);
    const uint8_t* srcendloop = (const uint8_t*)(srcend - 2);

    if (len == 0) {
        *dest = '\0';
        return (size_t)0;
    }

    while (src < srcendloop) {
        if (*src == '%') {
            d = (gsHexDecodeMap[(uint32_t)(*(src + 1))] << 4) | gsHexDecodeMap[(uint32_t)(*(src + 2))];
            if (d < 256) { /* if one of the hex chars is bad,  d >= 256 */
                *dest = (char)d;
                dest++;
                src += 3;
            } else {
                *dest++ = '%';
                src++;
            }
        } else {
            *dest++ = (char)*src++;
        }
    }

    /* handle last two chars
     * dont decode "%XX"
     */
    while (src < srcend) {
        *dest++ = (char)(*src++);
    }

    *dest = '\0';
    return (size_t)(dest - deststart); /* compute "strlen" of dest. */
}
