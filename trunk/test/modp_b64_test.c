/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modp_b64.h"
#include "CuTest.h"

/**
 * checks to make sure results are the same reguardless of
 * CPU endian type (i.e. Intel vs. Sparc, etc)
 *
 */
void testEndian(CuTest* tc)
{
    // this test that "1" is "AAAB"
    char buf[100];
    char result[10];
    char endian[] = {(char)0, (char)0, (char)1};
    int d = modp_b64_encode(buf, endian, 3);
    CuAssertIntEquals(tc, 4, d);
    CuAssertIntEquals(tc, 'A', buf[0]);
    CuAssertIntEquals(tc, 'A', buf[1]);
    CuAssertIntEquals(tc, 'A', buf[2]);
    CuAssertIntEquals(tc, 'B', buf[3]);

    memset(result, 255, sizeof(result));
    d = modp_b64_decode(result, "AAAB", 4);
    CuAssertIntEquals(tc, 3, d);
    CuAssertIntEquals(tc, 0, result[0]);
    CuAssertIntEquals(tc, 0, result[1]);
    CuAssertIntEquals(tc, 1, result[2]);
    CuAssertIntEquals(tc, -1, result[3]);
}

/**
 * sending 0 as length to encode and decode
 * should bascially do nothing
 */
void testEmpty(CuTest* tc)
{
    char buf[10];
    const char* input = 0; // null
    int d;

    memset(buf, 1, sizeof(buf));
    d = modp_b64_encode(buf, input, 0);
    CuAssertIntEquals(tc, 0, d);
    CuAssertIntEquals(tc, 0, buf[0]);
    CuAssertIntEquals(tc, 1, buf[1]);

    memset(buf, 1, sizeof(buf));
    d = modp_b64_decode(buf, input, 0);
    CuAssertIntEquals(tc, 0, d);
    CuAssertIntEquals(tc, 1, buf[0]);
    CuAssertIntEquals(tc, 1, buf[1]);
}

/**
 * Test 1-6 bytes input and decode
 *
 */
void testPadding(CuTest* tc)
{
    char msg[100];
    const char ibuf[6] = {1,1,1,1,1,1};
    char obuf[10];
    char rbuf[10];
    int d = 0;

    // 1 in, 4 out
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, 1);
    sprintf(msg, "b64='%s', d=%d", obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 4, d);
    CuAssertIntEquals_Msg(tc, msg, 0, obuf[4]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 1, d);
    CuAssertIntEquals(tc, 1, rbuf[0]);
    CuAssertIntEquals(tc, -1, rbuf[1]);

    // 2 in, 4 out
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, 2);
    sprintf(msg, "b64='%s', d=%d", obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 4, d);
    CuAssertIntEquals_Msg(tc, msg, 0, obuf[4]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 2, d);
    CuAssertIntEquals_Msg(tc, msg, 1, rbuf[0]);
    CuAssertIntEquals_Msg(tc, msg, 1, rbuf[1]);
    CuAssertIntEquals_Msg(tc, msg, -1, rbuf[2]);

    // 3 in, 4 out
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, 3);
    sprintf(msg, "b64='%s', d=%d", obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 4, d);
    CuAssertIntEquals_Msg(tc, msg, 0, obuf[4]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 3, d);
    CuAssertIntEquals_Msg(tc, msg, 1, rbuf[0]);
    CuAssertIntEquals_Msg(tc, msg, 1, rbuf[1]);
    CuAssertIntEquals_Msg(tc, msg, 1, rbuf[2]);
    CuAssertIntEquals_Msg(tc, msg, -1, rbuf[3]);

    // 4 in, 8 out
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, 4);
    sprintf(msg, "b64='%s', d=%d", obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 8, d);
    CuAssertIntEquals_Msg(tc, msg, 0, obuf[8]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    CuAssertIntEquals(tc, 4, d);
    CuAssertIntEquals(tc, 1, rbuf[0]);
    CuAssertIntEquals(tc, 1, rbuf[1]);
    CuAssertIntEquals(tc, 1, rbuf[2]);
    CuAssertIntEquals(tc, 1, rbuf[3]);
    CuAssertIntEquals(tc, -1, rbuf[4]);

    // 5 in, 8 out
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, 5);
    sprintf(msg, "b64='%s', d=%d", obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 8, d);
    CuAssertIntEquals_Msg(tc, msg, 0, obuf[8]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    CuAssertIntEquals(tc, 5, d);
    CuAssertIntEquals(tc, 1, rbuf[0]);
    CuAssertIntEquals(tc, 1, rbuf[1]);
    CuAssertIntEquals(tc, 1, rbuf[2]);
    CuAssertIntEquals(tc, 1, rbuf[3]);
    CuAssertIntEquals(tc, 1, rbuf[4]);
    CuAssertIntEquals(tc, -1, rbuf[5]);

    // 6 in, 8 out
    memset(obuf, 255, sizeof(obuf));
    d = modp_b64_encode(obuf, ibuf, 6);
    sprintf(msg, "b64='%s', d=%d", obuf, d);
    CuAssertIntEquals_Msg(tc, msg, 8, d);
    CuAssertIntEquals_Msg(tc, msg, 0, obuf[8]);
    memset(rbuf, 255, sizeof(rbuf));
    d = modp_b64_decode(rbuf, obuf, d);
    CuAssertIntEquals(tc, 6, d);
    CuAssertIntEquals(tc, 1, rbuf[0]);
    CuAssertIntEquals(tc, 1, rbuf[1]);
    CuAssertIntEquals(tc, 1, rbuf[2]);
    CuAssertIntEquals(tc, 1, rbuf[3]);
    CuAssertIntEquals(tc, 1, rbuf[4]);
    CuAssertIntEquals(tc, 1, rbuf[5]);
    CuAssertIntEquals(tc, -1, rbuf[6]);
}

