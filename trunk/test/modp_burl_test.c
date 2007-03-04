/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modp_burl.h"
#include "CuTest.h"

/**
 * Test empty input to encode and decode
 */
void TestUrlEmpty(CuTest* tc)
{
    int d;
    char buf[1000];
    buf[0] = 1;
    d = modp_burl_encode(buf, "", 0);
    CuAssertIntEquals(tc, d, 0);
    CuAssertTrue(tc, buf[0] == 0);

    buf[0] = 1;
    d = modp_burl_decode(buf, "", 0);
    CuAssertIntEquals(tc, d, 0);
    CuAssertTrue(tc, buf[0] == 0);
}

/**
 * test space <--> plus conversion
 */
void TestUrlSpaces(CuTest* tc)
{
    int d = 0;
    char buf[1000];
    const char* input = "   ";
    const char* output = "+++";

    d = modp_burl_encode(buf, input, strlen(input));
    CuAssertIntEquals(tc, d, strlen(output));
    CuAssertStrEquals(tc, buf, output);

    d = modp_burl_decode(buf, output, strlen(output));
    CuAssertIntEquals(tc, d, strlen(input));
    CuAssertStrEquals(tc, buf, input);
}

/**
 * Test charactes that should be unchanged
 */
void TestUrlUntouched(CuTest* tc)
{
    const char* lower = "abcdefghijklmnopqrstuvwxyz";
    const char* upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char* digits = "0123456789";
    const char* special = ".-_";
    char buf[1000];
    int d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_burl_encode(buf, lower, strlen(lower));
    CuAssertIntEquals(tc, d, strlen(lower));
    CuAssertStrEquals(tc, buf, lower);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, lower, strlen(lower));
    CuAssertIntEquals(tc, d, strlen(lower));
    CuAssertStrEquals(tc, buf, lower);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_encode(buf, upper, strlen(upper));
    CuAssertIntEquals(tc, d, strlen(upper));
    CuAssertStrEquals(tc, buf, upper);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, upper, strlen(upper));
    CuAssertIntEquals(tc, d, strlen(upper));
    CuAssertStrEquals(tc, buf, upper);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_encode(buf, digits, strlen(digits));
    CuAssertIntEquals(tc, d, strlen(digits));
    CuAssertStrEquals(tc, buf, digits);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, digits, strlen(digits));
    CuAssertIntEquals(tc, d, strlen(digits));
    CuAssertStrEquals(tc, buf, digits);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_encode(buf, special, strlen(special));
    CuAssertIntEquals(tc, d, strlen(special));
    CuAssertStrEquals(tc, buf, special);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, special, strlen(special));
    CuAssertIntEquals(tc, d, strlen(special));
    CuAssertStrEquals(tc, buf, special);
}


/**
 * Test charactes that should be unchanged
 */
void TestUrlMinUntouched(CuTest* tc)
{
    const char* lower   = "abcdefghijklmnopqrstuvwxyz";
    const char* upper   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char* digits  = "0123456789";
    const char* special = ".-_";
    const char* extra = "~!$()*,;:@/?";
    char buf[1000];
    int d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_burl_min_encode(buf, lower, strlen(lower));
    CuAssertIntEquals(tc, d, strlen(lower));
    CuAssertStrEquals(tc, buf, lower);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, lower, strlen(lower));
    CuAssertIntEquals(tc, d, strlen(lower));
    CuAssertStrEquals(tc, buf, lower);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_min_encode(buf, upper, strlen(upper));
    CuAssertIntEquals(tc, d, strlen(upper));
    CuAssertStrEquals(tc, buf, upper);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, upper, strlen(upper));
    CuAssertIntEquals(tc, d, strlen(upper));
    CuAssertStrEquals(tc, buf, upper);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_min_encode(buf, digits, strlen(digits));
    CuAssertIntEquals(tc, d, strlen(digits));
    CuAssertStrEquals(tc, buf, digits);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, digits, strlen(digits));
    CuAssertIntEquals(tc, d, strlen(digits));
    CuAssertStrEquals(tc, buf, digits);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_min_encode(buf, special, strlen(special));
    CuAssertIntEquals(tc, d, strlen(special));
    CuAssertStrEquals(tc, buf, special);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, special, strlen(special));
    CuAssertIntEquals(tc, d, strlen(special));
    CuAssertStrEquals(tc, buf, special);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_min_encode(buf, extra, strlen(extra));
    CuAssertIntEquals(tc, d, strlen(extra));
    CuAssertStrEquals(tc, buf, extra);
    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, extra, strlen(extra));
    CuAssertIntEquals(tc, d, strlen(extra));
    CuAssertStrEquals(tc, buf, extra);
}

