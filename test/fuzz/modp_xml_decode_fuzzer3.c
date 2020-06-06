#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "modp_xml.h"


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
        if(size<3){
                return 0;
        }

        size_t d = 0;
        char buf[size+1];
        d = modp_xml_decode(buf, (char*)data, size);
        return 1;
}
