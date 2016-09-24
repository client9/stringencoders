/**
 * \file modp_b85.h
 * \brief Base 85 encoding and decoding
 *
 * This provides a endian-safe base85 encode/decode operations.  This
 * means, the result will be the same on x86 or ibm/sparc chips.
 */

/*
 * <pre>
 * High Performance Base85 Encoder / Decoder
 *
 * Copyright &copy; 2006-2016 Nick Galbreath -- nickg [at] client9 [dot] com
 * All rights reserved.
 *
 * https://github.com/client9/stringencoders/
 *
 * Released under MIT license.  See LICENSE for details.
 * </PRE>
 *
 * This provides a endian-safe base85 encode/decode operations.  This
 * means, the result will be the same on x86 or ibm/sparc chips.
 *
 * (Note: making it endian-specifc only results in a 5% savings in
 *  the decode operation, so why bother)
 */

#ifndef COM_MODP_STRINGENCODERS_B85
#define COM_MODP_STRINGENCODERS_B85
#include "extern_c_begin.h"
#include "modp_stdint.h"

/**
 * \brief base 85 encode
 *
 * \param[out] dest  should have at least b85fast_encode_len memory allocated
 * \param[in] src   input string
 * \param[in] len   input string length, must be a multiple of 4
 * \return the strlen of the destination, or -1 if error
 *
 */
size_t modp_b85_encode(char* dest, const char* src, size_t len);

/**
 * \brief Base 85 decode
 * \param[out] dest -- destination locations.  May equal input.
 * \param[in] src -- source b85data
 * \param len -- length of source
 * \return -1 on decoding error, length of output otherwise
 *    No ending null is added
 */
size_t modp_b85_decode(char* dest, const char* src, size_t len);

/**
 * \brief Returns the amount of memory to allocate for encoding the input
 *        string.
 *
 */
#define modp_b85_encode_len(A) ((A + 3) / 4 * 5 + 1)

/**
 * \brief Return output strlen, without a NULL
 */
#define modp_b85_encode_strlen(A) ((A + 3) / 4 * 5)

/**
 * \brief Return the amount of memory to allocate for decoding a base 85
 *        encoded string.
 *
 */
#define modp_b85_decode_len(A) ((A + 4) / 5 * 4)

#include "extern_c_end.h"

#ifdef __cplusplus
#include <cstring>
#include <string>

namespace modp {

/**
     *
     * \param[in] s the input data
     * \param[in] len the length of input
     * \return b85 encoded string
     */
inline std::string b85_encode(const char* s, size_t len)
{
    std::string x(modp_b85_encode_len(len), '\0');
    size_t d = modp_b85_encode(const_cast<char*>(x.data()), s, len);
    if (d == (size_t)-1) {
        x.clear();
    } else {
        x.erase(d, std::string::npos);
    }
    return x;
}

/**
     * \param[in] null-terminated c-string input
     * \return b85 encoded string
     */
inline std::string b85_encode(const char* s)
{
    return b85_encode(s, strlen(s));
}

/**
     * /param[in,out] s the string to encode
     * /return a reference to the input string, empty if error
     */
inline std::string& b85_encode(std::string& s)
{
    std::string x(b85_encode(s.data(), s.size()));
    s.swap(x);
    return s;
}

/**
     * \param[in] s the input string
     * \return base85 encoded string
     */
inline std::string b85_encode(const std::string& s)
{
    return b85_encode(s.data(), s.size());
}

/**
     * Base85 decode a string.
     * This function does not allocate memory.
     *
     * \param s the string to decode
     * \return a reference to the input string. The string is empty
     *   if an error occurred.
     */
inline std::string& b85_decode(std::string& s)
{
    size_t d = modp_b85_decode(const_cast<char*>(s.data()), s.data(),
        s.size());
    if (d == (size_t)-1) {
        s.clear();
    } else {
        s.erase(d, std::string::npos);
    }
    return s;
}

inline std::string b85_decode(const std::string& s)
{
    std::string x(s);
    b85_decode(x);
    return x;
}

inline std::string b85_decode(const char* s, size_t len)
{
    std::string x(s, len);
    return b85_decode(x);
}

inline std::string b85_decode(const char* s)
{
    std::string x(s);
    return b85_decode(x);
}

} /* namespace modp */

#endif /* ifdef __cplusplus */

#endif /* ifndef MODP_B85 */
