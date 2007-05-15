/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include "modp_ascii.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"

static char* testToUpper()
{
    int i;
    char buf1[300];
    char expected[300];
    for (i = 0; i < 256; ++i) {
        buf1[i] = (char) i;
        expected[i] = (i >= 'a' && i <= 'z') ? i-32 : i;
    }

    modp_toupper(buf1, 256);
    mu_assert(memcmp(expected, buf1, 256) == 0);

    // do other sizes to check
    for (i = 0; i < 256; ++i) buf1[i] = (char) i;
    modp_toupper(buf1, 255);
    mu_assert(memcmp(expected, buf1, 255) == 0);

    for (i = 0; i < 256; ++i) buf1[i] = (char) i;
    modp_toupper(buf1, 254);
    mu_assert(memcmp(expected, buf1, 254) == 0);

    for (i = 0; i < 256; ++i) buf1[i] = (char) i;
    modp_toupper(buf1, 253);
    mu_assert(memcmp(expected, buf1, 253) == 0);
    return 0;
}

static char* testToLower()
{
    int i;
    char buf1[300];
    char expected[300];
    for (i = 0; i < 256; ++i) {
        buf1[i] = (char) i;
        expected[i] = (i >= 'A' && i <= 'Z') ? i+32 : i;
    }

    modp_tolower(buf1, 256);
    mu_assert(memcmp(expected, buf1, 256) == 0);

    // do other sizes to check
    for (i = 0; i < 256; ++i) buf1[i] = (char) i;
    modp_tolower(buf1, 255);
    mu_assert(memcmp(expected, buf1, 255) == 0);

    for (i = 0; i < 256; ++i) buf1[i] = (char) i;
    modp_tolower(buf1, 254);
    mu_assert(memcmp(expected, buf1, 254) == 0);

    for (i = 0; i < 256; ++i) buf1[i] = (char) i;
    modp_tolower(buf1, 253);
    mu_assert(memcmp(expected, buf1, 253) == 0);
    return 0;
}

static char* testToUpperCopy()
{
    int i;
    char buf1[300];
    char output[300];
    char expected[300];
    for (i = 0; i < 256; ++i) {
        buf1[i] = (char) i;
        output[i] = 0;
        expected[i] = (i >= 'a' && i <= 'z') ? i-32 : i;
    }
    buf1[i] = 0;
    output[i] = 0;

    modp_toupper_copy(output, buf1, 256);
    mu_assert(memcmp(expected, output, 256) == 0);

    // do other sizes to check
    for (i = 0; i < 256; ++i) output[i] = 0;
    modp_toupper_copy(output, buf1, 255);
    mu_assert(memcmp(expected, output, 255) == 0);

    for (i = 0; i < 256; ++i) output[i] = 0;
    modp_toupper_copy(output, buf1, 254);
    mu_assert(memcmp(expected, output, 254) == 0);

    for (i = 0; i < 256; ++i) output[i] = 0;
    modp_toupper_copy(output, buf1, 253);
    mu_assert(memcmp(expected, output, 253) == 0);
    return 0;
}

static char* testToLowerCopy()
{
    int i;
    char buf1[300];
    char output[300];
    char expected[300];
    for (i = 0; i < 256; ++i) {
        buf1[i] = (char) i;
        output[i] = 0;
        expected[i] = (i >= 'A' && i <= 'Z') ? i+32 : i;
    }
    buf1[i] = 0;
    output[i] = 0;

    modp_tolower_copy(output, buf1, 256);
    mu_assert(memcmp(expected, output, 256) == 0);

    // do other sizes to check
    for (i = 0; i < 256; ++i) output[i] = 0;
    modp_tolower_copy(output, buf1, 255);
    mu_assert(memcmp(expected, output, 255) == 0);

    for (i = 0; i < 256; ++i) output[i] = 0;
    modp_tolower_copy(output, buf1, 254);
    mu_assert(memcmp(expected, output, 254) == 0);

    for (i = 0; i < 256; ++i) output[i] = 0;
    modp_tolower_copy(output, buf1, 253);
    mu_assert(memcmp(expected, output, 253) == 0);
    return 0;
}

static char* all_tests() {
    mu_run_test(testToUpperCopy);
    mu_run_test(testToLowerCopy);
    mu_run_test(testToUpper);
    mu_run_test(testToLower);
    return 0;
}

UNITTESTS


