/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#ifndef COM_MODP_STRINGENCODERS_MSGPACK
#define COM_MODP_STRINGENCODERS_MSGPACK

#ifdef __cplusplus
#ifndef MODP_C_BEGIN_DECLS
# define MODP_C_BEGIN_DECLS extern "C" {
# define MODP_C_END_DECLS   }
#endif
#else
# define MODP_C_BEGIN_DECLS
# define MODP_C_END_DECLS
#endif

MODP_C_BEGIN_DECLS

#include <string.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  size_t size;
  char* dest;
} modp_msgpk_ctx;

void modp_msgpk_init(modp_msgpk_ctx* ctx, char* dest);
size_t modp_msgpk_end(modp_msgpk_ctx* ctx);
void modp_msgpk_add_bool(modp_msgpk_ctx* ctx, int32_t val);
void modp_msgpk_add_double(modp_msgpk_ctx* ctx, double d);
void modp_msgpk_add_int32(modp_msgpk_ctx* ctx, int32_t val);
void modp_msgpk_add_uint32(modp_msgpk_ctx* ctx, uint32_t val);

void modp_msgpk_add_string(modp_msgpk_ctx* ctx, const char*, size_t);
void modp_msgpk_add_cstring(modp_msgpk_ctx* ctx, const char*);
void modp_msgpk_map_open(modp_msgpk_ctx* ctx, size_t count);

/* Needed in JSON, not  needed in messagepack */
/* inline void modp_msgpk_map_close(modp_msgpk_ctx* ctx) { (void)ctx; } */
#define modp_msgpk_map_close(X) ((void)(X))

void modp_msgpk_ary_open(modp_msgpk_ctx* ctx, size_t count);

/* Needed in JSON, not  needed in messagepack */
/* inline void modp_msgpk_ary_close(modp_msgpk_ctx* ctx) { (void)ctx; } */
#define modp_msgpk_ary_close(X) ((void)X)

MODP_C_END_DECLS

#endif
