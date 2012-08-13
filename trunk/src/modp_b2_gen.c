/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */
#include <stdio.h>
#include "arraytoc.h"

/** \brief make map of a byte to a string of 8 chars
 *
 *
 */
static void binary_encodemap(void)
{
    static const unsigned char sBinaryChars[] = "01";
    int i, j;

    printf("%s","static const char* modp_b2_encodemap[] = {\n");
    unsigned char buf[9];
    buf[8] = 0;
    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 8; ++j) {
            buf[j] = sBinaryChars[(i >> (7-j)) & 1];
        }
        printf("\"%s\"",  buf);
        if (i != 255) {
            printf("%s", ", ");
        }
        if ((i+1) % 6 == 0) {
            printf("%s", "\n");
        }
    }
    printf("%s", "};\n");
}

//int main(int argc, char* argv[])
int main(void)
{
    binary_encodemap();

    //hexdecodemap();
    return 0;
}
