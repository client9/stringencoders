/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */

#include "modp_ascii.h"
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include <ctype.h>

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

int main()
{
    double last = 0.0;
    int i = 0;
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
		modp_toupper_copy(obuf, buf, sizeof(buf));
    }
    t1 = clock();
    printf("%lu\t", (t1-t0));
	fflush(stdout);


	printf("%.1fx\n", last/(t1-t0));
	fflush(stdout);

    return 0;
}
