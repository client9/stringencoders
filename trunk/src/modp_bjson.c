/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <stdio.h>
/**
 * \file
 * <pre>
 * modp_bjavascript.c High performance URL encoder/decoder
 * http://code.google.com/p/stringencoders/
 *
 * Copyright &copy; 2006, 2007  Nick Galbreath -- nickg [at] modp [dot] com
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

#include <assert.h>

#include "modp_bjson.h"
#include "modp_stdint.h"
#include "modp_bjson_data.h"

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

static void modp_json_add_char(modp_json_ctx* ctx, char c);
static void modp_json_add_value(modp_json_ctx* ctx);
static void modp_json_add_false(modp_json_ctx* ctx);
static void modp_json_add_true(modp_json_ctx* ctx);

static void modp_json_add_char(modp_json_ctx* ctx, char c)
{
    if (ctx->dest) {
        *(ctx->dest + ctx->size) = c;
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
    if (ctx->dest) {
        ctx->dest[0] = 't';
        ctx->dest[1] = 'r';
        ctx->dest[2] = 'u';
        ctx->dest[3] = 'e';
    }
    ctx->size += 4;
}

static void modp_json_add_false(modp_json_ctx* ctx)
{
    if (ctx->dest) {
        ctx->dest[0] = 'f';
        ctx->dest[1] = 'a';
        ctx->dest[2] = 'l';
        ctx->dest[3] = 's';
        ctx->dest[4] = 'e';
    }
    ctx->size += 5;
}

void modp_json_add_bool(modp_json_ctx* ctx, int val)
{
    if (val) {
        modp_json_add_true(ctx);
    } else {
        modp_json_add_false(ctx);
    }
}


void modp_json_add_null(modp_json_ctx* ctx)
{
    if (ctx->dest) {
        ctx->dest[0] = 'n';
        ctx->dest[1] = 'u';
        ctx->dest[2] = 'l';
        ctx->dest[3] = 'l';
    }
    ctx->size += 4;
}

void modp_json_add_int(modp_json_ctx* ctx, int v)
{
    unsigned int uv = (v < 0) ? (unsigned int)(-v) : (unsigned int)(v);
    size_t r =
       (uv >= 1000000000) ? 10 : (uv >= 100000000) ? 9 : (uv >= 10000000) ? 8 : 
       (uv >= 1000000) ? 7 : (uv >= 100000) ? 6 : (uv >= 10000) ? 5 : 
       (uv >= 1000) ? 4 : (uv >= 100) ? 3 : (uv >= 10) ? 2 : 1;

    modp_json_add_value(ctx);

    if (ctx->dest) {
        char* wstr = ctx->dest;
        if (v < 0) {
            *wstr = '-';
            r += 1;
        }
        wstr += r -1;


        // Conversion. Number is reversed.
        do *wstr-- = (char)(48 + (uv % 10)); while (uv /= 10);
    }

    ctx->size += r;
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
    const char* deststart = (const char*) dest;
    const uint8_t* s = (const uint8_t*) src;
    const uint8_t* srcend = s + len;
    uint8_t x;
    uint8_t val;

    // if 0, do nothing
    // if 'A', hex escape
    // else, \\ + value
    *dest++ = '"';

    while (s < srcend) {
        x = *s++;
        val = gsJSONEncodeMap[x];
        if (val == 0) {
            *dest++ = (char) x;
        } else if (val == 'A') {
            dest[0] = '\\';
            dest[1] = 'u';
            dest[2] = '0';
            dest[3] = '0';
            dest[4] = hexchar[x >> 4];
            dest[6] = hexchar[x & 0x0F];
            dest += 7;
        } else {
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
    size_t count = 2;  /* for start and end quotes */

    while (s < srcend) {
        switch (gsJSONEncodeMap[*s++]) {
        case 0:
            count++;
            break;
        case 'A':
            count += 7;
            break;
        default:
            count += 2;
        }
    }
    return count;
}
