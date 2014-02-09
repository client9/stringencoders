/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file modp_bjson.h
 * \brief "C-string" to "json-string" encoder
 *
 * Used in emitting dynamically-generated json.  Given a regular
 * C-string which might contain binary, the encoder will emit a string
 * that can be used inside a JSON object.
 *
 *
 * The "b" in "modp_bjson is due to legacy reasons.  It doesn't
 * mean anything
 *
 * There is no decoder.
 */

/*
 * <PRE>
 * High Performance c-string to javascript-string encoder
 *
 * Copyright &copy; 2014 Nick Galbreath -- nickg [at] client9 [dot] com
 * All rights reserved.
 *
 * http://code.google.com/p/stringencoders/
 *
 * Released under bsd license.  See modp_bjson.c for details.
 * </PRE>
 */

#ifndef COM_MODP_STRINGENCODERS_BJSON
#define COM_MODP_STRINGENCODERS_BJSON

#include "modp_stdint.h"

#include "extern_c_begin.h"

typedef struct {
    int depth;
    int state[10];
    size_t size;
    char* dest;
} modp_json_ctx;

void modp_json_init(modp_json_ctx* ctx, char* dest);
size_t modp_json_end(modp_json_ctx* ctx);
void modp_json_map_open(modp_json_ctx* ctx);
void modp_json_map_close(modp_json_ctx* ctx);
void modp_json_ary_open(modp_json_ctx* ctx);
void modp_json_ary_close(modp_json_ctx* ctx);

void modp_json_add_string(modp_json_ctx* ctx, const char*, size_t);
void modp_json_add_bool(modp_json_ctx* ctx, int val);
void modp_json_add_int(modp_json_ctx* ctx, int val);
void modp_json_add_double(modp_json_ctx* ctx, double d);
void modp_json_add_null(modp_json_ctx* ctx);

#include "extern_c_end.h"

#endif /* modp_bjson */
