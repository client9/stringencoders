A collection of high performance c-string transformations, frequently 2x faster than standard implementations (if they exist at all).

including

  * base64, standard
  * base64, [web/url safe](WebSafeBase64.md), with configurable alphabet
  * base85 (good for http cookies)
  * base16 (hex)
  * base2 (ascii binary)
  * url escaping
  * javascript string escaping
  * fast number to string conversion, [4-22x faster](PerformanceNumToA.md) than sprintf!
  * fast ascii upper/lower case conversion, [2-66x faster](PerformanceAscii.md) than stdlib! (yes, 66x faster)

And it's all wrapped up in a bow for you to use:

  * BSD License -- do what you want with it.
  * Standard _clean_ ANSI C,  will also compile as C99 and C++
  * C++ wrappers for `std::string`
  * Standard install: `configure && make && make install`
  * Extensive unit tests provided [with >98% coverage.](http://client9.com/projects/stringencoders/lcov-html/stringencoders/src/index.html)
  * Endian safe.
  * Performance test framework -- don't take my word, run it your self
  * Lots of in-code [documentation](http://client9.com/projects/stringencoders/doxy/html/)

## HEY ##

  * This code might compile on windows. I tried to work around the lack of stdint and stdbool, but I can't test it out.
  * Also this code won't work on Sparc chips (Solaris) due to alignment issues.  not hard to fix, but I don't have access to sparc machines
  * Works great on Mac and Linux.
  * No idea about iOS and ARM chips.

## News ##

23-Feb-2012: I'm back
  * Hey gang, I'm back and looking at bugs and getting this cleaned up.  Stay tuned for details.

30-Sep-2010: modp\_numtoa.[c|h] changed to MIT
  * By request, I've changed the license on modp\_numtoa code from BSD to MIT.  Go have fun kids!

04-Apr-2010: 3.10.3 released
  * The only change is autoconf was regenerated (again) to work on more platforms.  I think I found the magic combo this time.

30-March-2010: On the interwebs!
  * As seen on [Reddit](http://www.reddit.com/r/programming/comments/bja8c/stringencoders_a_collection_of_high_performance/)  (Thanks [Steve](http://www.exit2shell.com/~skreuzer/)).
19-Mar-2010: 3.10.0 release
  * Security fix: modp\_dota did not handle NaN correct, and **may** have a buffer overflow problem (certainly produced junk as output)..  Added unit tests to specifically target this issue.
  * New function:  Thanks to IPonWeb:  modp\_dtoa2 converts floating point numbers to a string, but without trailing zeros.   It's a nice addition since this type of function doesn't exist in the printf-family.
  * Minor fixes to unit test source code to play nice on 64-bit machines (sprintf and type problems).
  * Redid autoconf again.  Mac OS X will complain but should work.  Tested on Ubuntu 9.10 both 32 and 64 bit. Please file a ticket if "./configure" breaks.

12-Feb-2010: 3.9.0 release
  * Security fix for itoa type functions + matching unit tests [Issue 7](http://code.google.com/p/stringencoders/issues/detail?id=7&can=1).  Thanks to [simhasana](http://code.google.com/u/simhasana/).
  * Fix autoconf annoyances ([Issue 8](http://code.google.com/p/stringencoders/issues/detail?id=8&can=1), thanks [0xcafefeed](http://code.google.com/u/0xcafefeed/))
  * And a probably failed attempt to make VS2008 happy ([Issue 6](http://code.google.com/p/stringencoders/issues/detail?id=6&can=1), thanks to [AlienPenguin](http://code.google.com/u/AlienPenguin/))

27-Oct-2009: 3.8.0 released
  * Various fixes for compiler warnings using strict flags.  Checked with gcc 4.4
  * Adding various "num to alpha" 64-bit integer functions.

03-Dec-2008:
  * Looks like [Google Gears and Chrome uses the modp\_b64 functions](http://blog.modp.com/2008/11/googles-chrome-uses-stringencoders.html).

> [More News](http://stringencoders.googlecode.com/svn/trunk/ChangeLog)

If you use this library, be sure to join the group (listed in the right column) to be informed of all the latest goodies.

## Why ##

Ok, so why? Besides providing a standard interface to common operations, they provide up to 3.5x performance improvement over standard sane versions, and up 50x improvement over numerous brain-dead versions floating around on the web. Your results may vary. It's highly processor dependent, but here's a sample on run an 2.2Ghz AMD system.

From the built-in performance test:
```
AMD 2.2Ghz, Linux 2.6.15, gcc -O3 (3.4.4)

Message size = 20
        modpb64 apache  improvement     modpb85 modpurl modp_b16
Encode    0.04    0.10    2.50x           0.09    0.05    0.04
Decode    0.04    0.13    3.25x           0.07    0.06    0.05

Message size = 200
        modpb64 apache  improvement     modpb85 modpurl modp_b16
Encode    0.43    0.99    2.30x           0.83    0.47    0.33
Decode    0.34    1.10    3.24x           0.60    0.46    0.41

Message size = 2000
        modpb64 apache  improvement     modpb85 modpurl modp_b16
Encode    4.02    9.45    2.35x           7.98    4.66    3.09
Decode    3.18   10.77    3.39x           5.94    4.54    3.93

Tested 16-May-2006
```


## How It Works ##

The main trick with these functions is that they operate on 32-bits at a time and do crazy bit operations instead of regular algorithms that operate with 8-bits at a time. So for instance a standard implementation might do something like

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

```
void modp_toupper_copy(char* dest, const char* str, int len)
{
    int i;
    uint32_t eax, ebx;
    const uint8_t* ustr = (const uint8_t*) str;
    const int leftover = len % 4;
    const int imax = len / 4;
    const uint32_t* s = (const uint32_t*) str;
    uint32_t* d = (uint32_t*) dest;
    for (i = 0; i != imax; ++i) {
        eax = s[i];
        /*
         * This is based on the algorithm by Paul Hsieh
         * http://www.azillionmonkeys.com/qed/asmexample.html
         */
        ebx = (0x7f7f7f7ful & eax) + 0x05050505ul;
        ebx = (0x7f7f7f7ful & ebx) + 0x1a1a1a1aul;
        ebx = ((ebx & ~eax) >> 2)  & 0x20202020ul;
        *d++ = eax - ebx;
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

Here:
  * the string is treated as a array of uint32\_t _not_ chars
  * The integers are manipulated, effectively doing toupper 4 at a time
  * oddballs are are computed using a table, not stdlib calls, or by doing math


For base 2, 16, 65 and 85 even more tricks are used, such as using precomputed shift tables. Take a look at modp\_b64\_gen to see how this works.

It is probable that a lot of these tricks could be used in Java. I don't program in Java that much any more, but go nuts.

## That said.... ##

Unless your application is doing almost nothing except encoding and decoding, the odds are that these functions will not improve the total performance of your application. Even with slow versions of base64 it's already so fast it's probably not measurable in your application.

Also this is _highly_ compiler version and cpu model dependent.  While it _screams_ on AMD processors and is 'better' on Intel processors, it may not do as well on _your_ platform.  Please test (and send me the results from `./speedtest`)