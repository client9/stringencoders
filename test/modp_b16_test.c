/* we compile as C90 but use snprintf */
#define _ISOC99_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"

#include "modp_b16.h"

static char* testEndian(void)
{
    /* this test that "0001" is "00000001" */
    char buf[100];
    char result[10];
    char endian[] = { (char)0, (char)0, (char)0, (char)1 };
    size_t d = modp_b16_encode(buf, endian, (size_t)4);
    mu_assert_int_equals(8, d);
    mu_assert_str_equals("00000001", buf);
    mu_assert_int_equals('0', buf[0]);
    mu_assert_int_equals('0', buf[1]);
    mu_assert_int_equals('0', buf[2]);
    mu_assert_int_equals('0', buf[3]);
    mu_assert_int_equals('0', buf[4]);
    mu_assert_int_equals('0', buf[5]);
    mu_assert_int_equals('0', buf[6]);
    mu_assert_int_equals('1', buf[7]);

    memset(result, 255, sizeof(result));
    d = modp_b16_decode(result, buf, (size_t)8);
    mu_assert_int_equals(4, d);
    mu_assert_int_equals(endian[0], result[0]);
    mu_assert_int_equals(endian[1], result[1]);
    mu_assert_int_equals(endian[2], result[2]);
    mu_assert_int_equals(endian[3], result[3]);

    return 0;
}

static char* testEncodeDecode(void)
{
    /* 2 bytes == 4 bytes out */
    char ibuf[2];
    char obuf[5];
    char rbuf[4];
    char msg[100]; /* for test messages output */
    msg[0] = 0; /* make msg an empty string */
    unsigned int i, j;
    size_t d;

    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 256; ++j) {
            /* comment this out.. it really slows down the test */
            sprintf(msg, "(i,j) = (%u,%u):", i, j);
            ibuf[0] = (char)((unsigned char)i);
            ibuf[1] = (char)((unsigned char)j);

            memset(obuf, 0, sizeof(obuf));
            d = modp_b16_encode(obuf, ibuf, (size_t)2);
            mu_assert_int_equals_msg(msg, 4, d);
            d = modp_b16_decode(rbuf, obuf, d);
            mu_assert_int_equals_msg(msg, 2, d);
            mu_assert_int_equals_msg(msg, ibuf[0], rbuf[0]);
            mu_assert_int_equals_msg(msg, ibuf[1], rbuf[1]);
        }
    }

    return 0;
}

static char* testOddDecode(void)
{
    char obuf[100];
    char ibuf[100];

    memset(obuf, 0, sizeof(obuf));
    memset(ibuf, 0, sizeof(ibuf));

    /* Test Odd Number of Input bytes
     * Should be error
     */
    obuf[0] = 1;
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, (size_t)1));
    mu_assert_int_equals(1, obuf[0]);

    obuf[0] = 1;
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, (size_t)3));
    mu_assert_int_equals(1, obuf[0]);

    obuf[0] = 1;
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, (size_t)7));
    mu_assert_int_equals(1, obuf[0]);

    return 0;
}

/** \brief test input that is a multiple of 2 (special case in code)
 */
static char* testDecodeMutlipleOf2(void)
{
    char obuf[100];
    memset(obuf, 0xff, sizeof(obuf));

    mu_assert_int_equals(1, modp_b16_decode(obuf, "01", (size_t)2));
    mu_assert_int_equals(1, obuf[0]);

    return 0;
}

static char* testOddEncode(void)
{
    char obuf[100];
    char ibuf[100];

    /* oddball 1 char. */
    ibuf[0] = 1;
    mu_assert_int_equals(2, modp_b16_encode(obuf, ibuf, (size_t)1));
    mu_assert_int_equals(obuf[0], '0');
    mu_assert_int_equals(obuf[1], '1');

    /* oddball 2 char. */
    ibuf[0] = 0;
    ibuf[1] = 1;
    mu_assert_int_equals(4, modp_b16_encode(obuf, ibuf, (size_t)2));
    mu_assert_int_equals(obuf[0], '0');
    mu_assert_int_equals(obuf[1], '0');
    mu_assert_int_equals(obuf[2], '0');
    mu_assert_int_equals(obuf[3], '1');

    /* oddball 1 char. */
    ibuf[0] = 0;
    ibuf[1] = 0;
    ibuf[2] = 1;
    mu_assert_int_equals(6, modp_b16_encode(obuf, ibuf, (size_t)3));
    mu_assert_int_equals(obuf[0], '0');
    mu_assert_int_equals(obuf[1], '0');
    mu_assert_int_equals(obuf[2], '0');
    mu_assert_int_equals(obuf[3], '0');
    mu_assert_int_equals(obuf[4], '0');
    mu_assert_int_equals(obuf[5], '1');

    return 0;
}

