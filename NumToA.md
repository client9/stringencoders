# modp\_numtoa, fast number to string conversion #


A common programming task is converting a binary number into ascii string (char buffer).
modp\_numtoa introduces safe and fast method of doing this.

## The problem with sprintf ##

The standard way in "C" to convert a number to char buffer is to use `sprintf` (or `snprintf`).

### Integer Types ###

A common use of sprintf for a integer is:
```
int i;
char buf[32];
sprintf(buf,"%d", i);
```

The problem here is when `i` isn't an `int`.  if it changes, you'll be running to man page to learn that `%hhd` or "%lu" is the right format string.  Except some types, like `time_t` are different sizes  (either unsigned 32-bits or signed 64-bits), so you have to cast or use an `ifdef`.

The big problem is that using the wrong format string _can cause a core dump_.   Some compilers do a better job than others in catching the errors.

### Floating Point Types ###

Floating point types are even worse.  For instance

```
double d = 123456.0123456789;
char buf[64];
sprintf(buf, "%f", i);
```

would seem reasonable.  Until `d` is `1.1e200` and completely overruns your buffer.  You can use `snprintf` but you have to be careful in checking the return code.  And you can't totally limit the ouput size.  Only the number of digits to the _right_ of the decimal point, not to the left.

As an alternate, one can use the `%g` specifier which will switch between fixed and exponential format depending on the number of significant digits.
```
double d = 123456.0123456789;
char buf[64];
sprintf(buf, "%g", i);
```

This is _better_ but this format doesn't appear to be widely used.  Also the conversion to exponental format may occur in ways you don't want.

This biggest problem is that you don't want a _one character_ change to possibly cause memory corruption.

## modp\_numtoa ##

The modp stringencoders library introduces:

  * `modp_itoa` -- convert int32 (or smaller signed integer) to char buffer
  * `modp_uitoa` -- convert uint32 (or smaller unsigned integer) to char buffer
  * `modp_dtoa` -- convert double to char buffer, with maximum size


`modp_itoa` and `modp_uitoa` are simple: pass in the value and a char buffer (16 bytes or larger).

`modp_dtoa` is a bit more complex.  You also have to pass in precision, which corresponds the 'X' in "%.Xf" `printf` format specifier.  However, if the number is greater than 2 billion, or "very small" the format will switch to exponential format.  This insures the output will _never_ be greater than 32 bytes (include ending null characters).   For "regularly" sized values, the output is identical to printf.

### performance ###

Yum!

```
$ ./speedtest_numtoa 
    type        sprintf snprf   numtoa  improvement
unsigned 8      200000  210000  10000   21.0x
unsigned 16     190000  190000  30000   6.3x
unsigned 32     160000  190000  30000   6.3x
signed 8        190000  210000  20000   10.5x
signed 16       180000  210000  30000   7.0x
signed 32       170000  190000  40000   4.8x
double          1670000 1590000 110000  14.5x
```

Your millage may vary.


## Further Reading ##

This isn't the first or only way to solve the printf problem.

  * http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
  * [Efficient Integer to String Conversions, Part 1](http://www.ddj.com/dept/cpp/184401596)
  * [Efficient Integer to String Conversions, Part 2](http://www.ddj.com/dept/cpp/184403874)
  * [Efficient Integer to String Conversions, Part 3](http://www.ddj.com/dept/cpp/184403880)