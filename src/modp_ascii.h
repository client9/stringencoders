/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file modp_ascii.h
 * <PRE>
 * MODP_ASCII -- Simple ascii manipulation (uppercase, lowercase, etc)
 * http://code.google.com/p/stringencoders/
 *
 * Copyright &copy; 2007, Nick Galbreath -- nickg [at] modp [dot] com
 * All rights reserved.
 *
 * Released under bsd license.  See modp_ascii.c for details.
 * </PRE>
 *
 */

#ifndef MODP_ASCII
#define MODP_ASCII

#ifdef __cplusplus
extern "C" {
#endif

	void modp_toupper(char* str, int len);

	void modp_toupper_copy(char* dest, const char* str, int len);

	void modp_tolower(char* str, int len);

	void modp_tolower_copy(char* dest, const char* str, int len);

#ifdef __cplusplus
}
#endif

#endif

