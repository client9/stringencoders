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

static char* all_tests(void)
{
    mu_run_test(testUtf8ValidateEmpty);
    return 0;
}

UNITTESTS

