/* we compile as C90 but use snprintf */
#define _ISOC99_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"

#include "modp_b64.h"
/**
 * checks to make sure results are the same reguardless of
 * CPU endian type (i.e. Intel vs. Sparc, etc)
 *
 */
static char* testEndian(void)
{
    /* this test that "1" is "AAAB" */
    char buf[100];
    char result[10];
    char endian[] = { (char)0, (char)0, (char)1 };
    size_t d = modp_b64_encode(buf, endian, (size_t)3);
    mu_assert_int_equals(4, d);
    mu_assert_int_equals('A', buf[0]);
    mu_assert_int_equals('A', buf[1]);
    mu_assert_int_equals('A', buf[2]);
    mu_assert_int_equals('B', buf[3]);

    memset(result, 255, sizeof(result));
    d = modp_b64_decode(result, "AAAB", (size_t)4);
    mu_assert_int_equals(3, d);
    mu_assert_int_equals(0, result[0]);
    mu_assert_int_equals(0, result[1]);
    mu_assert_int_equals(1, result[2]);
    mu_assert_int_equals(-1, result[3]);

    return 0;
}

/**
 * sending 0 as length to encode and decode
 * should basically do nothing
 */
static char* testEmpty(void)
{
    char buf[10];
    const char* input = 0; /* null */
    size_t d;

    memset(buf, 1, sizeof(buf));
    d = modp_b64_encode(buf, input, (size_t)0);
    mu_assert_int_equals(0, d);
    mu_assert_int_equals(0, buf[0]);
    mu_assert_int_equals(1, buf[1]);

    memset(buf, 1, sizeof(buf));
    d = modp_b64_decode(buf, input, (size_t)0);
    mu_assert_int_equals(0, d);
    mu_assert_int_equals(1, buf[0]);
    mu_assert_int_equals(1, buf[1]);

    return 0;
}

/**
 * Test 1-6 bytes input and decode
 *
 */
static char* testPadding(void)
{
    char msg[100];
    const char ibuf[6] = { 1, 1, 1, 1, 1, 1 };
    char obuf[10];
    char rbuf[10];
    size_t d = 0;

    /* 1 in, 4 out */
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, (size_t)1);
    sprintf(msg, "b64='%s', d=%d", obuf, (int)d);
    mu_assert_int_equals_msg(msg, 4, d);
    mu_assert_int_equals_msg(msg, 0, obuf[4]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    mu_assert_int_equals_msg(msg, 1, d);
    mu_assert_int_equals(1, rbuf[0]);
    mu_assert_int_equals(-1, rbuf[1]);

    /* 2 in, 4 out */
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, (size_t)2);
    sprintf(msg, "b64='%s', d=%d", obuf, (int)d);
    mu_assert_int_equals_msg(msg, 4, d);
    mu_assert_int_equals_msg(msg, 0, obuf[4]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    mu_assert_int_equals_msg(msg, 2, d);
    mu_assert_int_equals_msg(msg, 1, rbuf[0]);
    mu_assert_int_equals_msg(msg, 1, rbuf[1]);
    mu_assert_int_equals_msg(msg, -1, rbuf[2]);

    /* 3 in, 4 out */
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, (size_t)3);
    sprintf(msg, "b64='%s', d=%d", obuf, (int)d);
    mu_assert_int_equals_msg(msg, 4, d);
    mu_assert_int_equals_msg(msg, 0, obuf[4]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    mu_assert_int_equals_msg(msg, 3, d);
    mu_assert_int_equals_msg(msg, 1, rbuf[0]);
    mu_assert_int_equals_msg(msg, 1, rbuf[1]);
    mu_assert_int_equals_msg(msg, 1, rbuf[2]);
    mu_assert_int_equals_msg(msg, -1, rbuf[3]);

    /* 4 in, 8 out */
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, (size_t)4);
    sprintf(msg, "b64='%s', d=%d", obuf, (int)d);
    mu_assert_int_equals_msg(msg, 8, d);
    mu_assert_int_equals_msg(msg, 0, obuf[8]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    mu_assert_int_equals(4, d);
    mu_assert_int_equals(1, rbuf[0]);
    mu_assert_int_equals(1, rbuf[1]);
    mu_assert_int_equals(1, rbuf[2]);
    mu_assert_int_equals(1, rbuf[3]);
    mu_assert_int_equals(-1, rbuf[4]);

    /* 5 in, 8 out */
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, (size_t)5);
    sprintf(msg, "b64='%s', d=%d", obuf, (int)d);
    mu_assert_int_equals_msg(msg, 8, d);
    mu_assert_int_equals_msg(msg, 0, obuf[8]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    mu_assert_int_equals(5, d);
    mu_assert_int_equals(1, rbuf[0]);
    mu_assert_int_equals(1, rbuf[1]);
    mu_assert_int_equals(1, rbuf[2]);
    mu_assert_int_equals(1, rbuf[3]);
    mu_assert_int_equals(1, rbuf[4]);
    mu_assert_int_equals(-1, rbuf[5]);

    /* 6 in, 8 out */
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, (size_t)6);
    sprintf(msg, "b64='%s', d=%d", obuf, (int)d);
    mu_assert_int_equals_msg(msg, 8, d);
    mu_assert_int_equals_msg(msg, 0, obuf[8]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    mu_assert_int_equals(6, d);
    mu_assert_int_equals(1, rbuf[0]);
    mu_assert_int_equals(1, rbuf[1]);
    mu_assert_int_equals(1, rbuf[2]);
    mu_assert_int_equals(1, rbuf[3]);
    mu_assert_int_equals(1, rbuf[4]);
    mu_assert_int_equals(1, rbuf[5]);
    mu_assert_int_equals(-1, rbuf[6]);

    return 0;
}

