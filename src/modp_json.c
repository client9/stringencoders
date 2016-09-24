/**
 * \file
 * <pre>
 * modp_json.c High performance JSON encoder/decoder
 * https://github.com/client9/stringencoders
 * NOTE THIS IS UNTESTED AND EXPERIMENTAL.
 *
 * Copyright &copy; 2006-2016 Nick Galbreath
 * All rights reserved.
 * Released under MIT license.  See LICENSE for details.
 * </PRE>
 */

#include "modp_json.h"
#include "modp_json_data.h"
#include <assert.h>

typedef enum {
    JSON_NONE,
    JSON_MAP_OPEN,
    JSON_MAP_CLOSE,
    JSON_MAP_KEY,
    JSON_MAP_VAL,
    JSON_ARY_OPEN,
    JSON_ARY_CLOSE,
    JSON_ARY_VAL,
} json_state_t;

static size_t modp_bjson_encode_strlen(const char* src, size_t len);
static size_t modp_bjson_encode(char* dest, const char* src, size_t len);

static void modp_json_add_char(modp_json_ctx* ctx, int c);
static void modp_json_add_value(modp_json_ctx* ctx);
static void modp_json_add_false(modp_json_ctx* ctx);
static void modp_json_add_true(modp_json_ctx* ctx);

static void modp_json_add_char(modp_json_ctx* ctx, int c)
{
    if (ctx->dest) {
        *(ctx->dest + ctx->size) = (char)c;
    }
    ctx->size += 1;
}

static void modp_json_add_value(modp_json_ctx* ctx)
{
    int depth = ctx->depth;

    switch (ctx->state[depth]) {
    case JSON_NONE:
        /* no-op */
        break;
    case JSON_MAP_OPEN:
        /* NO comma */
        ctx->state[depth] = JSON_MAP_KEY;
        break;
    case JSON_ARY_OPEN:
        /* NO comma */
        ctx->state[depth] = JSON_ARY_VAL;
        break;
    case JSON_ARY_VAL:
        modp_json_add_char(ctx, ',');
        break;
    case JSON_MAP_KEY:
        modp_json_add_char(ctx, ':');
        ctx->state[depth] = JSON_MAP_VAL;
        break;
    case JSON_MAP_VAL:
        modp_json_add_char(ctx, ',');
        ctx->state[depth] = JSON_MAP_KEY;
        break;
    }
}

void modp_json_init(modp_json_ctx* ctx, char* dest)
{
    memset((void*)ctx, 0, sizeof(modp_json_ctx));
    ctx->dest = dest;
}

size_t modp_json_end(modp_json_ctx* ctx)
{
    if (ctx->dest) {
        *(ctx->dest + ctx->size) = '\0';
    }
    return ctx->size;
}

void modp_json_map_open(modp_json_ctx* ctx)
{
    modp_json_add_value(ctx);

    ctx->depth++;
    ctx->state[ctx->depth] = JSON_MAP_OPEN;

    modp_json_add_char(ctx, '{');
}

void modp_json_map_close(modp_json_ctx* ctx)
{
    assert(ctx->depth > 0);
    ctx->depth--;
    modp_json_add_char(ctx, '}');
}

void modp_json_ary_open(modp_json_ctx* ctx)
{
    modp_json_add_value(ctx);
    ctx->depth++;
    ctx->state[ctx->depth] = JSON_ARY_OPEN;
    modp_json_add_char(ctx, '[');
}

void modp_json_ary_close(modp_json_ctx* ctx)
{
    assert(ctx->depth > 0);
    ctx->depth--;
    modp_json_add_char(ctx, ']');
}

static void modp_json_add_true(modp_json_ctx* ctx)
{
    char* wstr;
    if (ctx->dest) {
        wstr = ctx->dest + ctx->size;
        wstr[0] = 't';
        wstr[1] = 'r';
        wstr[2] = 'u';
        wstr[3] = 'e';
    }
    ctx->size += 4;
}

static void modp_json_add_false(modp_json_ctx* ctx)
{
    char* wstr;

    if (ctx->dest) {
        wstr = ctx->dest + ctx->size;
        wstr[0] = 'f';
        wstr[1] = 'a';
        wstr[2] = 'l';
        wstr[3] = 's';
        wstr[4] = 'e';
    }
    ctx->size += 5;
}

void modp_json_add_bool(modp_json_ctx* ctx, int val)
{

    modp_json_add_value(ctx);

    if (val) {
        modp_json_add_true(ctx);
    } else {
        modp_json_add_false(ctx);
    }
}

void modp_json_add_null(modp_json_ctx* ctx)
{
    char* wstr;

    modp_json_add_value(ctx);

    if (ctx->dest) {
        wstr = ctx->dest + ctx->size;
        wstr[0] = 'n';
        wstr[1] = 'u';
        wstr[2] = 'l';
        wstr[3] = 'l';
    }
    ctx->size += 4;
}

