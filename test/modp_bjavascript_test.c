/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modp_bjavascript.h"
#include "CuTest.h"

/**
 * Tests input where no escaping happens
 */
static void testNoEscape(CuTest* tc)
{
	char buf[100];
	const char* s1 = "this is a string";
	const int len1 = strlen(s1);
	int d = modp_bjavascript_encode(buf, s1, len1);

	CuAssertIntEquals(tc, len1, d);
	CuAssertStrEquals(tc, buf, s1);

	int sz = modp_bjavascript_encode_strlen(s1, len1);
	CuAssertIntEquals(tc, sz, len1);
}

static void testSimpleEscape(CuTest* tc)
{
	char buf[100];
	const char* s1 = "\\this\nis a string\n";
	const char* s2 = "\\\\this\\nis a string\\n";
	const int len1 = strlen(s1);
	const int len2 = strlen(s2);
	int d = modp_bjavascript_encode(buf, s1, len1);

	CuAssertIntEquals(tc, len2, d);
	CuAssertStrEquals(tc, buf, s2);

	int sz = modp_bjavascript_encode_strlen(s1, len1);
	CuAssertIntEquals(tc, sz, len2);

}

static void testSQuoteEscape(CuTest* tc)
{
	char buf[100];
	const char* s1 = "this is a 'string'\n";
	const char* s2 = "this is a \\'string\\'\\n";
	const int len1 = strlen(s1);
	const int len2 = strlen(s2);
	int d = modp_bjavascript_encode(buf, s1, len1);

	CuAssertIntEquals(tc, len2, d);
	CuAssertStrEquals(tc, buf, s2);

	int sz = modp_bjavascript_encode_strlen(s1, len1);
	CuAssertIntEquals(tc, sz, len2);
}

static void testDQuoteEscape(CuTest* tc)
{
	char buf[100];
	const char* s1 = "this is a \"string\"\n";
	const char* s2 = "this is a \\\"string\\\"\\n";
	const int len1 = strlen(s1);
	const int len2 = strlen(s2);
	int d = modp_bjavascript_encode(buf, s1, len1);

	CuAssertIntEquals(tc, len2, d);
	CuAssertStrEquals(tc, buf, s2);

	int sz = modp_bjavascript_encode_strlen(s1, len1);
	CuAssertIntEquals(tc, sz, len2);

}

static void testBinaryEscape(CuTest* tc)
{
	char buf[100];
	const char s1[] = {1,2,3,4,0};
	const char* s2 = "\\x01\\x02\\x03\\x04";
	const int len1 = strlen(s1);
	const int len2 = strlen(s2);
	int d = modp_bjavascript_encode(buf, s1, len1);

	CuAssertIntEquals(tc, len2, d);
	CuAssertStrEquals(tc, buf, s2);

	int sz = modp_bjavascript_encode_strlen(s1, len1);
	CuAssertIntEquals(tc, sz, len2);

}

static CuSuite* GetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testNoEscape);
    SUITE_ADD_TEST(suite, testSimpleEscape);
    SUITE_ADD_TEST(suite, testBinaryEscape);
    SUITE_ADD_TEST(suite, testSQuoteEscape);
    SUITE_ADD_TEST(suite, testDQuoteEscape);

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