void TestUrlDecodeHexBad(CuTest* tc)
{

    const char* bad1 = "%0X"; // bad trailing char
    const char* bad2 = "%X0"; // bad leading char
    const char* bad3 = "%XX"; // bad chars
    const char* bad4 = "%2"; // not enough room, good char
    const char* bad5 = "%X"; // not enought room, bad char
    int d = 0;
    char buf[1000];

    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, bad1, strlen(bad1));
    CuAssertIntEquals(tc, d, strlen(bad1));
    CuAssertStrEquals(tc, buf, bad1);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, bad2, strlen(bad2));
    CuAssertIntEquals(tc, strlen(bad2), d);
    CuAssertStrEquals(tc, bad2, buf);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, bad3, strlen(bad3));
    CuAssertIntEquals(tc, d, strlen(bad3));
    CuAssertStrEquals(tc, buf, bad3);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, bad4, strlen(bad4));
    CuAssertIntEquals(tc, d, strlen(bad4));
    CuAssertStrEquals(tc, buf, bad4);

    memset(buf, 0, sizeof(buf));
    d = modp_burl_decode(buf, bad5, strlen(bad5));
    CuAssertIntEquals(tc, d, strlen(bad5));
    CuAssertStrEquals(tc, buf, bad5);
}

void TestUrlDecodeHex(CuTest* tc)
{
    int d; // size of output
    int i, j; // loops
    int k = 0; // position in inputbuf;
    char inputbuf[3*256+1];
    char output[257];
    char msg[1000];

    // make input string contain every possible "%XX"
    static const char* hexdigits1 = "0123456789ABCDEF";
    memset(inputbuf, 0, sizeof(inputbuf));
    memset(output, 1, sizeof(output));
    k = 0;
    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 16; ++j) {
            inputbuf[k++] = '%';
            inputbuf[k++] = hexdigits1[i];
            inputbuf[k++] = hexdigits1[j];
        }
    }

    d = modp_burl_decode(output, inputbuf, sizeof(inputbuf)-1);
    CuAssertIntEquals(tc, d, 256);
    for (i = 0; i < 256; ++i) {
        sprintf(msg, "Loop at %d", i);
        CuAssertIntEquals_Msg(tc, msg, i, (unsigned char) output[i]);
    }

    // make input string contain every possible "%XX"
    static const char* hexdigits2 = "0123456789abcdef";
    memset(inputbuf, 0, sizeof(inputbuf));
    memset(output, 1, sizeof(output));

    k = 0;
    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 16; ++j) {
            inputbuf[k++] = '%';
            inputbuf[k++] = hexdigits2[i];
            inputbuf[k++] = hexdigits2[j];
        }
    }

    d = modp_burl_decode(output, inputbuf, sizeof(inputbuf)-1);
    CuAssertIntEquals(tc, 256, d);
    for (i = 0; i < 256; ++i) {
        sprintf(msg, "Loop at %d", i);
        CuAssertIntEquals_Msg(tc, msg, i, (unsigned char)output[i]);
    }
}

/**
 * test hex encoding.. to be done after hex decoding
 * is tested.
 */
void TestHexEncoding(CuTest* tc)
{
    int i = 0;
    int d = 0;
    char msg[1000];
    char input[257];
    memset(input, 0, sizeof(input));
    char output[257*3];
    memset(output, 0, sizeof(output));
    char buf[1000];
    memset(buf, 0, sizeof(buf));
    d = modp_burl_encode(output, input, 256);
    d = modp_burl_decode(buf, output, d);
    CuAssertIntEquals(tc, 256, d);
    for (i= 0; i < 256; ++i) {
        sprintf(msg, "Loop at %d failed", i);
        CuAssertIntEquals_Msg(tc, msg, input[i], buf[i]);
    }
}

void TestEncodeStrlen(CuTest* tc)
{
    char ibuf[100];
    char obuf[100];
    memset(ibuf, 0, sizeof(ibuf));
    memset(obuf, 0, sizeof(obuf));

    // Empty.  should be 0
    ibuf[0] = 0;
    CuAssertIntEquals(tc, strlen(ibuf), modp_burl_encode_strlen(ibuf, strlen(ibuf)));

    // Plain, should be same size
    strcpy(ibuf, "abcdefg");
    CuAssertIntEquals(tc, strlen(ibuf), modp_burl_encode_strlen(ibuf, strlen(ibuf)));

    // Plain and spaces, should be same size
    strcpy(ibuf, "a b c d e f g");
    CuAssertIntEquals(tc, strlen(ibuf), modp_burl_encode_strlen(ibuf, strlen(ibuf)));

    // one bad char, adds two bytes
    strcpy(ibuf, "abcdefg\n");
    CuAssertIntEquals(tc, strlen(ibuf)+2, modp_burl_encode_strlen(ibuf, strlen(ibuf)));

    // 2 bad chars, adds 4 bytes
    strcpy(ibuf, "\nabcdefg\n");
    CuAssertIntEquals(tc, strlen(ibuf)+4, modp_burl_encode_strlen(ibuf, strlen(ibuf)));

}

static CuSuite* GetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestUrlUntouched);
    SUITE_ADD_TEST(suite, TestUrlEmpty);
    SUITE_ADD_TEST(suite, TestUrlSpaces);
    SUITE_ADD_TEST(suite, TestUrlDecodeHex);
    SUITE_ADD_TEST(suite, TestUrlDecodeHexBad);
    SUITE_ADD_TEST(suite, TestHexEncoding);
    SUITE_ADD_TEST(suite, TestEncodeStrlen);
    SUITE_ADD_TEST(suite, TestUrlMinUntouched);
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
