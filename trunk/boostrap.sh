#!/bin/sh

#
# script to regen the autotools
#  glibtoolize might be "libtoolize" on your system
#
set -x
aclocal

# test for MacPorts version first
#  Mac version of libtool is bizarro
if [ "`which glibtoolize`" != "" ]; then
   LIBTOOLIZE=glibtoolize
elif [ "`which libtool`" != "" ]; then
   LIBTOOLIZE=libtoolize
else
   echo "Perhap you need to install libtool?"
   exit 1
fi
echo "Using ${LIBTOOLIZE}..."

${LIBTOOLIZE} --force --copy
autoheader
automake --add-missing --copy
autoconf
