/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include "modp_numtoa.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest.h"

void testITOA(CuTest* tc)
{
	char buf1[100];
	char buf2[100];
	int i;
	for (i = 0; i < 100000; ++i) {
		sprintf(buf1, "%d", i);
		modp_itoa10(i, buf2);
		CuAssertStrEquals(tc, buf1, buf2);

		sprintf(buf1, "%d", -i);
		modp_itoa10(-i, buf2);
		CuAssertStrEquals(tc, buf1, buf2);

		sprintf(buf1, "%d", INT_MAX - i);
		modp_itoa10(INT_MAX - i, buf2);
		CuAssertStrEquals(tc, buf1, buf2);

		sprintf(buf1, "%d", -(INT_MAX - i));
		modp_itoa10(-(INT_MAX - i), buf2);
		CuAssertStrEquals(tc, buf1, buf2);
	}
}

void testUITOA(CuTest* tc)
{
	char buf1[100];
	char buf2[100];
	uint32_t i;
	for (i = 0; i < 1000000; ++i) {
		sprintf(buf1, "%u", i);
		modp_uitoa10(i, buf2);
		CuAssertStrEquals(tc, buf1, buf2);
	}

	for (i = 0; i < 1000000; ++i) {
		sprintf(buf1, "%u", 0xFFFFFFFFu - i);
		modp_uitoa10(0xFFFFFFFFu -i, buf2);
		CuAssertStrEquals(tc, buf1, buf2);
	}
}

void testDoubleToA(CuTest* tc)
{
	char buf1[100];
	char buf2[100];
	double d;
	double wholes[] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,11.0,67.0,101.0,10000, 99999};
	double frac[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.9, 0.01, 0.25, 0.125, 0.001, 0.00001};
	int imax = sizeof(wholes)/sizeof(double);
	int jmax = sizeof(frac)/sizeof(double);
	int i,j;
	for (i = 0; i < imax; ++i) {
		for (j = 0; j < jmax; ++j) {
			d = wholes[i] + frac[j];
			sprintf(buf1, "%.6f", d);
			modp_dtoa(d, buf2, 6);
			CuAssertStrEquals(tc, buf1, buf2);

			d = -d;
			sprintf(buf1, "%f", d);
			modp_dtoa(d, buf2, 6);
			CuAssertStrEquals(tc, buf1, buf2);
		}
	}

	d = 1.0e200;
	modp_dtoa(d, buf2, 6);
	CuAssertStrEquals(tc, "1.000000e+200", buf2);

	d = -1.0e200;
	modp_dtoa(d, buf2, 6);
	CuAssertStrEquals(tc, "-1.000000e+200", buf2);
}

static CuSuite* GetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testITOA);
    SUITE_ADD_TEST(suite, testUITOA);
    SUITE_ADD_TEST(suite, testDoubleToA);
    return suite;
}

int main(void) {
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
    CuSuiteAddSuite(suite, GetSuite());
    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    return 0;
}
