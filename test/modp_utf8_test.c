/* we compile as C90 but use snprintf */
#define _ISOC99_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"

#include "modp_utf8.h"
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
    d = modp_utf8_validate("a\x00\x7Fz", (size_t)4);
    mu_assert_int_equals(d, 0);
    return 0;
}

static char* testUtf8Validate2(void)
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

    d = modp_utf8_validate(s2, strlen(s2) - 1);
    mu_assert_int_equals(d, MODP_UTF8_SHORT);

    /* overlong, decodes to 0 */
    s2 = "\xC0\x80";
    d = modp_utf8_validate(s2, strlen(s2));
    mu_assert_int_equals(d, MODP_UTF8_OVERLONG);

    /* overlong, decodes to 127 */
    s2 = "\xC1\xBF";
    d = modp_utf8_validate(s2, strlen(s2));
    mu_assert_int_equals(d, MODP_UTF8_OVERLONG);

    s2 = "\xC2\xC0";
    d = modp_utf8_validate(s2, strlen(s2));
    mu_assert_int_equals(d, MODP_UTF8_INVALID);

    s2 = "\xC2\xFF";
    d = modp_utf8_validate(s2, strlen(s2));
    mu_assert_int_equals(d, MODP_UTF8_INVALID);

    s2 = "\xC2\x01";
    d = modp_utf8_validate(s2, strlen(s2));
    mu_assert_int_equals(d, MODP_UTF8_INVALID);

    return 0;
}

static char* testUtf8Validate3(void)
{
    int d;
    const char* s3 = "\xE2\x82\xAC";
    mu_assert_int_equals(strlen(s3), 3);
    d = modp_utf8_validate(s3, strlen(s3));
    mu_assert_int_equals(d, 0);

    d = modp_utf8_validate(s3, strlen(s3) - 1);
    mu_assert_int_equals(d, MODP_UTF8_SHORT);

    s3 = "\xEF\xBF\xBF";
    d = modp_utf8_validate(s3, strlen(s3));
    mu_assert_int_equals(d, MODP_UTF8_OK);

    s3 = "\xE0\x80\x80";
    d = modp_utf8_validate(s3, strlen(s3));
    mu_assert_int_equals(d, MODP_UTF8_OVERLONG);

    s3 = "\xE2\x01\xAC";
    d = modp_utf8_validate(s3, strlen(s3));
    mu_assert_int_equals(d, MODP_UTF8_INVALID);

    s3 = "\xE2\x82\x01";
    d = modp_utf8_validate(s3, strlen(s3));
    mu_assert_int_equals(d, MODP_UTF8_INVALID);
    return 0;
}

static char* testUtf8Validate4(void)
{
    int d;
    const char* s4 = "\xF0\xA4\xAD\xA2";
    mu_assert_int_equals(strlen(s4), 4);
    d = modp_utf8_validate(s4, strlen(s4));
    mu_assert_int_equals(d, 0);

    d = modp_utf8_validate(s4, strlen(s4) - 1);
    mu_assert_int_equals(d, MODP_UTF8_SHORT);

    /* maximum value */
    s4 = "\xF7\xBF\xBF\xBF";
    d = modp_utf8_validate(s4, strlen(s4));
    mu_assert_int_equals(d, MODP_UTF8_OK);

    s4 = "\xF0\x01\xAD\xA2";
    d = modp_utf8_validate(s4, strlen(s4));
    mu_assert_int_equals(d, MODP_UTF8_INVALID);

    s4 = "\xF0\xA4\x01\xA2";
    d = modp_utf8_validate(s4, strlen(s4));
    mu_assert_int_equals(d, MODP_UTF8_INVALID);

    s4 = "\xF0\xA4\xAD\x01";
    d = modp_utf8_validate(s4, strlen(s4));
    mu_assert_int_equals(d, MODP_UTF8_INVALID);

    return 0;
}
static char* testUtf8Validate5(void)
{
    int d;
    const char* s = "\xF8";
    mu_assert_int_equals(strlen(s), 1);
    d = modp_utf8_validate(s, strlen(s));
    mu_assert_int_equals(d, MODP_UTF8_CODEPOINT);

    s = "\xF9";
    mu_assert_int_equals(strlen(s), 1);
    d = modp_utf8_validate(s, strlen(s));
    mu_assert_int_equals(d, MODP_UTF8_CODEPOINT);

    s = "\xFA";
    mu_assert_int_equals(strlen(s), 1);
    d = modp_utf8_validate(s, strlen(s));
    mu_assert_int_equals(d, MODP_UTF8_CODEPOINT);

    s = "\xFB";
    mu_assert_int_equals(strlen(s), 1);
    d = modp_utf8_validate(s, strlen(s));
    mu_assert_int_equals(d, MODP_UTF8_CODEPOINT);

    s = "\xFC";
    mu_assert_int_equals(strlen(s), 1);
    d = modp_utf8_validate(s, strlen(s));
    mu_assert_int_equals(d, MODP_UTF8_CODEPOINT);

    s = "\xFD";
    mu_assert_int_equals(strlen(s), 1);
    d = modp_utf8_validate(s, strlen(s));
    mu_assert_int_equals(d, MODP_UTF8_CODEPOINT);

    s = "\xFE";
    mu_assert_int_equals(strlen(s), 1);
    d = modp_utf8_validate(s, strlen(s));
    mu_assert_int_equals(d, MODP_UTF8_CODEPOINT);

    s = "\xFF";
    mu_assert_int_equals(strlen(s), 1);
    d = modp_utf8_validate(s, strlen(s));
    mu_assert_int_equals(d, MODP_UTF8_CODEPOINT);

    return 0;
}

static char* all_tests(void)
{
    mu_run_test(testUtf8ValidateEmpty);
    mu_run_test(testUtf8ValidateAscii);
    mu_run_test(testUtf8Validate2);
    mu_run_test(testUtf8Validate3);
    mu_run_test(testUtf8Validate4);
    mu_run_test(testUtf8Validate5);
    return 0;
}

UNITTESTS
