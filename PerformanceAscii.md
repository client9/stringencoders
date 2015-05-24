## Performance Numbers ##

Intel Core i7 @ 2.66Ghz
```
$gcc -i
gcc version 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2335.15.00)
$ date
Thu Feb 23 06:33:53 EST 2012
$ ./speedtest_ascii 
type	clib	direct	map	para	hsieh1	hsieh2	Final	improvement
toupper	1573096	353679	247750	167802	131714	135453	141505	11.1x
```

Intel Core 2 Duo @ 2.2 GHz / Mac OS 10.5.1 / gcc 4.0.1

```
$ date
Thu Dec  6 09:50:17 EST 2007

$ ./speedtest_ascii
type	clib	direct	map	para	hsieh1	hsieh2	Final	improvement
toupper	3182920	384768	473305	436182	189438	186662	197372	16.1x
```

Intel(R) Xeon(R) CPU L5310  @ 1.60GHz / Linux / Gcc 3.4.5
```
$ date
Thu Jul  5 16:55:36 UTC 2007
./speedtest_ascii
type    clib    direct  map     para    hsieh1  hsieh2  Final   improvement
toupper 520000  690000  440000  340000  320000  320000  310000  1.7x
```

AMD / Linux / gLibc

```
$ date
Mon Jun  4 21:04:58 UTC 2007
$ ./speedtest_ascii
type    clib    direct  map     para    hsieh1  hsieh2  Final   improvement
toupper 590000  590000  580000  260000  190000  190000  230000  2.6x
```

Intel Core Duo / Mac OS X

```
$ date
Mon Jun  4 17:03:56 EDT 2007
$ ./speedtest_ascii
type    clib    direct  map     para    hsieh1  hsieh2  Final   improvement
toupper 467     81      85      66      33      32      33      14.2x
```

G4

Something is very wrong with the optimizer/compiler/os if we can get a 62.5x performance improvement.

```
$ date
Tue Jun  5 00:05:26 EDT 2007
$ ./speedtest_ascii
type    clib    direct  map     para    hsieh1  hsieh2  Final   improvement
toupper 3249    136     110     71      40      48      52      62.5x
```


## ToUpper Algorithms ##

### Sequential Ctypes toupper ###
A simple example is doing upper casing an ascii string.  A standard implementation might look like

```
void toupper_copy1(char* dest, const char* str, int len)
{
    int i;
    for (i = 0; i < len; ++i) {
        // toupper is defined in <ctype.h>                                                                                              
        *dest++ = toupper(str[i]);
    }
    *dest = 0;
}
```

The problem here is some clibs have crap versions of `toupper`.  They have to look up the 'C Locale' and use a complicated table and bit shifts.  A better version might be


### Sequential Direct Computation ###

```
void toupper_copy2(char* dest, const char* str, int len)
{
    int i;
    unsigned char c;
    for (i = 0; i < len; ++i) {
        c = str[i];
        *dest++ = (c >= 'a' && c <= 'z') ? c : (c -32);
    }
    *dest = 0;
}
```

One some platforms, this simple version is quite a bit faster already.


### Sequential Table Lookup ###

The problem here is that for each byte we have to do two comparisons and maybe a subtraction.  This can all be precomputed.

```
static char toupper_map = {0x00, 0x01, ... 'A' .. 'Z' ... 'A', .. 'Z'... 0xFF};

void toupper_copy3(char* dest, const char* str, int len)
{
    int i;
    for (i = 0; i < len; ++i) {
        *dest++ = toupper_map[(unsigned char)(str[i])];
    }
    *dest = 0;
}
```

This may or may not be faster still depending on your compiler or CPU.


### Parallel Table Lookup ###
However modern CPUs can have multiple registers and can either do multiple things at once or pipeline multiple request.  For instance

```
void toupper_copy4(char* dest, const char* str, int len)
    const int leftover = len % 4;
    const int imax = len - leftover;
    const uint8_t* s = (const uint8_t*) str;
    for (i = 0; i != imax ; i+=4) {
        /*                                                                                                                              
          it's important to make these variables                                                                                        
          it helps the optimizer to figure out what to do                                                                               
        */
        c1 = s[i], c2=s[i+1], c3=s[i+2], c4=s[i+3];
        dest[0] = gsToUpperMap[c1];
        dest[1] = gsToUpperMap[c2];
        dest[2] = gsToUpperMap[c3];
        dest[3] = gsToUpperMap[c4];
        dest += 4;
    }
    switch (leftover) {
    case 3: *dest++ = gsToUpperMap[s[i++]];
    case 2: *dest++ = gsToUpperMap[s[i++]];
    case 1: *dest++ = gsToUpperMap[s[i]];
    case 0: *dest = '\0';
    }
}
```

This does things in chunks of 4.  On AMD processors this is twice as fast, on Intel, _nine_ times faster, and on my old Powerbook G4, _44 times faster_.  For details see PerformanceAscii.


