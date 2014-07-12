#!/usr/bin/env python

def intarray_to_c(ary, name):
    parts = []
    parts.append("static const size_t {0}[{1}] = {{".format(name,len(ary)))
    count = 0
    for val in ary:
        if (count % 10 == 0):
            parts.append("\n  ")
        parts.append("{0}, ".format(val))
        count += 1
    parts.append("\n};\n")
    return ''.join(parts)

def chararray_to_c(ary, name):
    parts = []
    parts.append("static const uint8_t {0}[{1}] = {{".format(name,
                                                             len(ary)))
    count = 0;
    for val in ary:
        if (count % 10 == 0):
            parts.append("\n ")
        parts.append("{0:>6}".format("'" + val + "',"))
        count += 1

    parts.append("\n};\n")
    return ''.join(parts)

def json_encodelen_map():
    lenmap = [ 6 for i in range(256) ]
    for i in range(32,128):
        lenmap[i] = 1
    for i in [0x08, 0x09, 0x0a,  0x0c, 0x0d, 0x5c, 0x22]:
        lenmap[i] = 2
    return lenmap

def json_encode_map():
    encodemap = [ 'u' for i in range(256) ]

    # things to quad-hex encode  \u00XY
    for i in range(32,128):
        encodemap[i] = 'a';

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

    with open('modp_json_data.h', 'w') as fd:
        fd.write(chararray_to_c(json_encode_map(), "gsJSONEncodeMap"))
        fd.write("\n")
        fd.write(intarray_to_c(json_encodelen_map(), "gsJSONEncodeLenMap"))
