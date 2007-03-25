/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modp_b85.h"
#include "CuTest.h"

/**
 * checks to make sure results are the same reguardless of
 * CPU endian type (i.e. Intel vs. Sparc, etc)
 *
 */
void testEndian(CuTest* tc)
{
    // this test that "1" is "!!!!#"
    char buf[100];
    char result[10];
    char endian[] = {(char)0, (char)0, (char)0, (char)1};
    int d = modp_b85_encode(buf, endian, 4);
    CuAssertIntEquals(tc, 5, d);
    CuAssertIntEquals(tc, '!', buf[0]);
    CuAssertIntEquals(tc, '!', buf[1]);
    CuAssertIntEquals(tc, '!', buf[2]);
    CuAssertIntEquals(tc, '!', buf[3]);
    CuAssertIntEquals(tc, '#', buf[4]);

    memset(result, 255, sizeof(result));
    d = modp_b85_decode(result, "!!!!#", 5);
    CuAssertIntEquals(tc, 4, d);
    CuAssertIntEquals(tc, 0, result[0]);
    CuAssertIntEquals(tc, 0, result[1]);
    CuAssertIntEquals(tc, 0, result[2]);
    CuAssertIntEquals(tc, 1, result[3]);
    CuAssertIntEquals(tc, -1, result[4]);
}

void testLength(CuTest* tc)
{
    /**
     * Decode Len
     * 5 bytes -> 4 in, no null
     */
    CuAssertIntEquals(tc, 0, modp_b85_decode_len(0));
    CuAssertIntEquals(tc, 4, modp_b85_decode_len(5));
    CuAssertIntEquals(tc, 8, modp_b85_decode_len(10));

    /* Encode Len
     * 4 byte -> 5 output + 1 null byte
     */
    CuAssertIntEquals(tc,  1, modp_b85_encode_len(0));
    CuAssertIntEquals(tc,  6, modp_b85_encode_len(4));
    CuAssertIntEquals(tc, 11, modp_b85_encode_len(8));
}

/** \brief test bad input lengths
 * The b85 encode only accepts multiples of 4.
 *
 * The b85 decoder only accepts a mutliple of 5.
 *
 */
void testBadInputLength(CuTest* tc)
{
    char buf[100];
    CuAssertIntEquals(tc, -1, modp_b85_encode(buf, buf, 5));
    CuAssertIntEquals(tc, -1, modp_b85_decode(buf, buf, 11));
}

/** \brief test decoding invalid b85 chars
 *
 */
void testBadCharDecode(CuTest* tc)
{
    char buf[] = {'A', 'B', 'C', 'D', '\n', '\0'};
    CuAssertIntEquals(tc, -1, modp_b85_decode(buf, buf, 5));
}

void testEncodeDecode(CuTest* tc)
{
    char ibuf[10]; /* input */
    char obuf[10]; /* output */
    char rbuf[10]; /* final result */
    int d;
    int i,j,k,l;
    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 256; j+=16) { // save some time +=16
            for (k = 0; k < 256; k += 8) { // save some time += 8
                for (l = 0; l < 256; ++l) {
                    ibuf[0] = i;
                    ibuf[1] = j;
                    ibuf[2] = k;
                    ibuf[3] = l;
                    memset(obuf, 255, sizeof(obuf));
                    d = modp_b85_encode(obuf, ibuf, 4);
                    CuAssertIntEquals(tc, 5, d);
                    CuAssertIntEquals(tc, 0, obuf[5]);
                    memset(rbuf, 255, sizeof(rbuf));
                    d = modp_b85_decode(rbuf, obuf, 5);
                    CuAssertIntEquals(tc, 4, d);
                    CuAssertIntEquals(tc, ibuf[0], rbuf[0]);
                    CuAssertIntEquals(tc, ibuf[1], rbuf[1]);
                    CuAssertIntEquals(tc, ibuf[2], rbuf[2]);
                    CuAssertIntEquals(tc, ibuf[3], rbuf[3]);
                    CuAssertIntEquals(tc, -1, rbuf[4]);
                }
            }
        }
    }
}

static CuSuite* GetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, testEndian);
    SUITE_ADD_TEST(suite, testLength);
    SUITE_ADD_TEST(suite, testBadInputLength);
    SUITE_ADD_TEST(suite, testBadCharDecode);
    SUITE_ADD_TEST(suite, testEncodeDecode);
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
