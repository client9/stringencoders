This was original based from research for [Cryptography for Internet and Database Applications](http://code.google.com/p/cida/)

## Base 64 Standard ##

Base 64 encoding converts 3 binary bytes into 4 ascii characters.  The base 64 "alphabet" or "digits" are A-Z,a-z,0-9 plus two extra characters `+` (plus) and `/` (slash).  It also defines one extra character `=` for padding if the original input was not a multiple of 3.


## Problems with the standard in urls ##

Unfortunately, the choice of the extra characters clashes with the http/html specifications.  For URLs and form-posts the mime type of `www-form-urlencoded` is used and defines certain transformations:

  * The `+` may be decoded as a space ' '
  * The `/` will be encoded with `%2f`
  * The `=` may be encoded with `%3d`
  * While `.`, `-`, `*` and `_` are safe and not touched.

While using `.` in the spec is safe, some proxies and servers may
misinterpret this as a filename reference (this is not so common anymore in 2007). Likewise I've seen some implementations use a tilde `~`, but again this is a special character used in user directory mappings and is not safe according to spec.

So to convert the base64 alpha into a format that is "web-safe" the following changes are recommended:

  * `+` --> `-` (char 62, plus to dash)
  * `/` --> `_` (char 63, slash to underscore)
  * `=` --> `*` padding

However this is not standardized.

## stringencoders to the rescue ##

You could use a standard base64 encoder and then post-process the output to convert the bad characters to ones that are web safe.  However the `stringencoders` library has separate functions to do this for you with no loss in performance.  The `modp_b64_xxx` functions are _standard_ base 64, while the the `modp_b65w_xx` (note the "w") are the web-safe versions.


You can change what characters are used at compile time.  The default is the equivalent of

```
./configure --with-b64w-chars='-_\*'
```

Other popular variations are

```
./configure --with-b64w-chars='-_.'
./configure --with-b64w-chars='_-.'
```

If you are integrating with a third party source, **be sure to check with them on the alphabet**.  If you only create and process the urls, the default is fine.


**ALWAYS TEST**

## RFC 4648 ##

To my amazement, in 2012, after doing string encoders for 5+ years, I just found out about

http://www.ietf.org/rfc/rfc4648.txt

which defines a "Base 64 Encoding with URL and Filename Safe Alphabet" and uses "-" (dash), "_" (underscore) and still uses "=" for the padding char._

```
./configure --with-b64w-chars='-_='
```

I'll be adding a modp\_b64r (for rfc) version shortly so you don't have to do this.