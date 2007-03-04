/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file
 * <PRE>
 * High Performance c-string to javascript-string encoder
 * Version 1.0 -- 01-Sep-2006
 *
 * Copyright &copy; 2006, Nick Galbreath -- nickg [at] modp [dot] com
 * All rights reserved.
 *
 * http://modp.com/release/base64
 *
 * Released under bsd license.  See bfast64.c for details.
 * </PRE>
 */

#ifndef MODP_BJAVASCRIPT
#define MODP_BJAVASCRIPT

#ifdef __cplusplus
extern "C" {
#endif

/**
 * "javascript" encode a stirng
 * This takes a c-string and does character escaping
 * so it can be put into a var js_string = '...';
 *
 * \param[out] dest output string.  Must
 * \param[in] str The input string
 * \param[in] len  The length of the input string, excluding any
 *   final null byte.
 */
int modp_bjavascript_encode(char* dest, const char* str, int len);

#define modp_bjavascript_encode_len(A) (4*A + 1)

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
int modp_bjavascript_encode_strlen(const char* str, const int len);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>
namespace modp {

	inline std::string& javascript_encode(std::string& s)
	{
		std::string x(modp_bjavascript_encode_len(s.size()), '\0');
		int d = modp_bjavascript_encode(const_cast<char*>(x.data()), s.data(), s.size());
		x.erase(d, std::string::npos);
		s.swap(x);
		return s;
	}

}
#endif

#endif
