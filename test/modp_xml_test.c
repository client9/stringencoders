/* we compile as C90 but use snprintf */
#define _ISOC99_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"

#include "modp_xml.h"

/**
 * Test empty input to decode
 */
static char* testXmlDecodeEmpty(void)
{
    size_t d;
    char buf[1000];

    buf[0] = 1;
    d = modp_xml_decode(buf, "", (size_t)0);
    mu_assert_int_equals(d, 0);
    mu_assert(buf[0] == 0);

    return 0;
}

/**
 * Test characters that should be unchanged
 * Mostly a copy from URL encoding
 */
static char* testXmlDecodeUntouched(void)
{
    const char* lower = "abcdefghijklmnopqrstuvwxyz";
    const char* upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char* digits = "0123456789";
    const char* special = ".-_~!$()*,;:@/?";
    char buf[1000];

    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, lower, strlen(lower));
    mu_assert_int_equals(d, strlen(lower));
    mu_assert_str_equals(buf, lower);

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, upper, strlen(upper));
    mu_assert_int_equals(d, strlen(upper));
    mu_assert_str_equals(buf, upper);

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, digits, strlen(digits));
    mu_assert_int_equals(d, strlen(digits));
    mu_assert_str_equals(buf, digits);

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, special, strlen(special));
    mu_assert_int_equals(d, strlen(special));
    mu_assert_str_equals(buf, special);

    return 0;
}

/**
 * Test characters that should be unchanged
 * Mostly a copy from URL encoding
 */
static char* testXmlDecodeNames(void)
{
    const char* strin = "X&quot;X&apos;X&amp;X&gt;X&lt;X";
    const char* strout = "X\"X'X&X>X<X";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));

    mu_assert_str_equals(strout, buf);

    return 0;
}

/**
 * Bad entity name.. not existent
 */
static char* testXmlDecodeBadNames1(void)
{
    const char* strin = "X&quo;X";
    const char* strout = "X&quo;X";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));
    mu_assert_str_equals(strout, buf);

    return 0;
}

/**
 * Bad entity name -- no semi-colon
 */
static char* testXmlDecodeBadNames2(void)
{
    const char* strin = "X&quotX";
    const char* strout = "X&quotX";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));
    mu_assert_str_equals(strout, buf);

    return 0;
}

/**
 * Bad entity name -- no name!
 */
static char* testXmlDecodeBadNames3(void)
{
    const char* strin = "X&;X";
    const char* strout = "X&;X";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));
    mu_assert_str_equals(strout, buf);

    return 0;
}

/**
 * Bad entity name -- boundary!
 */
static char* testXmlDecodeBadNames4(void)
{
    const char* strin = "X&;";
    const char* strout = "X&;";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));
    mu_assert_str_equals(strout, buf);

    return 0;
}

/**
 * Bad entity name -- boundary!
 */
static char* testXmlDecodeBadNames5(void)
{
    const char* strin = "X&";
    const char* strout = "X&";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));
    mu_assert_str_equals(strout, buf);

    return 0;
}

/**
 * Bad entity name.. case sensitive
 */
static char* testXmlDecodeBadNames6(void)
{
    const char* strin = "X&QUOT;X";
    const char* strout = "X&QUOT;X";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));
    mu_assert_str_equals(strout, buf);

    return 0;
}

