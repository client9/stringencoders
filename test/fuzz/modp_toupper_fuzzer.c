#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "modp_ascii.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
        if(size<3){
                return 0;
        }

        char *new_str = (char *)malloc(size+1);
        if (new_str == NULL){
                return 0;
        }
        memcpy(new_str, data, size);
        new_str[size] = '\0';

        modp_toupper(new_str, size);

        free(new_str);
        return 1;
}
