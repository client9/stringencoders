/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file modp_ascii.c
 * <PRE>
 * MODP_ASCII - Ascii transformations (upper/lower, etc)
 * http://code.google.com/p/stringencoders/
 *
 * Copyright &copy; 2007  Nick Galbreath -- nickg [at] modp [dot] com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the modp.com nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This is the standard "new" BSD license:
 * http://www.opensource.org/licenses/bsd-license.php
 * </PRE>
 */

#include "modp_ascii.h"
void modp_toupper(char* str, int len)
{
	int i = len;
	for (i = 0; i < len; ++i) {
		char c = str[i];
		str[i] = (c >= 'a' && c <= 'z') ? c -= 32 : c;
	}
}

void modp_toupper_copy(char* dest, const char* str, int len)
{
	int i = len;
	for (i = 0; i < len; ++i) {
		char c = str[i];
		*dest++ = (c >= 'a' && c <= 'z') ? c - 32 : c;
	}
}

void modp_tolower(char* str, int len)
{
	int i = len;
	for (i = 0; i < len; ++i) {
		char c = str[i];
		str[i] = (c >= 'A' && c <= 'Z') ? c + 32 : c;
	}
}

void modp_tolower_copy(char* dest, const char* str, int len)
{
	int i = 0;
	for (i = 0; i < len; ++i) {
		char c = str[i];
		*dest++ = (c >= 'A' && c <= 'Z') ? c + 32 : c;
	}
}
