/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modp_utf8.h"
#include "minunit.h"

/**
 * Test empty input to decode
 */
static char* testUtf8ValidateEmpty(void)
{
    int d = modp_utf8_validate("", (size_t)0);
    mu_assert_int_equals(d, 0);
    return 0;
}

static char* testUtf8ValidateAscii(void)
{
    int d = modp_utf8_validate("abc", (size_t)3);
    mu_assert_int_equals(d, 0);

    /* with null and highest ASCII character 127 */
    d = modp_utf8_validate("a\x00\x7Fz", (size_t) 4);
    return 0;
}

static char* testUtf8ValidateMulti(void)
{
    /*
     * examples taken from Wikipedia
     *  http://en.wikipedia.org/wiki/UTF-8
     */
    int d;
    const char* s2 = "\xC2\xA2";
    mu_assert_int_equals(strlen(s2), 2);
    d = modp_utf8_validate(s2, strlen(s2));
    mu_assert_int_equals(d, 0);

    const char* s3 = "\xE2\x82\xAC";
    mu_assert_int_equals(strlen(s3), 3);
    d = modp_utf8_validate(s3, strlen(s3));
    mu_assert_int_equals(d, 0);

    const char* s4 = "\xF0\xA4\xAD\xA2";
    mu_assert_int_equals(strlen(s4), 4);
    d = modp_utf8_validate(s4, strlen(s4));
    mu_assert_int_equals(d, 0);
    return 0;
}


static char* testUtf8ValidateShortRead(void)
{
    /*
     * examples taken from Wikipedia
     *  http://en.wikipedia.org/wiki/UTF-8
     */
    int d;
    d = modp_utf8_validate("\xC2\xA2", (size_t)1);
    mu_assert_int_equals(d, MODP_UTF8_SHORT);
    d = modp_utf8_validate("\xE2\x82\xAC", (size_t)2);
    mu_assert_int_equals(d, MODP_UTF8_SHORT);
    d = modp_utf8_validate("\xF0\xA4\xAD\xA2", (size_t)3);
    mu_assert_int_equals(d, MODP_UTF8_SHORT);
    return 0;
}

static char* testUtf8ValidateInvalid(void)
{
    return 0;
}

static char* all_tests(void)
{
    mu_run_test(testUtf8ValidateEmpty);
    mu_run_test(testUtf8ValidateAscii);
    mu_run_test(testUtf8ValidateMulti);
    mu_run_test(testUtf8ValidateShortRead);
    mu_run_test(testUtf8ValidateInvalid);
    return 0;
}

UNITTESTS

