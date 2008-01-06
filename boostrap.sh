#!/bin/sh

#
# script to regen the autotools
#  glibtoolize might be "libtoolize" on your system
#
set -x
aclocal
glibtoolize --force --copy
autoheader
automake --add-missing --copy
autoconf
