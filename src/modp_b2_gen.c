#include "arraytoc.h"
#include <stdio.h>

/** \brief make map of a byte to a string of 8 chars
 *
 *
 */
static void binary_encodemap(void)
{
    static const uint8_t sBinaryChars[] = "01";
    int i, j;
    uint8_t buf[9];

    printf("%s", "static const char* modp_b2_encodemap[] = {\n");
    buf[8] = 0;
    for (i = 0; i < 256; ++i) {
        for (j = 0; j < 8; ++j) {
            buf[j] = sBinaryChars[(i >> (7 - j)) & 1];
        }
        printf("\"%s\"", buf);
        if (i != 255) {
            printf("%s", ", ");
        }
        if ((i + 1) % 6 == 0) {
            printf("%s", "\n");
        }
    }
    printf("%s", "};\n");
}

int main(void)
{
    binary_encodemap();

    return 0;
}
