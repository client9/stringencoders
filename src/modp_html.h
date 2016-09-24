/**
 * \file modp_html.h
 * \brief Experimental HTML decoder
 *
 * This is mostly experimental.
 */

/*
 * <PRE>
 * High Performance HTML Decoder (for now)
 *
 * Copyright &copy; 2013-2016 Nick Galbreath
 * All rights reserved.
 *
 * https://github.com/client9/stringencoders
 *
 * Released under MIT license.  See LICENSE for details.
 * </PRE>
 */

#ifndef COM_MODP_STRINGENCODERS_HTML
#define COM_MODP_STRINGENCODERS_HTML

#include "modp_stdint.h"

#ifdef __cplusplus
#define BEGIN_C extern "C" {
#define END_C }
#else
#define BEGIN_C
#define END_C
#endif

BEGIN_C

/**
 * \brief HTML decode a single character
 *
 * \param[in] str The input string
 * \param[in] len  The length of the input string, excluding any
 *   final null byte.
 * \param[out] Number of characters consumed
 * \return unicode character value or -1 if error
 *
 * Decode numerical entities (decimal or hexadecimal),
 *   and all named characters
 *
 */
int modp_html_decode_char_at(const char* str, size_t len, size_t* consumed);

END_C

#endif
