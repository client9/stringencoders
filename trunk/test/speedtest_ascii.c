/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include "modp_ascii.h"
#include <stdio.h>
#include <time.h>
#include <stdint.h>
//#include "modp_ascii_data.h"
#include <ctype.h>

extern const char gsToUpperMap[256];
/**
 * This is standard clib implementation of uppercasing a string.
 * It has an unfair advantage since it's inside the test file
 * so the optimizing can inline it.
 */
void stdlib_toupper_copy(char* dest, const char* str, int len)
{
    int i;
    for (i = 0; i < len; ++i) {
        // toupper is defined in <ctype.h>
        *dest++ = toupper(str[i]);
    }
}

void modp_toupper_copy_a1(char* dest, const char* str, int len)
{
    int i=0;
    uint32_t eax,ebx,ecx,edx;
    const uint8_t* ustr = (const uint8_t*) str;
    const int leftover = len % 4;
    const int imax = len / 4;
    const uint32_t* s = (const uint32_t*) str;
    uint32_t* d = (uint32_t*) dest;
    for (i = 0; i != imax; ++i) {
        eax = s[i];
        ebx = 0x7f7f7f7f;
        edx = 0x7f7f7f7f;
        ebx = ebx & eax;
        ebx = ebx + 0x05050505;
        ecx = eax;
        ecx = ~ecx;
        ebx = ebx & edx;
        ebx = ebx + 0x1a1a1a1a;
        ebx = ebx & ecx;
        ebx = ebx >> 2;
        ebx = ebx & 0x20202020;
        eax = eax - ebx;
        *d++ = eax;
    }

    i = imax*4;
    dest = (char*) d;
    switch (leftover) {
        case 3: *dest++ = gsToUpperMap[ustr[i++]];
        case 2: *dest++ = gsToUpperMap[ustr[i++]];
        case 1: *dest++ = gsToUpperMap[ustr[i]];
        case 0: *dest = '\0';
    }
}

void modp_toupper_copy_a2(char* dest, const char* str, int len)
{
    int i=0;
    uint32_t eax,ebx,ecx;
    const uint8_t* ustr = (const uint8_t*) str;
    const int leftover = len % 4;
    const int imax = len / 4;
    const uint32_t* s = (const uint32_t*) str;
    uint32_t* d = (uint32_t*) dest;
    for (i = 0; i != imax; ++i) {
        eax = s[i];
        ecx = ~eax;
        ebx = (0x7f7f7f7ful & eax) + 0x05050505ul;
        ebx = (0x7f7f7f7ful & ebx) + 0x1a1a1a1aul;
        ebx = ((ebx & ecx) >> 2 ) & 0x20202020ul;
        eax = eax - ebx;
        *d++ = eax;
    }

    i = imax*4;
    dest = (char*) d;
    switch (leftover) {
        case 3: *dest++ = gsToUpperMap[ustr[i++]];
        case 2: *dest++ = gsToUpperMap[ustr[i++]];
        case 1: *dest++ = gsToUpperMap[ustr[i]];
        case 0: *dest = '\0';
    }
}

int main()
{
    double last = 0.0;
    size_t i = 0;
    char buf[256];
    char obuf[300];

    for (i = 0; i < 256; ++i) {
        buf[i] = (char)i;
    }

    uint32_t max = 1000000;
    clock_t t0, t1;
    printf("%s", "type\tclib\tmodp\timprovement\n");

    printf("toupper\t");
    fflush(stdout);

    /**
     ** CLIB
     **/
    t0 = clock();
    for (i = 0; i < max; ++i) {
        stdlib_toupper_copy(obuf, buf, sizeof(buf));
    }
    t1 = clock();
    last = t1 -t0;
    printf("%lu\t", (t1-t0));
    fflush(stdout);

    /**
     ** MODP
     **/
    t0 = clock();
    for (i = 0; i < max; ++i) {
        modp_toupper_copy_a1(obuf, buf, sizeof(buf));
    }
    t1 = clock();
    printf("%lu\t", (t1-t0));
    fflush(stdout);

    /**
     ** MODP
     **/
    t0 = clock();
    for (i = 0; i < max; ++i) {
        modp_toupper_copy_a2(obuf, buf, sizeof(buf));
    }
    t1 = clock();
    printf("%lu\t", (t1-t0));
    fflush(stdout);

    /**
     ** MODP
     **/
    t0 = clock();
    for (i = 0; i < max; ++i) {
        modp_toupper_copy(obuf, buf, sizeof(buf));
    }
    t1 = clock();
    printf("%lu\t", (t1-t0));
    fflush(stdout);


    printf("%.1fx\n", last/(t1-t0));
    fflush(stdout);

    return 0;
}
