/* we compile as C90 but use snprintf */
#define _ISOC99_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"

#include "modp_html.h"

/**
 * Test empty input to decode
 */
static char* testHtmlDecodeEmpty(void)
{
    const char* junk = "junk";
    int d;
    size_t consumed;
    d = modp_html_decode_char_at(NULL, 10, &consumed);
    mu_assert_int_equals(d, -1);

    d = modp_html_decode_char_at(junk, 0, &consumed);
    mu_assert_int_equals(d, -1);

    return 0;
}

/**
 * Test characters that should be unchanged
 * Mostly a copy from URL encoding
 */
static char* testHtmlDecodeUntouched(void)
{
    const char* digits = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_~!$()*,;:@/?";

    int i;
    int ch;
    size_t consumed;

    for (i = 0; i < (int)strlen(digits); ++i) {
        consumed = 0;
        ch = modp_html_decode_char_at(digits + i, 1, &consumed);
        mu_assert_int_equals(ch, digits[i]);
        mu_assert_int_equals(consumed, 1);
    }
    return 0;
}

static char* testHtmlDecodeDigits(void)
{
    size_t consumed;
    int ch;
    const char* s1 = "&#98;";
    const char* s2 = "&#00;";
    const char* s3 = "&#001;";
    const char* s4 = "&#1;";
    const char* s5 = "&#01";
    const char* s6 = "&#1";
    const char* s7 = "&#";
    const char* s8 = "&#01X";

    consumed = 0;
    ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
    mu_assert_int_equals(ch, 98);
    mu_assert_int_equals(consumed, 5);

    consumed = 0;
    ch = modp_html_decode_char_at(s2, strlen(s2), &consumed);
    mu_assert_int_equals(ch, 0);
    mu_assert_int_equals(consumed, 5);

    consumed = 0;
    ch = modp_html_decode_char_at(s3, strlen(s3), &consumed);
    mu_assert_int_equals(ch, 1);
    mu_assert_int_equals(consumed, 6);

    consumed = 0;
    ch = modp_html_decode_char_at(s4, strlen(s4), &consumed);
    mu_assert_int_equals(ch, 1);
    mu_assert_int_equals(consumed, 4);

    consumed = 0;
    ch = modp_html_decode_char_at(s5, strlen(s5), &consumed);
    mu_assert_int_equals(ch, 1);
    mu_assert_int_equals(consumed, 4);

    consumed = 0;
    ch = modp_html_decode_char_at(s6, strlen(s6), &consumed);
    mu_assert_int_equals(ch, 1);
    mu_assert_int_equals(consumed, 3);

    consumed = 0;
    ch = modp_html_decode_char_at(s7, strlen(s7), &consumed);
    mu_assert_int_equals(ch, '&');
    mu_assert_int_equals(consumed, 1);

    consumed = 0;
    ch = modp_html_decode_char_at(s8, strlen(s8), &consumed);
    mu_assert_int_equals(ch, 1);
    mu_assert_int_equals(consumed, 4);

    return 0;
}

static char* testHtmlDecodeHexDigits(void)
{
    size_t consumed;
    int ch;

    {
        const char* s1 = "&#x98;";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, 0x98);
        mu_assert_int_equals(consumed, 6);
    }

    {
        const char* s1 = "&#x00;";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, 0);
        mu_assert_int_equals(consumed, 6);
    }

    {
        const char* s1 = "&#x001;";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, 1);
        mu_assert_int_equals(consumed, 7);
    }

    {
        const char* s1 = "&#x1;";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, 1);
        mu_assert_int_equals(consumed, 5);
    }

    {
        const char* s1 = "&#x01";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, 1);
        mu_assert_int_equals(consumed, 5);
    }

    {
        const char* s1 = "&#x1";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, 1);
        mu_assert_int_equals(consumed, 4);
    }

    {
        const char* s1 = "&#x";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, '&');
        mu_assert_int_equals(consumed, 1);
    }

    {
        const char* s1 = "&#x01X";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, 1);
        mu_assert_int_equals(consumed, 5);
    }

    {
        const char* s1 = "&#X";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, '&');
        mu_assert_int_equals(consumed, 1);
    }

    {
        const char* s1 = "&#X1";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, 1);
        mu_assert_int_equals(consumed, 4);
    }

    {
        /* overflow */
        const char* s1 = "&#xFFFFFF";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, '&');
        mu_assert_int_equals(consumed, 1);
    }

    {
        /* overflow */
        const char* s1 = "&#99999999999999999999999";
        consumed = 0;
        ch = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(ch, '&');
        mu_assert_int_equals(consumed, 1);
    }

    return 0;
}

/**
 * http://www.w3.org/html/wg/drafts/html/master/text-level-semantics.html#character-references
 * http://www.w3.org/html/wg/drafts/html/master/text-level-semantics.html#named-character-references
 * http://www.w3.org/html/wg/drafts/html/master/text-level-semantics.html#consume-a-character-reference
 */

static char* testHtmlDecodeNamedEntities(void)
{
    size_t consumed;
    int cp;

    {
        consumed = 0;
        const char* s1 = "&amp;";
        cp = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(cp, 38);
        mu_assert_int_equals(consumed, 5);
    }

    {
        consumed = 0;
        const char* s1 = "&AMP;";
        cp = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(cp, 38);
        mu_assert_int_equals(consumed, 5);
    }

    {
        consumed = 0;
        const char* s1 = "&amp";
        cp = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(cp, 38);
        mu_assert_int_equals(consumed, 4);
    }

    {
        consumed = 0;
        const char* s1 = "&AMP";
        cp = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(cp, 38);
        mu_assert_int_equals(consumed, 4);
    }

    {
        /* &not is a special one where '&not' and '&not;' are defined */
        consumed = 0;
        const char* s1 = "&notit";
        cp = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(cp, 172);
        mu_assert_int_equals(consumed, 4);
    }

    {
        /* &not is a special one where '&not' and '&not;' are defined */
        consumed = 0;
        const char* s1 = "&notin;";
        cp = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(cp, 8713);
        mu_assert_int_equals(consumed, 7);
    }

    {
        /* off the edge */
        consumed = 0;
        const char* s1 = "&NotHumpEqua";
        cp = modp_html_decode_char_at(s1, strlen(s1), &consumed);
        mu_assert_int_equals(cp, 38);
        mu_assert_int_equals(consumed, 1);
    }

    return 0;
}

static char* all_tests(void)
{
    mu_run_test(testHtmlDecodeEmpty);
    mu_run_test(testHtmlDecodeUntouched);
    mu_run_test(testHtmlDecodeDigits);
    mu_run_test(testHtmlDecodeHexDigits);
    mu_run_test(testHtmlDecodeNamedEntities);

    return 0;
}

UNITTESTS
