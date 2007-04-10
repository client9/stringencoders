/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

/**
 * \file
 *
 * <pre>
 * Copyright &copy; 2007, Nick Galbreath -- nickg [at] modp [dot] com
 * All rights reserved.
 * http://code.google.com/p/stringencoders/
 * Released under the bsd license.
 * </pre>
 *
 * This defines signed/unsigned integer, and 'double' to char buffer
 * converters.  The standard way of doing this is with "sprintf", however
 * these functions are
 *   * guarenteed maximum size output
 *   * 5-20x faster!
 *   * core-dump safe
 *
 *
 */

#ifndef MODP_NUMTOA_H
#define MODP_NUMTOA_H

#include <stdint.h>

/** \brief convert an signed integer to char buffer
 *
 * \param[in] value
 * \param[out] output buffer.  Should be 16 chars or more.
 */
void modp_itoa10(int32_t value, char* str);

/** \brief convert an unsigned integer to char buffer
 *
 * \param[in] value
 * \param[out] output buffer.  Should be 16 chars or more.
 */
void modp_uitoa10(uint32_t value, char* str);

/** \brief convert a floating point number to char buffer with fixed-precision format
 *
 * If the input value is greater than 1<<31, then the output format
 * will be switched exponential format.
 *
 * \param[in] value
 * \param[out] output buffer.  Should be 32 chars or more.
 * \param[in] precision.  Number of digits to the right of the decimal point.
 *    Can only be 0-9.
 */
void modp_dtoa(double value, char* str, int prec);

#endif