static char* testXmlUnicodeCodePoints(void)
{
    static const int ranges[] = {
        0x0000, 0x0008, /* control characters */
        0x000B, 0x000B, /* Vertical Tab is forbidden, ?? */
        0x000E, 0x001F, /* control characters */
        0x0080, 0x009F, /* control characters */
        0x0750, 0x077D, /* the rest are undefined */
        0x07C0, 0x08FF,
        0x1380, 0x139F,
        0x18B0, 0x18FF,
        0x1980, 0x19DF,
        0x1A00, 0x1CFF,
        0x1D80, 0x1DFF,
        0x2C00, 0x2E7F,
        0x2FE0, 0x2FEF,
        0x31C0, 0x31EF,
        0x9FB0, 0x9FFF,
        0xA4D0, 0xABFF,
        0xD7B0, 0xD7FF,
        0xFE10, 0xFE1F,
        0x10140, 0x102FF,
        0x104B0, 0x107FF,
        0x1D200, 0x1D2FF,
        0x1D360, 0x1D3FF,
        0x1D800, 0x1FFFF,
        0x2A6E0, 0x2F7FF,
        0x2FAB0, 0xDFFFF,
        0xE0080, 0xE00FF,
        0xE01F0, 0xEFFFF,
        0xFFFFE, 0xFFFFF
    };
    static const int imax = sizeof(ranges) / sizeof(uint32_t);
    int i;
    for (i = 0; i < imax; ++i) {
        mu_assert_int_equals(-1, modp_xml_validate_unicode(ranges[i]));
    }

    /* too big */
    mu_assert_int_equals(-1, modp_xml_validate_unicode(0xFFFFFF));

    /* ok */
    mu_assert_int_equals(0x41, modp_xml_validate_unicode(0x41));
    return 0;
}

static char* testXmlUnicodeCharToUTF8(void)
{
    char buf[1000];
    size_t d = 0;
    int val = 0;

    d = modp_xml_unicode_char_to_utf8(buf, 0x41);
    mu_assert_int_equals(1, d);
    buf[d] = '\0';
    mu_assert_str_equals(buf, "A");

    /* Cent symbol */
    d = modp_xml_unicode_char_to_utf8(buf, 0x00A2);
    mu_assert_int_equals(2, d);
    val = (unsigned char)buf[0];
    mu_assert_int_equals(val, 0xC2);
    val = (unsigned char)buf[1];
    mu_assert_int_equals(val, 0xA2);

    /* Euro symbol */
    d = modp_xml_unicode_char_to_utf8(buf, 0x20AC);
    mu_assert_int_equals(3, d);
    val = (unsigned char)buf[0];
    mu_assert_int_equals(val, 0xE2);
    val = (unsigned char)buf[1];
    mu_assert_int_equals(val, 0x82);
    val = (unsigned char)buf[2];
    mu_assert_int_equals(val, 0xAC);

    /* Chinese Character */
    d = modp_xml_unicode_char_to_utf8(buf, 0x24B62);
    mu_assert_int_equals(4, d);
    val = (unsigned char)buf[0];
    mu_assert_int_equals(val, 0xF0);
    val = (unsigned char)buf[1];
    mu_assert_int_equals(val, 0xA4);
    val = (unsigned char)buf[2];
    mu_assert_int_equals(val, 0xAD);
    val = (unsigned char)buf[3];
    mu_assert_int_equals(val, 0xA2);

    return 0;
}

static char* testXmlDecodeDecimalEntities(void)
{
    const char* strin = "X&#65;X";
    const char* strout = "XAX";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));
    mu_assert_str_equals(strout, buf);

    return 0;
}

static char* testXmlDecodeHexEntities(void)
{
    const char* strin = "X&#x41;X";
    const char* strout = "XAX";

    char buf[1000];
    size_t d = 0;

    memset(buf, 0, sizeof(buf));
    d = modp_xml_decode(buf, strin, strlen(strin));
    mu_assert_int_equals(d, strlen(strout));
    mu_assert_str_equals(strout, buf);

    return 0;
}

static char* all_tests(void)
{
    mu_run_test(testXmlDecodeEmpty);
    mu_run_test(testXmlDecodeUntouched);
    mu_run_test(testXmlDecodeNames);
    mu_run_test(testXmlDecodeBadNames1);
    mu_run_test(testXmlDecodeBadNames2);
    mu_run_test(testXmlDecodeBadNames3);
    mu_run_test(testXmlDecodeBadNames4);
    mu_run_test(testXmlDecodeBadNames5);
    mu_run_test(testXmlDecodeBadNames6);
    mu_run_test(testXmlUnicodeCodePoints);
    mu_run_test(testXmlUnicodeCharToUTF8);
    mu_run_test(testXmlDecodeDecimalEntities);
    mu_run_test(testXmlDecodeHexEntities);
    return 0;
}

UNITTESTS
