/* we compile as C90 but use snprintf */
#define _ISOC99_SOURCE

#include "minunit.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modp_ascii.h"

static char* testLowerSimple(void)
{
    const char* case4 = "ABCD abcd1234";
    const char* case3 = "ABCD abcd123";
    const char* case2 = "ABCD abcd12";
    const char* case1 = "ABCD abcd1";

    char orig[100];
    strcpy(orig, case4);
    modp_tolower(orig, strlen(orig));
    mu_assert_str_equals("abcd abcd1234", orig);

    strcpy(orig, case3);
    modp_tolower(orig, strlen(orig));
    mu_assert_str_equals("abcd abcd123", orig);

    strcpy(orig, case2);
    modp_tolower(orig, strlen(orig));
    mu_assert_str_equals("abcd abcd12", orig);

    strcpy(orig, case1);
    modp_tolower(orig, strlen(orig));
    mu_assert_str_equals("abcd abcd1", orig);

    return 0;
}

/** 
 * Test possible overflow conditions
 *
 */
static char* testUpperHighBits(void)
{
    char obuf[5];
    char expected[5];
    char buf[5];

    buf[0] = 'z';
    buf[1] = (char)0xff;
    buf[2] = (char)0xff;
    buf[3] = (char)0xff;
    buf[4] = '\0';

    expected[0] = 'Z';
    expected[1] = (char)0xff;
    expected[2] = (char)0xff;
    expected[3] = (char)0xff;
    expected[4] = '\0';

    memset(obuf, 0, sizeof(obuf));
    modp_toupper_copy(obuf, buf, strlen(buf));
    mu_assert_str_equals(expected, obuf);

    buf[0] = (char)0xff;
    buf[1] = (char)0xff;
    buf[2] = (char)0xff;
    buf[3] = 'z';
    buf[4] = '\0';

    expected[0] = (char)0xff;
    expected[1] = (char)0xff;
    expected[2] = (char)0xff;
    expected[3] = 'Z';
    expected[4] = '\0';
    memset(obuf, 0, sizeof(obuf));
    modp_toupper_copy(obuf, buf, strlen(buf));
    mu_assert_str_equals(expected, obuf);

    return 0;
}

static char* testUpperSimple(void)
{
    const char* case4 = "ABCD abcd1234";
    const char* case3 = "ABCD abcd123";
    const char* case2 = "ABCD abcd12";
    const char* case1 = "ABCD abcd1";

    char orig[100];
    strcpy(orig, case4);
    modp_toupper(orig, strlen(orig));
    mu_assert_str_equals("ABCD ABCD1234", orig);

    strcpy(orig, case3);
    modp_toupper(orig, strlen(orig));
    mu_assert_str_equals("ABCD ABCD123", orig);

    strcpy(orig, case2);
    modp_toupper(orig, strlen(orig));
    mu_assert_str_equals("ABCD ABCD12", orig);

    strcpy(orig, case1);
    modp_toupper(orig, strlen(orig));
    mu_assert_str_equals("ABCD ABCD1", orig);

    return 0;
}

static char* testPrintSimple(void)
{
    const char* case4 = "ABCD abcd\t\t\t\t";
    const char* case3 = "ABCD abcd\t\t\t";
    const char* case2 = "ABCD abcd\t\t";
    const char* case1 = "ABCD abcd\t";

    char orig[100];
    strcpy(orig, case4);
    modp_toprint(orig, strlen(orig));
    mu_assert_str_equals("ABCD abcd????", orig);

    strcpy(orig, case3);
    modp_toprint(orig, strlen(orig));
    mu_assert_str_equals("ABCD abcd???", orig);

    strcpy(orig, case2);
    modp_toprint(orig, strlen(orig));
    mu_assert_str_equals("ABCD abcd??", orig);

    strcpy(orig, case1);
    modp_toprint(orig, strlen(orig));
    mu_assert_str_equals("ABCD abcd?", orig);

    return 0;
}

