#!/bin/bash
set -x
set -e

INDENT=clang-format-3.8 ./indent.sh
git status --porcelain | wc -l

autoreconf --install --force
automake --add-missing >/dev/null 2>&1
./configure
make
make check

