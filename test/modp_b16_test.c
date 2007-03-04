/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modp_b16.h"
#include "CuTest.h"

static void testEndian(CuTest* tc)
{
    // this test that "0001" is "00000001"
    char buf[100];
    char result[10];
    char endian[] = {(char)0, (char)0, (char)0, (char)1};
    int d = modp_b16_encode(buf, endian, 4);
    CuAssertIntEquals(tc, 8, d);
    CuAssertStrEquals(tc, "00000001", buf);
    CuAssertIntEquals(tc, '0', buf[0]);
    CuAssertIntEquals(tc, '0', buf[1]);
    CuAssertIntEquals(tc, '0', buf[2]);
    CuAssertIntEquals(tc, '0', buf[3]);
    CuAssertIntEquals(tc, '0', buf[4]);
    CuAssertIntEquals(tc, '0', buf[5]);
    CuAssertIntEquals(tc, '0', buf[6]);
    CuAssertIntEquals(tc, '1', buf[7]);

    memset(result, 255, sizeof(result));
    d = modp_b16_decode(result, buf, 8);
    CuAssertIntEquals(tc, 4, d);
    CuAssertIntEquals(tc, endian[0], result[0]);
    CuAssertIntEquals(tc, endian[1], result[1]);
    CuAssertIntEquals(tc, endian[2], result[2]);
    CuAssertIntEquals(tc, endian[3], result[3]);
}

static void testEncodeDecode(CuTest* tc)
{
    // 2 bytes == 4 bytes out
    char ibuf[2];
    char obuf[5];
    char rbuf[4];
    char msg[100]; // for test messages output
    msg[0] = 0; // make msg an empty string
    unsigned int i,j;
    int d;

    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 256; ++j) {
            // comment this out.. it really slows down the test
            sprintf(msg, "(i,j) = (%d,%d):", i,j);
            ibuf[0] = (unsigned char) i;
            ibuf[1] = (unsigned char) j;

            memset(obuf, 0, sizeof(obuf));
            d = modp_b16_encode(obuf, ibuf, 2);
            CuAssertIntEquals_Msg(tc, msg, 4, d);
            d = modp_b16_decode(rbuf, obuf, d);
            CuAssertIntEquals_Msg(tc, msg, 2, d);
            CuAssertIntEquals_Msg(tc, msg, ibuf[0], rbuf[0]);
            CuAssertIntEquals_Msg(tc, msg, ibuf[1], rbuf[1]);
        }
    }
}

static void testOddDecode(CuTest* tc)
{
    char obuf[100];
    char ibuf[100];

    memset(obuf, 0, sizeof(obuf));
    memset(ibuf, 0, sizeof(ibuf));

    /* Test Odd Number of Input bytes
     * Should be error
     */
    obuf[0] = 1;
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, 1));
    CuAssertIntEquals(tc, 1, obuf[0]);

    obuf[0] = 1;
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, 3));
    CuAssertIntEquals(tc, 1, obuf[0]);

    obuf[0] = 1;
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, 7));
    CuAssertIntEquals(tc, 1, obuf[0]);
}

static void testBadDecode(CuTest* tc)
{
    char obuf[100];
    char ibuf[100];

    memset(obuf, 0, sizeof(obuf));
    memset(ibuf, 0, sizeof(ibuf));

    /* we are testing input of 2 bytes, 4 bytes
     * to test all possible screwups
     */
    strcpy(ibuf, "X1");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "1X");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "XX");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    /* 1 bad char */
    strcpy(ibuf, "X111");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "1X11");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "11X1");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "111X");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    /* 2 bad chars */
    strcpy(ibuf, "XX11");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "1XX1");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "11XX");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "X1X1");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "1X1X");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "X11X");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    /* 3 bad chars */
    strcpy(ibuf, "1XXX");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "X1XX");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "XX1X");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "XXX1");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    /* 4 bad chars */
    strcpy(ibuf, "XXXX");
    CuAssertIntEquals(tc, -1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
}

static void testEmptyInput(CuTest* tc)
{
    char obuf[100];
    char ibuf[100];

    memset(obuf, 0, sizeof(obuf));
    memset(ibuf, 0, sizeof(ibuf));

    // encode 0 bytes, get a null byte back
    obuf[0] = 1;
    CuAssertIntEquals(tc, 0, modp_b16_encode(obuf, ibuf,0));
    CuAssertIntEquals(tc, 0, obuf[0]);

    // decode 0 bytes, buffer is untouched
    obuf[0] = 1;
    CuAssertIntEquals(tc, 0, modp_b16_decode(obuf, ibuf, 0));
    CuAssertIntEquals(tc, 1, obuf[0]);
}

static void testLengths(CuTest* tc)
{
    /* Decode Len
     * 2 input -> 1 output.  No added NULL byte
     */
    CuAssertIntEquals(tc, 0, modp_b16_decode_len(0));
    CuAssertIntEquals(tc, 1, modp_b16_decode_len(1));
    CuAssertIntEquals(tc, 1, modp_b16_decode_len(2));
    CuAssertIntEquals(tc, 2, modp_b16_decode_len(3));
    CuAssertIntEquals(tc, 2, modp_b16_decode_len(4));

    /* Encode Len
     * 1 byte -> 2 output + 1 null byte
     */
    CuAssertIntEquals(tc, 1, modp_b16_encode_len(0));
    CuAssertIntEquals(tc, 3, modp_b16_encode_len(1));
    CuAssertIntEquals(tc, 5, modp_b16_encode_len(2));
    CuAssertIntEquals(tc, 7, modp_b16_encode_len(3));
    CuAssertIntEquals(tc, 9, modp_b16_encode_len(4));
}

static CuSuite* GetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testEndian);
    SUITE_ADD_TEST(suite, testEncodeDecode);
    SUITE_ADD_TEST(suite, testLengths);
    SUITE_ADD_TEST(suite, testEmptyInput);
    SUITE_ADD_TEST(suite, testBadDecode);
    SUITE_ADD_TEST(suite, testOddDecode);

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