### The Hsieh Algorithm, Take 1 ###

The above trick works quite well, but all those char -> unsigned char -> int conversions and table looks takes time.  Even better is treat the string as if it were a array of integers, not chars.

```
 * Based code from Paul Hsieh
 * http://www.azillionmonkeys.com/qed/asmexample.html
 *
 * This was his "improved" version, but it appears to either run just
 * as fast, or a bit slower than his original version
 */
void toupper_copy5(char* dest, const char* str, int len)
{
    int i;
    uint32_t eax,ebx,ecx,edx;
    const uint8_t* ustr = (const uint8_t*) str;
    const int leftover = len % 4;
    const int imax = len / 4;
    const uint32_t* s = (const uint32_t*) str;
    uint32_t* d = (uint32_t*) dest;
    for (i = 0; i != imax; ++i) {
        eax = s[i];
        ebx = 0x80808080ul | eax;
        ecx = ebx - 0x61616161ul;
        edx = ~(ebx - 0x7b7b7b7bul);
        ebx = (ecx & edx) & (~eax & 0x80808080ul);
        *d++ = eax - (ebx >> 2);
    }

    i = imax*4;
    dest = (char*) d;
    switch (leftover) {
        case 3: *dest++ = gsToUpperMap[ustr[i++]];
        case 2: *dest++ = gsToUpperMap[ustr[i++]];
        case 1: *dest++ = gsToUpperMap[ustr[i]];
        case 0: *dest = '\0';
    }
}
```


On some platforms this is the fastest I've seen (but at this stage we are splitting hairs).

### The Hsieh Algorithm, Take 2 ###

Hsieh provided an earlier version of the toupper algorithm, but it's only as raw ASM source.  The most direct translation is

```
for (i = 0; i != imax; ++i) {
    eax = s[i];
    ebx = 0x7f7f7f7f;
    edx = 0x7f7f7f7f;
    ebx = ebx & eax;
    ebx = ebx + 0x05050505;
    ecx = eax;
    ecx = ~ecx;
    ebx = ebx & edx;
    ebx = ebx + 0x1a1a1a1a;
    ebx = ebx & ecx;
    ebx = ebx >> 2;
    ebx = ebx & 0x20202020;
    eax = eax - ebx;
    *d++ = eax;
}
```

### The Hsieh Algorithm, Take 3, Final ###

The "De-Asmed" version of the above algorithm is something like this.  On some platforms this is a bit slower than the previous.

```
for (i = 0; i != imax; ++i) {
    eax = s[i];
    ebx = (0x7f7f7f7ful & eax) + 0x05050505ul;
    ebx = (0x7f7f7f7ful & ebx) + 0x1a1a1a1aul;
    ebx = ((ebx & ~eax) >> 2 ) & 0x20202020ul;
    *d++ = eax - ebx;
}
```



# The Hsieh ToUpper Algorithm in Painful Detail #


It in it's raw C-form, the Hseih toupper algorithm is

```
uint32_t toupper(uint32_t eax)
{
    uint32_t ebx = (0x7f7f7f7ful & eax) + 0x05050505ul;
    ebx = (0x7f7f7f7ful & ebx) + 0x1a1a1a1aul;
    ebx = ((ebx & ~eax) >> 2 ) & 0x20202020ul;
    return eax - ebx;
}
```


Here's how it works. Lower case ascii characer are from 0x61 to 0x7A, and uppercase characters are from 0x41 to 0x5A, (a difference of 0x20).  For each octet in the uint32\_t, we want an intermediate result that is either 0x00 or 0x20 depending.  Then we can just subtract.  For instance if the input is 0x61614242 ("aaBB") we want 0x20200000, so 0x61614242 - 0x20200000 = 0x41414242 ("AABB").

1. Strip high bits of each octet (e.g. `(0x7f7f7f7ful & eax)` and then add 0x05 to each octet `+ 0x05050505ul;`.  The stripping of high bits is needed when we add 5, overflow doesn't effect the next octet.  The addition moves the range of lower case characters to 0x66 to 0x7F.

2. Restrip off the high bits.  This means anything that is "bigger" than "z" is removed, and then add 0x1a (26).  Now all lower case characters have the hight bit SET.  All non-lower case character do NOT.  This bit is all we care about now.

3. `ebx & ~eax` is a bit tricky.  All we care about in `~eax` is the high bit.  We could do `ebx & ~(eax & 0x80808080ul)` to make the intentions more clear.  Regardless, if the original input has the high bit set, then _clear_ our high bit since this is not a lower case character If the original does not have the high bit set, then all is ok.

4. Shift the high bits over by two places, i.e. 0x80 becomes 0x20.  Then clear out all the other bits.

5. We now have a integer that has octets of either 0x00 or 0x020. Subtract.

Ta-da.