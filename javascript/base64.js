
base64 = {}
base64.PADCHAR = '=';
base64.ALPHA = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';

base64.getbyte64 = function(s,i) {
    var idx = base64.ALPHA.indexOf(s.charAt(i));
    if (idx == -1) {
	// do something
    }
    return idx;
}

base64.decode = function(s) {
    var getbyte64 = base64.getbyte64;
    var pads, i, b10;
    var imax = s.length
    if (imax == 0) {
	return s;
    }

    if (imax % 4 != 0) {
	// throw exception
    }

    pads = 0
    if (s.charAt(imax -1) == base64.PADCHAR) {
	pads = 1;
	if (s.charAt(imax -2) == base64.PADCHAR) {
	    pads = 2;
	}

	// either way, we want to ignore this last block
	imax -= 4;
    }

    var x = '';
    for (i = 0; i < imax; i += 4) {
	b10 = (getbyte64(s,i) << 18) | (getbyte64(s,i+1) << 12) |
	    (getbyte64(s,i+2) << 6) | getbyte64(s,i+3);
	x += String.fromCharCode(b10 >> 16, (b10 >> 8) & 0xff, b10 & 0xff);
    }

    if (pads == 2) {
	b10 = (getbyte64(s,i) << 18) | (getbyte64(s,i+1) << 12);
	x += String.fromCharCode(b10 >> 16);
    } else if (pads == 1) {
	b10 = (getbyte64(s,i) << 18) | (getbyte64(s,i+1) << 12) | (getbyte64(s,i+2) << 6)
	x += String.fromCharCode(b10 >> 16, (b10 >> 8) & 0xff);
    }

    return x;

}
base64.getbyte = function(s,i) {
    var x = s.charCodeAt(i);
    if (x > 255) {
	// throw exception
    }
    return x;
}

base64.encode = function(s) {
    var padchar = base64.PADCHAR;
    var alpha = base64.ALPHA;
    var getbyte = base64.getbyte;

    var i;
    var x = '';
    var b10;
    var imax = s.length - s.length % 3;

    if (s.length == 0) {
	return s;
    }
    for (i = 0; i < imax; i += 3) {
	b10 = (getbyte(s,i) << 16) | (getbyte(s,i+1) << 8) | getbyte(s,i+2);
	x += alpha.charAt(b10 >> 18) + alpha.charAt((b10 >> 12) & 0x3F) +
            alpha.charAt((b10 >> 6) & 0x3f) + alpha.charAt(b10 & 0x3f);
    }
    var leftover = s.length - imax;
    if (leftover == 2) {
	b10 = (getbyte(s,i) << 16) | (getbyte(s,i+1) << 8);
	x += alpha.charAt(b10 >> 18) + alpha.charAt((b10 >> 12) & 0x3F) +
            alpha.charAt((b10 >> 6) & 0x3f) + padchar;
    } else if (leftover == 1) {
	b10 = getbyte(s,i) << 16;
	x += alpha.charAt(b10 >> 18) + alpha.charAt((b10 >> 12) & 0x3F) +
            padchar + padchar;
    }
    return x;
}