/**
 * Test all 17M 3 bytes inputs to encoder, decode
 * and make sure it's equal.
 */
void testEncodeDecode(CuTest* tc)
{
    char ibuf[4];
    char obuf[5];
    char rbuf[4];
    char msg[100];
    msg[0] = 0; // make msg an empty string
    unsigned int i,j,k;
    int d = 0;
    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 256; ++j) {
            for (k= 0; k < 256; ++k) {
                // comment this out.. it really slows down the test
                // sprintf(msg, "(i,j,k) = (%d,%d,%d):", i,j,k);
                ibuf[0] = (unsigned char) i;
                ibuf[1] = (unsigned char) j;
                ibuf[2] = (unsigned char) k;
                ibuf[3] = 0;

                memset(obuf, 1, sizeof(obuf));
                d = modp_b64_encode(obuf, ibuf, 3);
                CuAssertIntEquals_Msg(tc, msg, 4, d);
                CuAssertIntEquals_Msg(tc, msg, 0, obuf[4]);

                memset(rbuf, 1, sizeof(rbuf));
                d = modp_b64_decode(rbuf, obuf, 4);
                CuAssertIntEquals_Msg(tc, msg, 3, d);
                CuAssertIntEquals_Msg(tc, msg, ibuf[0], rbuf[0]);
                CuAssertIntEquals_Msg(tc, msg, ibuf[1], rbuf[1]);
                CuAssertIntEquals_Msg(tc, msg, ibuf[2], rbuf[2]);
                CuAssertIntEquals_Msg(tc, msg, 1, rbuf[3]);
            }
        }
    }
}

void testDecodeErrors(CuTest* tc)
{
    int i, y;
    char out[1000];
    char decode[5];
    char msg[100];

    /* negative length */
    memset(decode, 1, sizeof(decode));
    y = modp_b64_decode(out, decode, -1);
    CuAssertIntEquals(tc, -1, y);

    /* test bad input -  all combinations */
    char goodchar = 'A';
    char badchar = '~';
    for (i = 1; i < 16; ++i) {
        decode[0] = ((i & 0x01) == 0) ? goodchar : badchar;
        decode[1] = ((i & 0x02) == 0) ? goodchar : badchar;
        decode[2] = ((i & 0x04) == 0) ? goodchar : badchar;
        decode[3] = ((i & 0x08) == 0) ? goodchar : badchar;
        decode[4] = '\0';

        sprintf(msg, "i = %d, %s", i, decode);
        y = modp_b64_decode(out, decode, 4);
        CuAssertIntEquals_Msg(tc, msg, -1, y);
    }


    /*  test just 1-4 padchars */
    for (i = 0; i < 4; ++i) {
        decode[i] = '=';
        decode[i+1] = '\0';
        y = modp_b64_decode(out, decode, i+1);
        sprintf(msg, "i=%d, b64=%s", i, decode);
        CuAssertIntEquals_Msg(tc, msg, -1, y);
    }

    /* Test good+3 pad chars (should be impossible) */
    decode[0] = 'A';
    decode[1] = '=';
    decode[2] = '=';
    decode[3] = '=';
    y = modp_b64_decode(out, decode, 4);
    CuAssertIntEquals(tc, -1, y);
}

static CuSuite* GetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testEndian);
    SUITE_ADD_TEST(suite, testEmpty);
    SUITE_ADD_TEST(suite, testPadding);
    SUITE_ADD_TEST(suite, testEncodeDecode);
    SUITE_ADD_TEST(suite, testDecodeErrors);
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