static char* testBadDecode(void)
{
    char obuf[100];
    char ibuf[100];

    memset(obuf, 0, sizeof(obuf));
    memset(ibuf, 0, sizeof(ibuf));

    /* we are testing input of 2 bytes, 4 bytes
     * to test all possible screwups
     */
    strcpy(ibuf, "X1");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "1X");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "XX");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    /* 1 bad char */
    strcpy(ibuf, "X111");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "1X11");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "11X1");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "111X");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    /* 2 bad chars */
    strcpy(ibuf, "XX11");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "1XX1");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "11XX");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "X1X1");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "1X1X");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "X11X");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    /* 3 bad chars */
    strcpy(ibuf, "1XXX");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "X1XX");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "XX1X");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));
    strcpy(ibuf, "XXX1");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    /* 4 bad chars */
    strcpy(ibuf, "XXXX");
    mu_assert_int_equals(-1, modp_b16_decode(obuf, ibuf, strlen(ibuf)));

    return 0;
}

static char* testEmptyInput(void)
{
    char obuf[100];
    char ibuf[100];

    memset(obuf, 0, sizeof(obuf));
    memset(ibuf, 0, sizeof(ibuf));

    /* encode 0 bytes, get a null byte back */
    obuf[0] = 1;
    mu_assert_int_equals(0, modp_b16_encode(obuf, ibuf, (size_t)0));
    mu_assert_int_equals(0, obuf[0]);

    /* decode 0 bytes, buffer is untouched */
    obuf[0] = 1;
    mu_assert_int_equals(0, modp_b16_decode(obuf, ibuf, (size_t)0));
    mu_assert_int_equals(1, obuf[0]);

    return 0;
}

static char* testLengths(void)
{
    /* Decode Len
     * 2 input -> 1 output.  No added NULL byte
     */
    mu_assert_int_equals(0, modp_b16_decode_len(0));
    mu_assert_int_equals(1, modp_b16_decode_len(1));
    mu_assert_int_equals(1, modp_b16_decode_len(2));
    mu_assert_int_equals(2, modp_b16_decode_len(3));
    mu_assert_int_equals(2, modp_b16_decode_len(4));

    /* Encode Len
     * 1 byte -> 2 output + 1 null byte
     */
    mu_assert_int_equals(1, modp_b16_encode_len(0));
    mu_assert_int_equals(3, modp_b16_encode_len(1));
    mu_assert_int_equals(5, modp_b16_encode_len(2));
    mu_assert_int_equals(7, modp_b16_encode_len(3));
    mu_assert_int_equals(9, modp_b16_encode_len(4));

    return 0;
}

static char* testValgrind(void)
{
    /*
     * an input of 3 chars, means output is 3*2+1 = 7 chars
     * However, the code works on output in values of 4bytes
     * so valgrind complains about use stomping on memory
     */
    const size_t buflen1 = 3;
    const size_t buflen2 = 7;
    char* ibuf = (char*)malloc(buflen1);
    ibuf[0] = '1';
    ibuf[1] = '2';
    ibuf[2] = '3';
    char* obuf = (char*)malloc(buflen2);

    memset(obuf, 0, buflen1);
    size_t d = modp_b16_encode(obuf, ibuf, buflen1);
    free(ibuf);
    free(obuf);

    mu_assert_int_equals(6, d);
    return 0;
}

static char* all_tests(void)
{
    mu_run_test(testEndian);
    mu_run_test(testEncodeDecode);
    mu_run_test(testLengths);
    mu_run_test(testEmptyInput);
    mu_run_test(testBadDecode);
    mu_run_test(testOddDecode);
    mu_run_test(testOddEncode);
    mu_run_test(testDecodeMutlipleOf2);
    mu_run_test(testValgrind);

    return 0;
}

UNITTESTS
