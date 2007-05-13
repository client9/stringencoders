/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */
#include <stdio.h>
#include "arraytoc.h"

/** \brief make map of a byte to a string of 8 chars
 *
 *
 */
void binary_encodemap()
{
    static const unsigned char sBinaryChars[] = "01";
    int i, j;

    unsigned char e1[256];


    for (i = 0; i <= 256; ++i) {
        e1[i] = 0;
    }

    char buf[9];
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


    //char_array_to_c((char*)e1, 256, "gsHexEncodeC1");
    //char_array_to_c((char*)e2, 256, "gsHexEncodeC2");
}


// exact same thing as one used on urlencode
void hexdecodemap()
{
    int i;
    uint32_t map1[256];
    uint32_t map2[256];
    for (i = 0; i <= 255; ++i) {
        map1[i] = 256;
        map2[i] = 256;
    }

    // digits
    for (i = '0'; i <= '9'; ++i) {
        map1[i] = i - '0';
        map2[i] = map1[i] << 4;
    }

    // upper
    for (i = 'A'; i <= 'F'; ++i) {
        map1[i] = i - 'A' + 10;
        map2[i] = map1[i] << 4;
    }

    // lower
    for (i = 'a'; i <= 'f'; ++i) {
        map1[i] = i - 'a' + 10;
        map2[i] = map1[i] << 4;
    }


    uint32_array_to_c(map1, 256, "gsHexDecodeMap");

    uint32_array_to_c(map2, 256, "gsHexDecodeD2");
}

int main(int argc, char* argv[])
{
    binary_encodemap();

    //hexdecodemap();
    return 0;
}
