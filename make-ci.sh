#!/bin/bash
set -x
set -e

autoreconf --install --force
automake --add-missing >/dev/null 2>&1
./configure
make
make test
