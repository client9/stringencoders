
#include <stddef.h>
#include "modp_messagepack.h"


void modp_msgpk_init(modp_msgpk_ctx* ctx, char* dest)
{
    memset((void*)ctx, 0, sizeof(modp_msgpk_ctx));
    ctx->dest = dest;
}

size_t modp_msgpk_end(modp_msgpk_ctx* ctx)
{
  return ctx->size;
}

static void modp_msgpk_raw_byte(modp_msgpk_ctx* ctx, int c)
{
  if (ctx->dest) {
    *(ctx->dest + ctx->size) = (char) c;
  }
  ctx->size += 1;
}

static void modp_msgpk_raw_unit16(modp_msgpk_ctx* ctx, size_t val)
{

  uint16_t nval = (uint16_t) val;
  if (ctx->dest) {
    memcpy(ctx->dest + ctx->size, (void*)(&nval), (size_t)2);
  }
  ctx->size += 2;
}

static void modp_msgpk_raw_uint32(modp_msgpk_ctx* ctx, size_t val)
{
  uint32_t nval = (uint32_t) val;
  if (ctx->dest) {
    memcpy(ctx->dest + ctx->size, (void*)(&nval), (size_t)4);
  }
  ctx->size += 4;
}

static void modp_msgpk_raw_bytes(modp_msgpk_ctx* ctx, const void* s, size_t len)
{
  if (ctx->dest) {
    memcpy(ctx->dest + ctx->size, s, len);
  }
  ctx->size += len;
}

void modp_msgpk_add_null(modp_msgpk_ctx* ctx)
{
  modp_msgpk_raw_byte(ctx, 0xC0);
}

void modp_msgpk_add_bool(modp_msgpk_ctx* ctx, int val)
{
  modp_msgpk_raw_byte(ctx, val ?  0xc3 : 0xc2);
}

void modp_msgpk_add_double(modp_msgpk_ctx* ctx, double d)
{
   modp_msgpk_raw_byte(ctx, 0xCB);
   modp_msgpk_raw_bytes(ctx, (const void*)(&d), 8);
}

void modp_msgpk_add_int32(modp_msgpk_ctx* ctx, int val)
{
   modp_msgpk_raw_byte(ctx, 0xD2);
   modp_msgpk_raw_uint32(ctx, (uint32_t)val);
}

void modp_msgpk_add_uint32(modp_msgpk_ctx* ctx, uint32_t val)
{
   modp_msgpk_raw_byte(ctx, 0xCE);
   modp_msgpk_raw_uint32(ctx, val);
}

void modp_msgpk_add_string(modp_msgpk_ctx* ctx, const char* s, size_t len)
{
  if (len < 32) {
    modp_msgpk_raw_byte(ctx, 0xA0 | len);
  } else if (len < 256) {
    modp_msgpk_raw_byte(ctx, 0xD9);
    modp_msgpk_raw_byte(ctx, (uint8_t) len);
  } else if (len < 65535) {
    modp_msgpk_raw_byte(ctx, 0xDA);
    modp_msgpk_raw_unit16(ctx, len);
  } else if (len <= 0xFFFFFFFF) {
    modp_msgpk_raw_byte(ctx, 0xDB);
    modp_msgpk_raw_uint32(ctx, len);
  }
  modp_msgpk_raw_bytes(ctx, (const void*)s, len);
}

void modp_msgpk_add_cstring(modp_msgpk_ctx* ctx, const char* s)
{
  modp_msgpk_add_string(ctx, s, strlen(s));
}

void modp_msgpk_map_open(modp_msgpk_ctx* ctx, size_t count)
{
  if (count < 16) {
    modp_msgpk_raw_byte(ctx, 0x80 | count);
  } else if (count < 0xFFFF) {
    modp_msgpk_raw_byte(ctx, 0xDE);
    modp_msgpk_raw_unit16(ctx, count);
  } else if (count <= 0xFFFFFFFF) {
    modp_msgpk_raw_byte(ctx, 0xDF);
    modp_msgpk_raw_uint32(ctx, count);
  }
  /* DO ASSERT */
}

void modp_msgpk_ary_open(modp_msgpk_ctx* ctx, size_t count)
{
  if (count < 16) {
    modp_msgpk_raw_byte(ctx, 0x90 | count);
  } else if (count < 0xFFFF) {
    modp_msgpk_raw_byte(ctx, 0xDC);
    modp_msgpk_raw_unit16(ctx, count);
  } else if (count <= 0xFFFFFFFF) {
    modp_msgpk_raw_byte(ctx, 0xDD);
    modp_msgpk_raw_uint32(ctx, count);
  }
  /* ASSERT */
}