/**
 * Test all 17M 3 bytes inputs to encoder, decode
 * and make sure it's equal.
 */
static char* testEncodeDecode(void)
{
    char ibuf[4];
    char obuf[5];
    char rbuf[4];
    char msg[100];
    msg[0] = 0; /* make msg an empty string */
    unsigned int i, j, k;
    size_t d = 0;
    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 256; ++j) {
            for (k = 0; k < 256; ++k) {
                /* comment this out.. it really slows down the test */
                /* sprintf(msg, "(i,j,k) = (%d,%d,%d):", i,j,k); */
                ibuf[0] = (char)((unsigned char)i);
                ibuf[1] = (char)((unsigned char)j);
                ibuf[2] = (char)((unsigned char)k);
                ibuf[3] = 0;

                memset(obuf, 1, sizeof(obuf));
                d = modp_b64_encode(obuf, ibuf, (size_t)3);
                mu_assert_int_equals_msg(msg, 4, d);
                mu_assert_int_equals_msg(msg, 0, obuf[4]);

                memset(rbuf, 1, sizeof(rbuf));
                d = modp_b64_decode(rbuf, obuf, (size_t)4);
                mu_assert_int_equals_msg(msg, 3, d);
                mu_assert_int_equals_msg(msg, ibuf[0], rbuf[0]);
                mu_assert_int_equals_msg(msg, ibuf[1], rbuf[1]);
                mu_assert_int_equals_msg(msg, ibuf[2], rbuf[2]);
                mu_assert_int_equals_msg(msg, 1, rbuf[3]);
            }
        }
    }
    return 0;
}

static char* testDecodeErrors(void)
{
    int i;
    size_t y;
    char out[1000];
    char decode[5];
    char msg[100];

    /* negative length */
    memset(decode, 1, sizeof(decode));
    y = modp_b64_decode(out, decode, (size_t)-1);
    mu_assert_int_equals(-1, y);

    /* test bad input -  all combinations */
    char goodchar = 'A';
    char badchar = '~';
    for (i = 1; i < 16; ++i) {
        decode[0] = (char)(((i & 0x01) == 0) ? goodchar : badchar);
        decode[1] = (char)(((i & 0x02) == 0) ? goodchar : badchar);
        decode[2] = (char)(((i & 0x04) == 0) ? goodchar : badchar);
        decode[3] = (char)(((i & 0x08) == 0) ? goodchar : badchar);
        decode[4] = '\0';

        sprintf(msg, "i = %d, %s", i, decode);
        y = modp_b64_decode(out, decode, (size_t)4);
        mu_assert_int_equals_msg(msg, (size_t)-1, y);
    }

    /*  test just 1-4 padchars */
    for (i = 0; i < 4; ++i) {
        decode[i] = '=';
        decode[i + 1] = '\0';
        y = modp_b64_decode(out, decode, (size_t)(i + 1));
        sprintf(msg, "i=%d, b64=%s", i, decode);
        mu_assert_int_equals_msg(msg, (size_t)-1, y);
    }

    /* Test good+3 pad chars (should be impossible) */
    decode[0] = 'A';
    decode[1] = '=';
    decode[2] = '=';
    decode[3] = '=';
    y = modp_b64_decode(out, decode, (size_t)4);
    mu_assert_int_equals(-1, y);

    return 0;
}

static char* all_tests(void)
{
    mu_run_test(testEndian);
    mu_run_test(testEmpty);
    mu_run_test(testPadding);
    mu_run_test(testEncodeDecode);
    mu_run_test(testDecodeErrors);
    return 0;
}

UNITTESTS
