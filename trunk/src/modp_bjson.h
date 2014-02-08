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



/**
 * "json" encode a stirng
 * This takes a c-string and does character escaping
 * so it can be put into a JSON object
 *
 * \param[out] dest output string.  Must
 * \param[in] str The input string
 * \param[in] len  The length of the input string, excluding any
 *   final null byte.
 */
size_t modp_bjson_encode(char* dest, const char* str, size_t len);

#define modp_bjson_encode_len(A) (4*A + 1)

/**
 * Given the exact size of output string.
 *
 * Can be used to allocate the right amount of memory for
 * modp_burl_encode.  Be sure to add 1 byte for final null.
 *
 * This is somewhat expensive since it examines every character
 *  in the input string
 *
 * \param[in] str  The input string
 * \param[in] len  THe length of the input string, excluding any
 *   final null byte (i.e. strlen(str))
 * \return the size of the output string, excluding the final
 *   null byte.
 */
size_t modp_bjson_encode_strlen(const char* str, size_t len);

#include "extern_c_end.h"

#endif /* modp_bjson */
