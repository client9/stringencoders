/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#ifndef MODP_BFAST_ARRAYTOC
#define MODP_BFAST_ARRAYTOC

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void uint32_array_to_c(const uint32_t* ary, int size, const char* name);

void uint32_array_to_c_hex(const uint32_t* ary, int size, const char* name);

void char_array_to_c(const char* ary, int size, const char* name);

#ifdef __cplusplus
}
#endif

#endif
