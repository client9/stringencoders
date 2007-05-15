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

    /** \brief ascii upper case a string in place
     *
     * \param[in,out] str the input string
     * \param[in] len the length of input string (the strlen)
     */
    void modp_toupper(char* str, int len);

    /** \brief make lower case copy of input string
     *
     * \param[out] output buffer, with at least 'len + 1' byte allocated
     * \param[in] str the input string
     * \param[in] len the length of input string (the strlen)
     *
     * Please make sure dest has been allocation with at least 'len+1'
     * bytes.  This appends a trailing NULL character at the end of
     * dest!
     */
    void modp_toupper_copy(char* dest, const char* str, int len);

    /** \brief lower case a string in place
     *
     * \param[in, out] str the input string
     * \param[in] len the length of input string (the strlen)
     *
     */
    void modp_tolower(char* str, int len);

    /** \brief make lower case copy of input string
     *
     * \param[out] output buffer, with at least 'len + 1' byte allocated
     * \param[in] str the input string
     * \param[in] len the length of input string (the strlen)
     *
     * Please make sure dest has been allocation with at least 'len+1'
     * bytes.  This appends a trailing NULL character at the end of
     * dest!
     */
    void modp_tolower_copy(char* dest, const char* str, int len);

#ifdef __cplusplus
}
#endif

#endif

