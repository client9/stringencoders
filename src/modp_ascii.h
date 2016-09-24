/**
 * \file modp_ascii.h
 * \brief Simple ASCII manipulations including upper and lower casing,
 *        white space trimming, and conversion to "printable" characters.
 *
 * blah blah blah
 */

/*
 * <PRE>
 * MODP_ASCII -- Simple ascii manipulation (uppercase, lowercase, etc)
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2007-2016, Nick Galbreath -- nickg [at] client9 [dot] com
 * All rights reserved.
 *
 * Released under MIT license.  See LICENSE for details.
 * </PRE>
 *
 */

#ifndef COM_MODP_STRINGENCODERS_ASCII
#define COM_MODP_STRINGENCODERS_ASCII

#include "extern_c_begin.h"
#include "modp_stdint.h"

/*
 * \param[in,out] str the input string
 * \param[in] len the length of input string (the strlen)
 */
void modp_toupper(char* str, size_t len);

/** \brief make lower case copy of input string
 *
 * \param[out] dest output buffer, with at least 'len + 1' bytes allocated
 * \param[in] str the input string
 * \param[in] len the length of input string (the strlen)
 *
 * Please make sure dest has been allocation with at least 'len+1'
 * bytes.  This appends a trailing NULL character at the end of
 * dest!
 *
 * This is based on the algorithm by Paul Hsieh
 * http://www.azillionmonkeys.com/qed/asmexample.html
 */
void modp_toupper_copy(char* dest, const char* str, size_t len);

/** \brief lower case a string in place
 *
 * \param[in,out] str the input string
 * \param[in] len the length of input string (the strlen)
 *
 */
void modp_tolower(char* str, size_t len);

/** \brief make lower case copy of input string
 *
 * \param[out] dest output buffer, with at least 'len + 1' bytes allocated
 * \param[in] str the input string
 * \param[in] len the length of input string (the strlen)
 *
 * Please make sure dest has been allocation with at least 'len+1'
 * bytes.  This appends a trailing NULL character at the end of
 * dest!
 *
 * This is based on the algorithm by Paul Hsieh
 * http://www.azillionmonkeys.com/qed/asmexample.html
 */
void modp_tolower_copy(char* dest, const char* str, size_t len);

/** \brief turn a string into 7-bit printable ascii.
 *
 * By "printable" we means all characters between 32 and 126.
 * All other values are turned into '?'
 *
 * \param[in,out] str the input string
 * \param[in] len the length of input string (the strlen)
 *
 */
void modp_toprint(char* str, size_t len);

/** \brief make a printable copy of a string
 *
 * By "printable" we means all characters between 32 and 126.
 * All other values are turned into '?'
 *
 * \param[out] dest output buffer, with at least 'len + 1' bytes allocated
 * \param[in] str the input string
 * \param[in] len the length of input string (the strlen)
 *
 * Please make sure dest has been allocation with at least 'len+1'
 * bytes.  This appends a trailing NULL character at the end of
 * dest!
 */
void modp_toprint_copy(char* dest, const char* str, size_t len);

/**
 * \brief remove trailing whitespace from a string
 * \param[in,out] str  string to be stripped
 * \param[in] len the size of the input
 * \return the size of the output, not including any ending null byte.
 */
size_t modp_rtrim(char* str, size_t len);

#include "extern_c_end.h"

#ifdef __cplusplus
#include <string>

namespace modp {

inline std::string& toupper(std::string& str)
{
    modp_toupper(const_cast<char*>(str.c_str()), str.size());
    return str;
}

inline std::string toupper(const std::string& str)
{
    std::string s(str.size(), '\0');
    modp_toupper_copy(const_cast<char*>(s.data()), str.data(), str.size());
    return s;
}

inline std::string tolower(const std::string& str)
{
    std::string s(str.size(), '\0');
    modp_tolower_copy(const_cast<char*>(s.data()), str.data(), str.size());
    return s;
}

inline std::string& tolower(std::string& str)
{
    modp_tolower(const_cast<char*>(str.c_str()), str.size());
    return str;
}

inline std::string toprint(const std::string& str)
{
    std::string s(str.size(), '\0');
    modp_toprint_copy(const_cast<char*>(s.data()), str.data(), str.size());
    return s;
}

inline std::string& toprint(std::string& str)
{
    modp_toprint(const_cast<char*>(str.c_str()), str.size());
    return str;
}

inline std::string& rtrim(std::string& s)
{
    size_t d = modp_rtrim(const_cast<char*>(s.data()), s.size());
    s.erase(d, std::string::npos);
    return s;
}
}

#endif /* __cplusplus */

#endif /* MODP_ASCII */
