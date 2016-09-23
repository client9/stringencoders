#include "arraytoc.h"
#include <stdio.h>

/* dump uint32_t as hex digits */
void uint32_array_to_c_hex(const uint32_t* ary, size_t sz, const char* name)
{
    size_t i = 0;

    printf("static const uint32_t %s[%d] = {\n    ", name, (int)sz);
    for (;;) {
        printf("0x%08x", ary[i]);
        ++i;
        if (i == sz)
            break;
        if (i % 6 == 0) {
            printf(",\n    ");
        } else {
            printf(", ");
        }
    }
    printf("\n};\n");
}

/**
 * prints char array as a c program snippet
 */
void char_array_to_c(const char* ary, size_t sz, const char* name)
{
    uint8_t tmp;
    size_t i = 0;

    printf("static const uint8_t %s[%d] = {\n    ", name, (int)sz);

    for (;;) {
        if (ary[i] == 0) {
            printf("'\\0'");
        } else if (ary[i] == '\n') {
            printf("'\\n'");
        } else if (ary[i] == '\t') {
            printf("'\\t'");
        } else if (ary[i] == '\r') {
            printf("'\\r'");
        } else if (ary[i] == '\'') {
            printf("'\\''");
        } else if (ary[i] == '\\') {
            printf("'\\\\'");
        } else if (ary[i] < 32 || ary[i] > 126) {
            tmp = (uint8_t)ary[i];
            printf("0x%02x", tmp);
        } else {
            printf("'%c'", (char)ary[i]);
        }
        ++i;
        if (i == sz)
            break;
        if (i % 10 == 0) {
            printf(",\n    ");
        } else {
            printf(", ");
        }
    }
    printf("\n};\n");
}

/**
 * prints an uint array as a c program snippet
 */
void uint32_array_to_c(const uint32_t* ary, size_t sz, const char* name)
{
    size_t i = 0;

    printf("static const uint32_t %s[%d] = {\n    ", name, (int)sz);
    for (;;) {
        printf("%u", ary[i]);
        ++i;
        if (i == sz)
            break;
        if (i % 12 == 0) {
            printf(",\n    ");
        } else {
            printf(", ");
        }
    }
    printf("\n};\n");
}
