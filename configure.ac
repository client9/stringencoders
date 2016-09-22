AC_INIT(stringencoders, [v3.10.3], [nickg -at- client9 -dot- com])
AC_PREREQ(2.68)
AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_C_CONST
AC_C_BIGENDIAN
AC_TYPE_SIZE_T
AC_CHECK_FUNCS([memset htonl strlen])
AC_ARG_ENABLE(gcov, AC_HELP_STRING([--enable-gcov],[turn on code coverage analysis tools]))

B64WCHARS="-_."
AC_ARG_WITH([b64wchars], AC_HELP_STRING([--with-b64wchars=3CHARS],[change b64 web extra chars, default is '-_.']),
          [B64WCHARS=$withval], [])

EXTRACFLAGS="-Isrc -Wall -Wextra -Werror -Wshadow -Wpointer-arith -Wcast-qual -Wconversion"
# -Wstrict-prototypes
# -Wmissing-prototypes

if test "x$enable_gcov" = "xyes";
then
    EXTRACFLAGS="$EXTRACFLAGS -fprofile-arcs -ftest-coverage"
    dnl Turn off optimization so code coverage tool
    dnl can get accurate line numbers
    CFLAGS=`echo "$EXTRACFLAGS" | sed -e 's/-O[0-9]*//g'`
    CFLAGS="$CFLAGS -O0 -fno-inline"
fi
CFLAGS="$EXTRACFLAGS $CFLAGS"
CXXFLAGS="$CFLAGS"
AC_SUBST(B64WCHARS)
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	test/Makefile
	src/stringencoders.pc
])
AC_PROG_MAKE_SET
AC_OUTPUT