void modp_json_add_uint64(modp_json_ctx* ctx, uint64_t uv,
    int stringonly)
{
    size_t r = (uv >= 10000000000000000000ULL) ? 20 : (uv >= 1000000000000000000ULL) ? 19 : (uv >= 100000000000000000ULL) ? 18 : (uv >= 10000000000000000ULL) ? 17 : (uv >= 1000000000000000ULL) ? 16 : (uv >= 100000000000000ULL) ? 15 : (uv >= 10000000000000ULL) ? 14 : (uv >= 1000000000000ULL) ? 13 : (uv >= 100000000000ULL) ? 12 : (uv >= 10000000000ULL) ? 11 : (uv >= 1000000000ULL) ? 10 : (uv >= 100000000ULL) ? 9 : (uv >= 10000000ULL) ? 8 : (uv >= 1000000ULL) ? 7 : (uv >= 100000ULL) ? 6 : (uv >= 10000ULL) ? 5 : (uv >= 1000ULL) ? 4 : (uv >= 100ULL) ? 3 : (uv >= 10ULL) ? 2 : 1ULL;

    if (uv > (1ULL << 53)) {
        stringonly = 1;
    }

    modp_json_add_value(ctx);

    if (ctx->dest) {
        char* wstr = ctx->dest + ctx->size;
        if (stringonly) {
            wstr[0] = '"';
            wstr[r + 1] = '"';
            wstr += r;
        } else {
            wstr += r - 1;
        }

        /* Conversion. Number is reversed. */
        do
            *wstr-- = (char)(48 + (uv % 10));
        while (uv /= 10);
    }

    if (stringonly) {
        r += 2;
    }

    ctx->size += r;
}

void modp_json_add_int32(modp_json_ctx* ctx, int v)
{
    char* wstr;
    if (v > 0) {
        return modp_json_add_uint32(ctx, (uint32_t)v);
    }
    uint32_t uv = (uint32_t)(-v);
    size_t r = (uv >= 1000000000) ? 10 : (uv >= 100000000) ? 9 : (uv >= 10000000) ? 8 : (uv >= 1000000) ? 7 : (uv >= 100000) ? 6 : (uv >= 10000) ? 5 : (uv >= 1000) ? 4 : (uv >= 100) ? 3 : (uv >= 10) ? 2 : 1;

    modp_json_add_value(ctx);

    if (ctx->dest) {
        wstr = ctx->dest + ctx->size;
        *wstr = '-';
        wstr += r;
        /* Conversion. Number is reversed. */
        do
            *wstr-- = (char)(48 + (uv % 10));
        while (uv /= 10);
    }

    ctx->size += r + 1; /* +1 for '-' minus sign */
}

void modp_json_add_uint32(modp_json_ctx* ctx, uint32_t uv)
{
    char* wstr;
    size_t r = (uv >= 1000000000UL) ? 10 : (uv >= 100000000UL) ? 9 : (uv >= 10000000UL) ? 8 : (uv >= 1000000UL) ? 7 : (uv >= 100000UL) ? 6 : (uv >= 10000UL) ? 5 : (uv >= 1000UL) ? 4 : (uv >= 100UL) ? 3 : (uv >= 10UL) ? 2 : 1UL;

    modp_json_add_value(ctx);

    if (ctx->dest) {
        wstr = ctx->dest + ctx->size;
        wstr += r - 1;
        do
            *wstr-- = (char)(48 + (uv % 10));
        while (uv /= 10);
    }

    ctx->size += r;
}

void modp_json_add_cstring(modp_json_ctx* ctx, const char* src)
{
    return modp_json_add_string(ctx, src, strlen(src));
}

void modp_json_add_string(modp_json_ctx* ctx, const char* src, size_t len)
{
    modp_json_add_value(ctx);

    if (ctx->dest) {
        ctx->size += modp_bjson_encode(ctx->dest + ctx->size, src, len);
    } else {
        ctx->size += modp_bjson_encode_strlen(src, len);
    }
}

static size_t modp_bjson_encode(char* dest, const char* src, size_t len)
{
    static const char* hexchar = "0123456789ABCDEF";
    const char* deststart = (const char*)dest;
    const uint8_t* s = (const uint8_t*)src;
    const uint8_t* srcend = s + len;
    uint8_t x;
    uint8_t val;

    *dest++ = '"';

    while (s < srcend) {
        x = *s++;
        val = gsJSONEncodeMap[x];
        if (val == 'a') {
            /* a for ascii, as-is */
            *dest++ = (char)x;
        } else if (val == 'u') {
            /* u for unicode, 6 byte escape sequence */
            dest[0] = '\\';
            dest[1] = 'u';
            dest[2] = '0';
            dest[3] = '0';
            dest[4] = hexchar[x >> 4];
            dest[5] = hexchar[x & 0x0F];
            dest += 6;
        } else {
            /* 2 byte escape sequence */
            dest[0] = '\\';
            dest[1] = (char)val;
            dest += 2;
        }
    }
    *dest++ = '"';
    return (size_t)(dest - deststart);
}

static size_t modp_bjson_encode_strlen(const char* src, size_t len)
{
    const uint8_t* s = (const uint8_t*)src;
    const uint8_t* srcend = s + len;
    size_t count = 2; /* for start and end quotes */
    while (s < srcend) {
        count += (gsJSONEncodeLenMap[*s++]);
    }
    return count;
}
