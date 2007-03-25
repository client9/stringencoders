AC_INIT(b64fast, [v3.0.0], [nickg -at- modp -dot- com])
AC_PREREQ(2.59)
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)
AC_CONFIG_SRCDIR(src)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_C_CONST
AC_C_BIGENDIAN
AC_TYPE_SIZE_T
AC_CHECK_FUNCS([memset, htonl, strlen])
AC_ARG_ENABLE(gcov, AC_HELP_STRING([--enable-gcov],[turn on code coverage analysis tools]))

EXTRACFLAGS="-Isrc -Icutest -Wall -Werror"

if test "x$enable_gcov" = "xyes";
then
    EXTRACFLAGS="$EXTRACFLAGS -fprofile-arcs -ftest-coverage"
    dnl Turn off optimization so code coverage tool
    dnl can get accurate line numbers
    CFLAGS=`echo "$EXTRACFLAGS" | sed -e 's/-O[0-9]*//g'`
fi
CFLAGS="$EXTRACFLAGS $CFLAGS"
CXXFLAGS="$CFLAGS"
AC_CONFIG_FILES([Makefile])
AC_PROG_MAKE_SET
AC_OUTPUT