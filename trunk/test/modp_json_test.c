/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modp_bjson.h"
#include "minunit.h"

/**
 * Smoke test
 */
static char* test_json_init()
{
    char buf[100];
    modp_json_ctx ctx;

    modp_json_init(&ctx, NULL);
    mu_assert(ctx.dest == NULL);

    modp_json_init(&ctx, buf);
    mu_assert(ctx.dest != NULL);

    modp_json_init(&ctx, NULL);
    mu_assert(ctx.dest == NULL);

    return 0;
}

static char* test_json_map_empty()
{
    size_t len;
    char buf[100];
    modp_json_ctx ctx;
    const int expected = 2;

    // do count
    modp_json_init(&ctx, buf);
    modp_json_map_open(&ctx);
    modp_json_map_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);

    // do real thing
    modp_json_init(&ctx, buf);
    modp_json_map_open(&ctx);
    modp_json_map_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);
    mu_assert_int_equals(strlen(buf), expected);
    mu_assert_str_equals("{}", buf);
    return 0;

}

static char* test_json_map_1()
{
    size_t len;
    char buf[100];
    modp_json_ctx ctx;
    const int expected = 13;

    // do count
    modp_json_init(&ctx, buf);
    modp_json_map_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_add_string(&ctx, "bar", 3);
    modp_json_map_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);

    // do real thing
    modp_json_init(&ctx, buf);
    modp_json_map_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_add_string(&ctx, "bar", 3);
    modp_json_map_close(&ctx);
    len = modp_json_end(&ctx);

    mu_assert_int_equals(strlen(buf), expected);
    mu_assert_str_equals("{\"foo\":\"bar\"}", buf);
    mu_assert_int_equals(len, expected);
    return 0;
}

static char* test_json_map_2()
{
    size_t len;
    char buf[100];
    modp_json_ctx ctx;
    const int expected = 21;

    // do count
    modp_json_init(&ctx, buf);
    modp_json_map_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_add_string(&ctx, "bar", 3);
    modp_json_add_string(&ctx, "x", 1);
    modp_json_add_string(&ctx, "y", 1);
    modp_json_map_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);

    // do real thing
    modp_json_init(&ctx, buf);
    modp_json_map_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_add_string(&ctx, "bar", 3);
    modp_json_add_string(&ctx, "x", 1);
    modp_json_add_string(&ctx, "y", 1);
    modp_json_map_close(&ctx);
    len = modp_json_end(&ctx);

    mu_assert_int_equals(strlen(buf), expected);
    mu_assert_str_equals("{\"foo\":\"bar\",\"x\":\"y\"}", buf);
    mu_assert_int_equals(len, expected);
    return 0;
}


static char* test_json_nest_1()
{
    size_t len;
    char buf[100];
    modp_json_ctx ctx;
    const int expected = 17;

    // do count
    modp_json_init(&ctx, buf);
    modp_json_map_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_ary_open(&ctx);
    modp_json_add_string(&ctx, "x", 1);
    modp_json_add_string(&ctx, "y", 1);
    modp_json_ary_close(&ctx);
    modp_json_map_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);

    // do real thing
    modp_json_init(&ctx, buf);
    modp_json_map_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_ary_open(&ctx);
    modp_json_add_string(&ctx, "x", 1);
    modp_json_add_string(&ctx, "y", 1);
    modp_json_ary_close(&ctx);
    modp_json_map_close(&ctx);
    len = modp_json_end(&ctx);

    mu_assert_int_equals(len, expected);
    mu_assert_int_equals(strlen(buf), expected);
    mu_assert_str_equals(buf, "{\"foo\":[\"x\",\"y\"]}");

    return 0;
}


static char* test_json_ary_empty()
{
    size_t len;
    char buf[100];
    modp_json_ctx ctx;
    const int expected = 2;

    // do count
    modp_json_init(&ctx, buf);
    modp_json_ary_open(&ctx);
    modp_json_ary_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);

    // do real thing
    modp_json_init(&ctx, buf);
    modp_json_ary_open(&ctx);
    modp_json_ary_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);
    mu_assert_int_equals(strlen(buf), expected);
    mu_assert_str_equals("[]", buf);
    return 0;
}

static char* test_json_ary_1()
{
    size_t len;
    char buf[100];
    modp_json_ctx ctx;
    const int expected = 7;

    // do count
    modp_json_init(&ctx, buf);
    modp_json_ary_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_ary_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);

    // do real thing
    modp_json_init(&ctx, buf);
    modp_json_ary_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_ary_close(&ctx);
    len = modp_json_end(&ctx);

    mu_assert_int_equals(strlen(buf), expected);
    mu_assert_str_equals("[\"foo\"]", buf);
    mu_assert_int_equals(len, expected);
    return 0;
}


static char* test_json_ary_2()
{
    size_t len;
    char buf[100];
    modp_json_ctx ctx;
    const int expected = 13;

    // do count
    modp_json_init(&ctx, buf);
    modp_json_ary_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_add_string(&ctx, "bar", 3);
    modp_json_ary_close(&ctx);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, expected);

    // do real thing
    modp_json_init(&ctx, buf);
    modp_json_ary_open(&ctx);
    modp_json_add_string(&ctx, "foo", 3);
    modp_json_add_string(&ctx, "bar", 3);
    modp_json_ary_close(&ctx);
    len = modp_json_end(&ctx);

    mu_assert_int_equals(strlen(buf), expected);
    mu_assert_str_equals("[\"foo\",\"bar\"]", buf);
    mu_assert_int_equals(len, expected);
    return 0;
}

static char* test_json_int()
{
    size_t len;
    char buf[100];
    modp_json_ctx ctx;

    // do count
    modp_json_init(&ctx, buf);
    modp_json_add_int(&ctx, 123);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, 3);
    mu_assert_str_equals("123", buf);

    modp_json_init(&ctx, buf);
    modp_json_add_int(&ctx, -123);
    len = modp_json_end(&ctx);
    mu_assert_int_equals(len, 4);
    mu_assert_str_equals("-123", buf);

    return 0;
}

static char* all_tests()
{
    mu_run_test(test_json_init);
    mu_run_test(test_json_map_empty);
    mu_run_test(test_json_map_1);
    mu_run_test(test_json_map_2);
    mu_run_test(test_json_ary_empty);
    mu_run_test(test_json_ary_1);
    mu_run_test(test_json_ary_2);
    mu_run_test(test_json_nest_1);
    mu_run_test(test_json_int);
    return 0;
}

UNITTESTS
