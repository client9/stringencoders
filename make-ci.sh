#!/bin/bash
set -x
set -e

./indent.sh
git status --porcelain | wc -l

autoreconf --install --force
automake --add-missing >/dev/null 2>&1
./configure
make
make check

