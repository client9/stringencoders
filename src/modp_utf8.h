/**
 * \file modp_utf.h
 * \brief Various UTF8 utilities
 *
 */

/*
 * <PRE>
 * MODP_UTF8 -- UTF-8 utilities
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2013-2016, Nick Galbreath
 * All rights reserved.
 *
 * Released under MIT license.  See LICENSE for details.
 * </PRE>
 *
 */

#ifndef COM_MODP_UTF8
#define COM_MODP_UTF8

#include "extern_c_begin.h"
#include "modp_stdint.h"

#define MODP_UTF8_OK 0
#define MODP_UTF8_SHORT 1
#define MODP_UTF8_INVALID 2
#define MODP_UTF8_OVERLONG 3
#define MODP_UTF8_CODEPOINT 4
/**
 * Validate a UTF-8 string.
 *   checks for blah blah blah
 */
int modp_utf8_validate(const char* src, size_t len);

#include "extern_c_end.h"
#endif /* ifndef modp_utf8 */
