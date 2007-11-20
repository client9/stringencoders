/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include "modp_numtoa.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"

static char* testITOA()
{
    char buf1[100];
    char buf2[100];
    int i;
    for (i = 0; i < 100000; ++i) {
        sprintf(buf1, "%d", i);
        modp_itoa10(i, buf2);
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%d", -i);
        modp_itoa10(-i, buf2);
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%d", INT_MAX - i);
        modp_itoa10(INT_MAX - i, buf2);
        mu_assert_str_equals(buf1, buf2);

        sprintf(buf1, "%d", -(INT_MAX - i));
        modp_itoa10(-(INT_MAX - i), buf2);
        mu_assert_str_equals(buf1, buf2);
    }
    return 0;
}

static char* testUITOA()
{
    char buf1[100];
    char buf2[100];
    uint32_t i;
    for (i = 0; i < 1000000; ++i) {
        sprintf(buf1, "%u", i);
        modp_uitoa10(i, buf2);
        mu_assert_str_equals(buf1, buf2);
    }

    for (i = 0; i < 1000000; ++i) {
        sprintf(buf1, "%u", 0xFFFFFFFFu - i);
        modp_uitoa10(0xFFFFFFFFu -i, buf2);
        mu_assert_str_equals(buf1, buf2);
    }
    return 0;
}

static char* testDoubleToA()
{
    char buf1[100];
    char buf2[100];
    char msg[200];
    double d;

    /* test each combination of whole number + fraction,
       at every precision */
    /* and test negative version */
    double wholes[] = {0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,
                       67.0,101.0, 10000, 99999};
    double frac[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.49, 0.5, 0.51, 0.6, 0.7,
                     0.9, 0.01, 0.25, 0.125, 0.05, 0.005, 0.0005, 0.00005,
                     0.001, 0.00001, 0.99, 0.999, 0.9999};
    const char* formats[] = {"%.0f", "%.1f", "%.2f", "%.3f", "%.4f", "%.5f",
                             "%.6f", "%.7f", "%.8f", "%.9f"};

    int imax = sizeof(wholes)/sizeof(double);
    int jmax = sizeof(frac)/sizeof(double);
    int kmax = sizeof(formats)/sizeof(const char*);

    int i,j,k;
    for (i = 0; i < imax; ++i) {
        for (j = 0; j < jmax; ++j) {
            for (k = 0; k < kmax; ++k) {
                d = wholes[i] + frac[j];

                sprintf(msg, "whole=%f, frac=%f, prec=%d -- ",
                        wholes[i], frac[j], k);
                sprintf(buf1, formats[k], d);
                modp_dtoa(d, buf2, k);
                mu_assert_str_equals_msg(msg,buf1, buf2);

                if (d != 0) {
                    sprintf(msg, "whole=%f, frac=%f, prec=%d -- ",
                            -wholes[i], frac[j], k);
                    /* not dealing with "-0" issues */
                    d = -d;
                    sprintf(buf1, formats[k], d);
                    modp_dtoa(d, buf2, k);
                    mu_assert_str_equals_msg(msg,buf1, buf2);
                }

            }
        }
    }

    /* test very large positive number */
    d = 1.0e200;
    modp_dtoa(d, buf2, 6);
    mu_assert_str_equals("1.000000e+200", buf2);

    /* test very large negative number */
    d = -1.0e200;
    modp_dtoa(d, buf2, 6);
    mu_assert_str_equals("-1.000000e+200", buf2);

    /* test very small positive number */
    d = 1e-10;
    sprintf(buf1, "%.6f", d);
    modp_dtoa(d, buf2, 6);
    mu_assert_str_equals(buf1, buf2);

    /* test very small negative number */
    d = -1e-10;
    sprintf(buf1, "%.6f", d);
    modp_dtoa(d, buf2, 6);
    mu_assert_str_equals(buf1, buf2);

    // test bad precision values
    d = 1.1;
    modp_dtoa(d, buf2, -1);
    mu_assert_str_equals("1", buf2);
    modp_dtoa(d, buf2, 10);
    mu_assert_str_equals("1.100000000", buf2);
    return 0;
}

static char* all_tests() {
    mu_run_test(testITOA);
    mu_run_test(testUITOA);
    mu_run_test(testDoubleToA);
    return 0;
}

UNITTESTS
