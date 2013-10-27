/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file modp_b2.h
 * \brief Encode and decode of base 2 strings ("00001110" to 0x43)
 *
 */

/*
 * <PRE>
 * MODP_UTF8 -- UTF-8 utilities
 * http://code.google.com/p/stringencoders/
 *
 * Copyright &copy; 2013, Nick Galbreath -- nickg [at] client9 [dot] com
 * All rights reserved.
 *
 * Released under bsd license.  See modp_b2.c for details.
 * </PRE>
 *
 */

#ifndef COM_MODP_UTF8
#define COM_MODP_UTF8

#include "modp_stdint.h"
#include "extern_c_begin.h"

#define MODP_UTF8_OK  0
#define MODP_UTF8_SHORT  1
#define MODP_UTF8_INVALID  2
#define MODP_UTF8_OVERLONG  3
#define MODP_UTF8_CODEPOINT  4
/**
 * Validate a UTF-8 string.
 *   checks for blah blah blah
 */
int modp_utf8_validate(const char* src, size_t len);

#include "extern_c_end.h"
#endif  /* ifndef modp_utf8 */
