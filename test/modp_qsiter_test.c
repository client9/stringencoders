/* we compile as C90 but use snprintf */
#define _ISOC99_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"

#include "modp_qsiter.h"

/**
 * Test empty input to encode and decode
 */
static char* test_qs_init()
{
    struct qsiter_t qsi;

    const char* s = "foobar";

    qsiter_reset(&qsi, s, strlen(s));

    mu_assert_int_equals(qsi.pos, 0);
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 0);
    mu_assert_int_equals(qsi.vallen, 0);

    return 0;
}

static char* test_qs_parse1()
{
    struct qsiter_t qsi;

    const char* s = "foo=bar1";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, strlen(s));
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 3);
    mu_assert_int_equals(qsi.vallen, 4);
    mu_assert(!memcmp("foo", qsi.key, qsi.keylen));
    mu_assert(!memcmp("bar1", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* test_qs_parse2()
{
    struct qsiter_t qsi;

    const char* s = "foobar";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, strlen(s));
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 6);
    mu_assert_int_equals(qsi.vallen, 0);
    mu_assert(!memcmp("foobar", qsi.key, qsi.keylen));
    mu_assert(!memcmp("", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* test_qs_parse3()
{
    struct qsiter_t qsi;

    const char* s = "foo=bar&ding=bat";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, 8);
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 3);
    mu_assert_int_equals(qsi.vallen, 3);
    mu_assert(!memcmp("foo", qsi.key, qsi.keylen));
    mu_assert(!memcmp("bar", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, strlen(s));
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 4);
    mu_assert_int_equals(qsi.vallen, 3);
    mu_assert(!memcmp("ding", qsi.key, qsi.keylen));
    mu_assert(!memcmp("bat", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* test_qs_parse4()
{
    struct qsiter_t qsi;

    const char* s = "";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* test_qs_parse5()
{
    struct qsiter_t qsi;

    const char* s = "=";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, strlen(s));
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 0);
    mu_assert_int_equals(qsi.vallen, 0);
    mu_assert(!memcmp("", qsi.key, qsi.keylen));
    mu_assert(!memcmp("", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* test_qs_parse6()
{
    struct qsiter_t qsi;

    const char* s = "&";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, strlen(s));
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 0);
    mu_assert_int_equals(qsi.vallen, 0);
    mu_assert(!memcmp("", qsi.key, qsi.keylen));
    mu_assert(!memcmp("", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* test_qs_parse7()
{
    struct qsiter_t qsi;

    const char* s = "=&";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, strlen(s));
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 0);
    mu_assert_int_equals(qsi.vallen, 0);
    mu_assert(!memcmp("", qsi.key, qsi.keylen));
    mu_assert(!memcmp("", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* test_qs_parse8()
{
    struct qsiter_t qsi;

    const char* s = "&&";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, 1);
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 0);
    mu_assert_int_equals(qsi.vallen, 0);
    mu_assert(!memcmp("", qsi.key, qsi.keylen));
    mu_assert(!memcmp("", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, 2);
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 0);
    mu_assert_int_equals(qsi.vallen, 0);
    mu_assert(!memcmp("", qsi.key, qsi.keylen));
    mu_assert(!memcmp("", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* test_qs_parse9()
{
    struct qsiter_t qsi;

    const char* s = "&&foo=bar";

    qsiter_reset(&qsi, s, strlen(s));

    int ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, 1);
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 0);
    mu_assert_int_equals(qsi.vallen, 0);
    mu_assert(!memcmp("", qsi.key, qsi.keylen));
    mu_assert(!memcmp("", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, 2);
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 0);
    mu_assert_int_equals(qsi.vallen, 0);
    mu_assert(!memcmp("", qsi.key, qsi.keylen));
    mu_assert(!memcmp("", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(ok);
    mu_assert_int_equals(qsi.pos, strlen(s));
    mu_assert_int_equals(qsi.len, strlen(s));
    mu_assert_int_equals(qsi.keylen, 3);
    mu_assert_int_equals(qsi.vallen, 3);
    mu_assert(!memcmp("foo", qsi.key, qsi.keylen));
    mu_assert(!memcmp("bar", qsi.val, qsi.vallen));

    ok = qsiter_next(&qsi);
    mu_assert(!ok);

    return 0;
}

static char* all_tests()
{
    mu_run_test(test_qs_init);
    mu_run_test(test_qs_parse1);
    mu_run_test(test_qs_parse2);
    mu_run_test(test_qs_parse3);
    mu_run_test(test_qs_parse4);
    mu_run_test(test_qs_parse5);
    mu_run_test(test_qs_parse6);
    mu_run_test(test_qs_parse7);
    mu_run_test(test_qs_parse8);
    mu_run_test(test_qs_parse9);
    return 0;
}

UNITTESTS
