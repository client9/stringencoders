/* we compile as C90 but use snprintf */
#define _ISOC99_SOURCE

#include "minunit.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modp_numtoa.h"

/* Need only for INFINITY and optionally NAN macros */
/* We do not link with -lm */
#include <math.h>

static char* testITOA(void)
{
    char buf1[100];
    char buf2[100];
    int i;
    size_t len;
    for (i = 0; i < 100000; ++i) {
        sprintf(buf1, "%d", i);
        len = modp_itoa10(i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%d", -i);
        len = modp_itoa10(-i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%d", INT_MAX - i);
        len = modp_itoa10(INT_MAX - i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%d", -(INT_MAX - i));
        len = modp_itoa10(-(INT_MAX - i), buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);
    }
    return 0;
}

static char* testUITOA(void)
{
    char buf1[100];
    char buf2[100];
    uint32_t i;
    size_t len;
    for (i = 0; i < 1000000; ++i) {
        sprintf(buf1, "%u", i);
        len = modp_uitoa10(i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);
    }

    for (i = 0; i < 1000000; ++i) {
        sprintf(buf1, "%u", 0xFFFFFFFFu - i);
        len = modp_uitoa10(0xFFFFFFFFu - i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);
    }
    return 0;
}

static char* testLITOA(void)
{
    char buf1[100];
    char buf2[100];
    long int i;
    size_t len;
    for (i = 0; i < 100000; ++i) {
        sprintf(buf1, "%ld", i);
        len = modp_litoa10(i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%ld", -i);
        len = modp_litoa10(-i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%ld", LONG_MAX - i);
        len = modp_litoa10(LONG_MAX - i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%ld", -(LONG_MAX - i));
        len = modp_litoa10(-(LONG_MAX - i), buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);
    }
    return 0;
}

static char* testULITOA(void)
{
    char buf1[100];
    char buf2[100];
    size_t len;
    long long unsigned int i;
    for (i = 0; i < 1000000; ++i) {
        sprintf(buf1, "%llu", i);
        len = modp_ulitoa10(i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);
    }

    for (i = 0; i < 1000000; ++i) {
        sprintf(buf1, "%llu", 0xFFFFFFFFFFFFFFFFllu - i);
        len = modp_ulitoa10(0xFFFFFFFFFFFFFFFFull - i, buf2);
        mu_assert_int_equals(len, strlen(buf1));
        mu_assert_str_equals(buf1, buf2);
    }
    return 0;
}

static char* testDoubleToA(void)
{
    char buf1[100];
    char buf2[100];
    char msg[200];
    double d;
    size_t len;
    char* tmp;
    size_t tmplen;

    /* test each combination of whole number + fraction,
	   at every precision */
    /* and test negative version */
    double wholes[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0,
        67.0, 101.0, 10000, 99999 };
    double frac[] = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.49, 0.5, 0.51, 0.6, 0.7,
        0.9, 0.01, 0.25, 0.125, 0.03, 0.0625, 0.0078125,
        0.001, 0.00001, 0.99, 0.999, 0.9999, 0.99999, 0.999999,
        0.875, 0.9375, 0.96875, 0.9921875,
        //		0.95, 0.995, 0.9995, 0.99995, 0.999995, 0.9999995,
        0.09, 0.099, 0.0999, 0.09999, 0.099999, 0.0999999,
        0.09999999 };

    /* TBD
	 * 0.95, 0.995, 0.9995, 0.99995, 0.999995, 0.9999995
	 * since not exactly represented by floating point
	 * printf uses some tricks that we do not use
	 * causing test issues
	 */

    const char* formats[] = { "%.0f", "%.1f", "%.2f", "%.3f", "%.4f", "%.5f",
        "%.6f", "%.7f", "%.8f", "%.9f" };

    size_t imax = sizeof(wholes) / sizeof(double);
    size_t jmax = sizeof(frac) / sizeof(double);
    size_t kmax = sizeof(formats) / sizeof(const char*);

    size_t i, j, k;
    for (i = 0; i < imax; ++i) {
        for (j = 0; j < jmax; ++j) {
            for (k = 0; k < kmax; ++k) {
                d = wholes[i] + frac[j];

                sprintf(buf1, formats[k], d);
                sprintf(msg, "orig=%f whole=%f, frac=%f, prec=%d -- want %s",
                    wholes[i] + frac[j], wholes[i], frac[j], (int)k, buf1);
                len = modp_dtoa(d, buf2, (int)k);
                mu_assert_str_equals_msg(msg, buf1, buf2);
                mu_assert_int_equals(len, strlen(buf1));

                if ((int)wholes[i] != 0 && (int)(frac[j] * 10000000) != 0) {
                    sprintf(msg, "whole=%f, frac=%f, prec=%d -- ",
                        -wholes[i], frac[j], (int)k);
                    /* not dealing with "-0" issues */
                    d = -d;
                    sprintf(buf1, formats[k], d);
                    len = modp_dtoa(d, buf2, (int)k);
                    mu_assert_int_equals(len, strlen(buf1));
                    mu_assert_str_equals_msg(msg, buf1, buf2);

                    /* find the '.', and see how many chars are after it */
                    tmp = buf2;
                    while (*tmp != '.' && *tmp != '\0') {
                        ++tmp;
                    }
                    if (*tmp == '\0') {
                        tmplen = 0;
                    } else {
                        tmplen = strlen(++tmp);
                    }

                    sprintf(msg, "whole=%f, frac=%f, prec=%d, got=%d %s-- ",
                        wholes[i], frac[j], (int)k, (int)tmplen, buf2);
                    mu_assert_msg(msg, k >= tmplen);
                }
            }
        }
    }

    /* test very large positive number */
    d = 1.0e200;
    len = modp_dtoa(d, buf2, 6);
    mu_assert_int_equals(len, strlen(buf2));
    mu_assert_str_equals("1.000000e+200", buf2);

    /* test very large negative number */
    d = -1.0e200;
    len = modp_dtoa(d, buf2, 6);
    mu_assert_int_equals(len, strlen(buf2));
    mu_assert_str_equals("-1.000000e+200", buf2);

    /* test very small positive number */
    d = 1e-10;
    sprintf(buf1, "%.6f", d);
    len = modp_dtoa(d, buf2, 6);
    mu_assert_int_equals(len, strlen(buf1));
    mu_assert_str_equals(buf1, buf2);

    /* test very small negative number */
    d = -1e-10;
    sprintf(buf1, "%.6f", d);
    len = modp_dtoa(d, buf2, 6);
    mu_assert_int_equals(len, strlen(buf1));
    mu_assert_str_equals(buf1, buf2);

    return 0;
}

/* Helper function
 *  Removes trailing zeros
 * this is horible but it's just for testing.
 */
static void stripTrailingZeros(char* buf)
{
    size_t i;
    int hasdot = 0;
    for (i = 0; i < strlen(buf); ++i) {
        if (buf[i] == '.') {
            hasdot = 1;
            break;
        }
    }

    /* it's just an integer */
    if (!hasdot) {
        return;
    }

    i = strlen(buf);
    if (i == 0) {
        return;
    }
    --i;

    while (i > 0 && (buf[i] == '0' || buf[i] == '.')) {
        if (buf[i] == '.') {
            buf[i] = '\0';
            break;
        } else {
            buf[i] = '\0';
            --i;
        }
    }
}

static char* testDoubleToA2(void)
{
    char buf1[100];
    char buf2[100];
    char msg[200];
    double d;

    char* tmp;
    size_t len;
    size_t tmplen;

    /* test each combination of whole number + fraction,
	   at every precision */
    /* and test negative version */
    double wholes[] = { 0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0,
        67.0, 101.0, 10000, 99999 };
    double frac[] = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.49, 0.5, 0.51, 0.6, 0.7,
        0.9, 0.01, 0.25, 0.125, 0.03, 0.0625, 0.0078125,
        0.001, 0.00001, 0.99, 0.999, 0.9999, 0.99999, 0.999999,
        0.875, 0.9375, 0.96875, 0.9921875,
        0.09, 0.099, 0.0999, 0.09999, 0.099999, 0.0999999,
        0.09999999 };
    const char* formats[] = { "%.0f", "%.1f", "%.2f", "%.3f", "%.4f", "%.5f",
        "%.6f", "%.7f", "%.8f", "%.9f" };

    int imax = sizeof(wholes) / sizeof(double);
    int jmax = sizeof(frac) / sizeof(double);
    int kmax = sizeof(formats) / sizeof(const char*);

    int i, j, k;
    for (i = 0; i < imax; ++i) {
        for (j = 0; j < jmax; ++j) {
            for (k = 0; k < kmax; ++k) {
                d = wholes[i] + frac[j];

                sprintf(msg, "whole=%f, frac=%f, prec=%d -- ",
                    wholes[i], frac[j], k);

                sprintf(buf1, formats[k], d);
                stripTrailingZeros(buf1);
                len = modp_dtoa2(d, buf2, k);

                if ((int)wholes[i] != 0 && (int)(frac[j] * 10000000) != 0) {

                    /* find the '.', and see how many chars are after it */
                    tmp = buf2;
                    while (*tmp != '.' && *tmp != '\0') {
                        ++tmp;
                    }
                    if (*tmp == '\0') {
                        tmplen = 0;
                    } else {
                        tmplen = strlen(++tmp);
                    }

                    sprintf(msg, "orig=%f whole=%f, frac=%f, prec=%d -- want %s",
                        wholes[i] + frac[j], wholes[i], frac[j], (int)k, buf1);
                    mu_assert_str_equals_msg(msg, buf1, buf2);
                    mu_assert_msg(msg, (size_t)k >= tmplen);

                    /* not dealing with "-0" issues */
                    d = -d;
                    sprintf(buf1, formats[k], d);
                    stripTrailingZeros(buf1);

                    len = modp_dtoa2(d, buf2, k);
                    mu_assert_int_equals(len, strlen(buf2));
                    mu_assert_str_equals_msg(msg, buf1, buf2);
                }
            }
        }
    }

    /* test very large positive number */
    d = 1.0e200;
    len = modp_dtoa2(d, buf2, 6);
    mu_assert_int_equals(len, strlen(buf2));
    mu_assert_str_equals("1.000000e+200", buf2);

    /* test very large negative number */
    d = -1.0e200;
    len = modp_dtoa2(d, buf2, 6);
    mu_assert_int_equals(len, strlen(buf2));
    mu_assert_str_equals("-1.000000e+200", buf2);

    /* test very small positive number */
    d = 1e-10;
    sprintf(buf1, "%.6f", d);
    stripTrailingZeros(buf1);

    len = modp_dtoa2(d, buf2, 6);
    mu_assert_int_equals(len, strlen(buf2));
    mu_assert_str_equals(buf1, buf2);

    /* test very small negative number */
    d = -1e-10;
    sprintf(buf1, "%.6f", d);
    stripTrailingZeros(buf1);

    len = modp_dtoa2(d, buf2, 6);
    mu_assert_int_equals(len, strlen(buf2));
    mu_assert_str_equals(buf1, buf2);

    /* test bad precision values */
    d = 1.1;
    len = modp_dtoa(d, buf2, -1);
    mu_assert_int_equals(len, strlen(buf2));
    mu_assert_str_equals("1", buf2);
    len = modp_dtoa2(d, buf2, 10);
    mu_assert_int_equals(len, strlen(buf2));
    mu_assert_str_equals("1.1", buf2);
    return 0;
}

/* From Issue 7  -- http://code.google.com/p/stringencoders/issues/detail?id=7
 * thanks to http://code.google.com/u/simhasana/
 */
static char* testOverflowLITOA(void)
{
    char buf1[100];
    char buf2[100];

    long long int longmin = LONG_MIN;
    sprintf(buf1, "%lld", longmin);
    modp_litoa10(longmin, buf2);
    mu_assert_str_equals(buf1, buf2);

    long long int longmax = LONG_MAX;
    sprintf(buf1, "%lld", longmax);
    modp_litoa10(longmax, buf2);
    mu_assert_str_equals(buf1, buf2);

    return 0;
}

static char* testOverflowITOA(void)
{
    char buf1[100];
    char buf2[100];

    int32_t intmin = INT_MIN;
    sprintf(buf1, "%d", intmin);
    modp_itoa10(intmin, buf2);
    mu_assert_str_equals(buf1, buf2);

    int32_t intmax = INT_MAX;
    sprintf(buf1, "%d", intmax);
    modp_itoa10(intmax, buf2);
    mu_assert_str_equals(buf1, buf2);

    return 0;
}

/* Test NaN and Infinity behavior */
static char* testDTOANonFinite(void)
{
    char buf2[100];
    double d;

    /* Test for inf */
    d = 1e200 * 1e200;
    /* NOTE!!! next line will core dump!
	 * sprintf(buf1, "%.6f", d);
	 */
    buf2[0] = '\0';
    modp_dtoa2(d, buf2, 6);
    mu_assert_str_equals("inf", buf2);
    return 0;
}

static char* testDTOAInfinity(void)
{

/* INFINITY should be standard. Defined in <math.h> */
/* http://www.gnu.org/s/libc/manual/html_node/Infinity-and-NaN.html */
#ifdef INFINITY
    char buf1[100];
    char buf2[100];
    double d = INFINITY;

    /* test libc support */
    sprintf(buf1, "%f", d);
    mu_assert_str_equals("inf", buf1);

    buf2[0] = '\0';
    modp_dtoa(d, buf2, 6);
    mu_assert_str_equals("inf", buf2);

    buf2[0] = '\0';
    modp_dtoa2(d, buf2, 6);
    mu_assert_str_equals("inf", buf2);
#endif

    return 0;
}

static char* testDTOAandNAN(void)
{
/* NAN is a GNU extension, defined in <math.h> */
/* http://www.gnu.org/s/libc/manual/html_node/Infinity-and-NaN.html */

#ifdef NAN
    char buf1[100];
    char buf2[100];
    double d;

    d = NAN;

    /* test libc support */
    sprintf(buf1, "%f", d);
    mu_assert_str_equals("nan", buf1);

    /* now test ours */
    buf2[0] = '\0';
    modp_dtoa(d, buf2, 6);
    mu_assert_str_equals("nan", buf2);
    buf2[0] = '\0';
    modp_dtoa2(d, buf2, 6);
    mu_assert_str_equals("nan", buf2);
#endif

    return 0;
}

static char* testUITOA16(void)
{
    char buf1[100];
    char buf2[100];

    modp_uitoa16(1, buf1, 1);
    mu_assert_str_equals(buf1, "00000001");

    modp_uitoa16(0, buf1, 1);
    mu_assert_str_equals(buf1, "00000000");

    modp_uitoa16(0xFFFFFFFF, buf1, 1);
    mu_assert_str_equals(buf1, "FFFFFFFF");

    unsigned int i;
    for (i = 1; i < 1000000; ++i) {
        sprintf(buf1, "%08X", i);
        modp_uitoa16(i, buf2, 1);
        mu_assert_str_equals(buf1, buf2);
    }
    return 0;
}

/**
 * Attempt to replicate issue
 * http://code.google.com/p/stringencoders/issues/detail?id=15
 */
static char* testRoundingPrecisionOverflow(void)
{
    char buf1[100];

    modp_dtoa(0.09999999, buf1, 6);
    mu_assert_str_equals(buf1, "0.100000");
    modp_dtoa2(0.09999999, buf1, 6);
    mu_assert_str_equals(buf1, "0.1");
    return 0;
}

static char* all_tests(void)
{
    mu_run_test(testITOA);
    mu_run_test(testUITOA);
    mu_run_test(testLITOA);
    mu_run_test(testULITOA);
    mu_run_test(testDoubleToA);
    mu_run_test(testDoubleToA2);
    mu_run_test(testOverflowLITOA);
    mu_run_test(testOverflowITOA);
    mu_run_test(testDTOANonFinite);
    mu_run_test(testDTOAInfinity);
    mu_run_test(testDTOAandNAN);
    mu_run_test(testUITOA16);
    mu_run_test(testRoundingPrecisionOverflow);
    return 0;
}

UNITTESTS
