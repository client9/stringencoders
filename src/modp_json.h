/**
 * \file modp_json.h
 * \brief ad-hoc json generator
 *
 * Used to serialize data structures.
 *
 * There is no decoder.
 */

/*
 * <PRE>
 * High Performance c-string to javascript-string encoder
 *
 * Copyright &copy; 2014-2016 Nick Galbreath
 * All rights reserved.
 *
 * https://github.com/client9/stringencoders
 *
 * Released under MIT license.  See LICENSE for details.
 * </PRE>
 */

#ifndef COM_MODP_STRINGENCODERS_BJSON
#define COM_MODP_STRINGENCODERS_BJSON

#ifdef __cplusplus
#ifndef MODP_C_BEGIN_DECLS
#define MODP_C_BEGIN_DECLS extern "C" {
#define MODP_C_END_DECLS }
#endif
#else
#define MODP_C_BEGIN_DECLS
#define MODP_C_END_DECLS
#endif

MODP_C_BEGIN_DECLS

#ifndef JSON_MAX_DEPTH
#define JSON_MAX_DEPTH 10
#endif

/* pull in size_t */
#include <stdint.h>
#include <string.h>

typedef struct {
    int depth;
    int state[JSON_MAX_DEPTH];
    size_t size;
    char* dest;
} modp_json_ctx;

void modp_json_init(modp_json_ctx* ctx, char* dest);
size_t modp_json_end(modp_json_ctx* ctx);
void modp_json_map_open(modp_json_ctx* ctx);
void modp_json_map_close(modp_json_ctx* ctx);
void modp_json_ary_open(modp_json_ctx* ctx);
void modp_json_ary_close(modp_json_ctx* ctx);

/*
 *
 * Note: to add a 'null string' explicitly use 'add_null' and do NOT
 * pass in a null pointer here.
 */
void modp_json_add_string(modp_json_ctx* ctx, const char*, size_t);

void modp_json_add_cstring(modp_json_ctx* ctx, const char*);

/*
 * Sets a json 'false' value if val = 0, other wise sets a 'true' value
 */
void modp_json_add_bool(modp_json_ctx* ctx, int val);

void modp_json_add_double(modp_json_ctx* ctx, double d);

void modp_json_add_int32(modp_json_ctx* ctx, int val);

void modp_json_add_uint32(modp_json_ctx* ctx, uint32_t val);

/**
 * JSON/Javascript only supports a single number type represented by a
 * 64-bit IEEE floating-point value.  This means JSON can not exactly
 * represent a full 64-bit integer since the mantissa of a double is
 * only 53 bits.  If the input integer needs more than 53 bits then
 * the value is stored as string "1234567899329342" instead of a
 * raw/naked numeric value.  If you wish to anyways represent 64-bit
 * integers as a string, use stringonly=1.  If you wish to never
 * casting to a string, use add_uint32 or add_int32 which can be
 * expressed directly or consider loosing precession and cast to a
 * double value.
 *
 */
void modp_json_add_uint64(modp_json_ctx* ctx, uint64_t val,
    int stringonly);

/*
 * explicitly add a null type
 */
void modp_json_add_null(modp_json_ctx* ctx);

MODP_C_END_DECLS

#endif /* modp_bjson */
