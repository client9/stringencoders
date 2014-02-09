#!/usr/bin/env python

def chararray_to_c(ary, name):
    parts = []
    parts.append("static const unsigned char {0}[{1}] = {{".format(name,
                                                                   len(ary)))
    count = 0;
    for val in ary:
        if (count % 10 == 0):
            parts.append("\n ")
        parts.append("{0:>7}".format("'" + val + "',"))
        count += 1

    parts.append("\n};\n")
    return ''.join(parts)

def json_encode_map():
    encodemap = [ "\\0" for i in range(256) ]

    # things to quad-hex encode  \u00XY
    for i in range(32):
        encodemap[i] = 'A';

    # special escapes
    encodemap[0x08] = 'b'
    encodemap[0x09] = 't'
    encodemap[0x0a] = 'n'
    encodemap[0x0c] = 'f'
    encodemap[0x0d] = 'r'
    encodemap[0x5c] = "\\\\"
    encodemap[0x22] = "\""

    return encodemap

if __name__ == '__main__':

    with open('modp_json_data_h', 'w') as fd:
        fd.write(chararray_to_c(json_encode_map(), "gsJSONEncodeMap"))