static char* testToUpper(void)
{
    int i;
    char buf1[300];
    char expected[300];
    for (i = 0; i < 256; ++i) {
        buf1[i] = (char)i;
        expected[i] = (char)((i >= 'a' && i <= 'z') ? i - 32 : i);
    }
    modp_toupper(buf1, (size_t)256);
    mu_assert(memcmp(expected, buf1, (size_t)256) == 0);

    /* do other sizes to check */
    for (i = 0; i < 256; ++i)
        buf1[i] = (char)i;
    modp_toupper(buf1, (size_t)255);
    mu_assert(memcmp(expected, buf1, (size_t)255) == 0);

    for (i = 0; i < 256; ++i)
        buf1[i] = (char)i;
    modp_toupper(buf1, (size_t)254);
    mu_assert(memcmp(expected, buf1, (size_t)254) == 0);

    for (i = 0; i < 256; ++i)
        buf1[i] = (char)i;
    modp_toupper(buf1, (size_t)253);
    mu_assert(memcmp(expected, buf1, (size_t)253) == 0);
    return 0;
}

static char* testToLower(void)
{
    int i;
    char buf1[300];
    char expected[300];
    for (i = 0; i < 256; ++i) {
        buf1[i] = (char)i;
        expected[i] = (char)((i >= 'A' && i <= 'Z') ? i + 32 : i);
    }

    modp_tolower(buf1, (size_t)256);
    mu_assert(memcmp(expected, buf1, (size_t)256) == 0);

    /* do other sizes to check */
    for (i = 0; i < 256; ++i)
        buf1[i] = (char)i;
    modp_tolower(buf1, (size_t)255);
    mu_assert(memcmp(expected, buf1, (size_t)255) == 0);

    for (i = 0; i < 256; ++i)
        buf1[i] = (char)i;
    modp_tolower(buf1, (size_t)254);
    mu_assert(memcmp(expected, buf1, (size_t)254) == 0);

    for (i = 0; i < 256; ++i)
        buf1[i] = (char)i;
    modp_tolower(buf1, (size_t)253);
    mu_assert(memcmp(expected, buf1, (size_t)253) == 0);
    return 0;
}

static char* testToUpperCopy(void)
{
    int i;
    char buf1[300];
    char output[300];
    char expected[300];
    for (i = 0; i < 256; ++i) {
        buf1[i] = (char)i;
        output[i] = 0;
        expected[i] = (char)((i >= 'a' && i <= 'z') ? i - 32 : i);
    }
    buf1[i] = 0;
    output[i] = 0;

    modp_toupper_copy(output, buf1, (size_t)256);
    mu_assert(memcmp(expected, output, (size_t)256) == 0);

    /* do other sizes to check */
    for (i = 0; i < 256; ++i)
        output[i] = 0;
    modp_toupper_copy(output, buf1, (size_t)255);
    mu_assert(memcmp(expected, output, (size_t)255) == 0);

    for (i = 0; i < 256; ++i)
        output[i] = 0;
    modp_toupper_copy(output, buf1, (size_t)254);
    mu_assert(memcmp(expected, output, (size_t)254) == 0);

    for (i = 0; i < 256; ++i)
        output[i] = 0;
    modp_toupper_copy(output, buf1, (size_t)253);
    mu_assert(memcmp(expected, output, (size_t)253) == 0);
    return 0;
}

static char* testToLowerCopy(void)
{
    int i;
    char buf1[300];
    char output[300];
    char expected[300];
    for (i = 0; i < 256; ++i) {
        buf1[i] = (char)i;
        output[i] = 0;
        expected[i] = (char)((i >= 'A' && i <= 'Z') ? i + 32 : i);
    }
    buf1[i] = 0;
    output[i] = 0;

    modp_tolower_copy(output, buf1, (size_t)256);
    mu_assert(memcmp(expected, output, (size_t)256) == 0);

    /* do other sizes to check */
    for (i = 0; i < 256; ++i)
        output[i] = 0;
    modp_tolower_copy(output, buf1, (size_t)255);
    mu_assert(memcmp(expected, output, (size_t)255) == 0);

    for (i = 0; i < 256; ++i)
        output[i] = 0;
    modp_tolower_copy(output, buf1, (size_t)254);
    mu_assert(memcmp(expected, output, (size_t)254) == 0);

    for (i = 0; i < 256; ++i)
        output[i] = 0;
    modp_tolower_copy(output, buf1, (size_t)253);
    mu_assert(memcmp(expected, output, (size_t)253) == 0);
    return 0;
}

static char* all_tests(void)
{
    mu_run_test(testUpperHighBits);
    mu_run_test(testUpperSimple);
    mu_run_test(testLowerSimple);
    mu_run_test(testPrintSimple);
    mu_run_test(testToUpperCopy);
    mu_run_test(testToLowerCopy);
    mu_run_test(testToUpper);
    mu_run_test(testToLower);
    return 0;
}

UNITTESTS
