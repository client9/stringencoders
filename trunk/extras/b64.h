#include <stdlib.h>

/**
 *
 * Why are there so many versions?
 * When working on groups of chars, differnt compilers and CPU behave
 * quite differently (sometimes in ways you wouldn't expect).  In addition
 * 32 and 64 bit versions are included.
 *
 * If you don't want to go through the hassle of benchmarking, your best
 * best as of 12/2005 is to use
 *
 * b64_encode32_2
 * b64_decode32_4
 */

/**
 * Encoding
 */

/**
 * A somewhat simple version that processes a byte at a time.
 * See source for more comments.
 *
 * No 64-bit version since this doesn't really use 32/64-bit types.
 */
int b64_encode32_0(char* dest, const char* src, int len);

/**
 * Encode: Version 1
 * Output uses chars, while input is aliased as integers
 */
int b64_encode32_1(char* dest, const char* src, int len);
int b64_encode64_1(char* dest, const char* src, int len);


/**
 * Encoding Version 2
 * Full Aliasing of input and output as integers.
 * Shifts (<<) are replaced by table lookups.
 * 
 * Ridiculously fast on AMD and PowerPC G4
 */
int b64_encode32_2(char* dest, const char* src, int len);
int b64_encode64_2(char* dest, const char* src, int len);


/**
 * Encoding Version 3
 * Full Aliasing of input and output as integers.
 * Shifts (<<) are replaced by table lookups.
 * 
 * Ridiculously fast on AMD and PowerPC G4
 */
int b64_encode32_3(char* dest, const char* src, int len);


/**
 * BASE64 DECODE: Version 1
 * First attempt at aliasing char* as int*
 * 
 *
 */
int b64_decode32_1(char* dest, const char* src, int len);
int b64_decode64_1(char* dest, const char* src, int len);

/**
 *
 * Similar to above except loops are manually unrolled.
 * Version 2 is always faster than Version 1
 *
 */
int b64_decode32_2(char* dest, const char* src, int len);
int b64_decode64_2(char* dest, const char* src, int len);

/**
 * Version 3 -- "deferred exit".
 *  Similar to version 2, except on bad input, the rest of the 
 *  string is processed, and then exits with an error.
 *
 *  Fastest on: PowerPC G4 (gcc 4.0.2)
 */
int b64_decode32_3(char* dest, const char* src, int len);
int b64_decode64_3(char* dest, const char* src, int len);

/**
 * Version 4:
 * Instead of checking each input character for validity, this
 * version checks 4 characters at a times (and if invalid, exits).
 * 
 * AMD Opteron: 64-bit is best version
 * PowerPC g4 : Oddly slower than version 3, but faster than version 2
 */
int b64_decode32_4(char* dest, const char* src, int len);
int b64_decode64_4(char* dest, const char* src, int len);

/**
 * Version 5: combines "deferred errors" with "parallel error check"
 * Sometimes a percent slower, sometimes a bit faster.  Overall:
 *  not worth it.
 */
int b64_decode32_5(char* dest, const char* src, int len);
int b64_decode64_5(char* dest, const char* src, int len);
