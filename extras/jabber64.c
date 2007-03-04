#include "jabber64.h"
#include <string.h>
/* --------------------------------------------------------------------------
 *
 * License
 *
 * The contents of this file are subject to the Jabber Open Source License
 * Version 1.0 (the "JOSL").  You may not copy or use this file, in either
 * source code or executable form, except in compliance with the JOSL. You
 * may obtain a copy of the JOSL at http://www.jabber.org/ or at
 * http://www.opensource.org/.  
 *
 * Software distributed under the JOSL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied.  See the JOSL
 * for the specific language governing rights and limitations under the
 * JOSL.
 *
 * Copyrights
 * 
 * Portions created by or assigned to Jabber.com, Inc. are 
 * Copyright (c) 1999-2002 Jabber.com, Inc.  All Rights Reserved.  Contact
 * information for Jabber.com, Inc. is available at http://www.jabber.com/.
 *
 * Portions Copyright (c) 1998-1999 Jeremie Miller.
 * 
 * Acknowledgements
 * 
 * Special thanks to the Jabber Open Source Contributors for their
 * suggestions and support of Jabber.
 * 
 * Alternatively, the contents of this file may be used under the terms of the
 * GNU General Public License Version 2 or later (the "GPL"), in which case
 * the provisions of the GPL are applicable instead of those above.  If you
 * wish to allow use of your version of this file only under the terms of the
 * GPL and not to allow others to use your version of this file under the JOSL,
 * indicate your decision by deleting the provisions above and replace them
 * with the notice and other provisions required by the GPL.  If you do not
 * delete the provisions above, a recipient may use your version of this file
 * under either the JOSL or the GPL. 
 * 
 * --------------------------------------------------------------------------*/


/**
 * @file base64.c
 * @brief Functions to handle Base64 encoding and decoding
 */

/**
 * characters used for Base64 encoding
 */
const char *BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * encode three bytes using base64 (RFC 3548)
 *
 * @param triple three bytes that should be encoded
 * @param result buffer of four characters where the result is stored
 */
void _base64_encode_triple(unsigned char triple[3], char result[4])
{
    int tripleValue, i;

    tripleValue = triple[0];
    tripleValue *= 256;
    tripleValue += triple[1];
    tripleValue *= 256;
    tripleValue += triple[2];

    for (i=0; i<4; i++)
    {
	result[3-i] = BASE64_CHARS[tripleValue%64];
	tripleValue /= 64;
    }
}

/**
 * encode an array of bytes using Base64 (RFC 3548)
 *
 * @param source the source buffer
 * @param sourcelen the length of the source buffer
 * @param target the target buffer
 * @param targetlen the length of the target buffer
 * @return 1 on success, 0 otherwise
 */
int jabber64_encode(unsigned char *source, size_t sourcelen, char *target, size_t targetlen)
{
    /* check if the result will fit in the target buffer */
    if ((sourcelen+2)/3*4 > targetlen-1)
	return 0;

    /* encode all full triples */
    while (sourcelen >= 3)
    {
	_base64_encode_triple(source, target);
	sourcelen -= 3;
	source += 3;
	target += 4;
    }

    /* encode the last one or two characters */
    if (sourcelen > 0)
    {
	unsigned char temp[3];
	memset(temp, 0, sizeof(temp));
	memcpy(temp, source, sourcelen);
	_base64_encode_triple(temp, target);
	target[3] = '=';
	if (sourcelen == 1)
	    target[2] = '=';

	target += 4;
    }

    /* terminate the string */
    target[0] = 0;

    return 1;
}

/**
 * decode a base64 string and put the result in the same buffer as the source
 *
 * This function does not handle decoded data that contains the null byte
 * very well as the size of the decoded data is not returned.
 *
 * The result will be zero terminated.
 *
 * @deprecated use base64_decode instead
 *
 * @param str buffer for the source and the result
 */
void str_b64decode(char* str) {
    size_t decoded_length;

    decoded_length = jabber64_decode(str, (unsigned char*) str, strlen(str));
    str[decoded_length] = '\0';
}

/**
 * decode base64 encoded data
 *
 * @param source the encoded data (zero terminated)
 * @param target pointer to the target buffer
 * @param targetlen length of the target buffer
 * @return length of converted data on success, -1 otherwise
 */
size_t jabber64_decode(char *source, unsigned char *target, size_t targetlen) {
    char *cur;
    unsigned char *dest, *max_dest;
    int d, dlast, phase;
    unsigned char c;
    static int table[256] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
    };

    d = dlast = phase = 0;
    dest = target;
    max_dest = dest+targetlen;

    for (cur = source; *cur != '\0' && dest<max_dest; ++cur ) {
        d = table[(int)*cur];
        if(d != -1) {
            switch(phase) {
		case 0:
		    ++phase;
		    break;
		case 1:
		    c = ((dlast << 2) | ((d & 0x30) >> 4));
		    *dest++ = c;
		    ++phase;
		    break;
		case 2:
		    c = (((dlast & 0xf) << 4) | ((d & 0x3c) >> 2));
		    *dest++ = c;
		    ++phase;
		    break;
		case 3:
		    c = (((dlast & 0x03 ) << 6) | d);
		    *dest++ = c;
		    phase = 0;
		    break;
	    }
            dlast = d;
        }
    }

    /* we decoded the whole buffer */
    if (*cur == '\0') {
	return dest-target;
    }

    /* we did not convert the whole data, buffer was to small */
    return (size_t)-1;
}


