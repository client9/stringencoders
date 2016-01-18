#!/bin/bash
autoreconf --install --force
automake --add-missing >/dev/null 2>&1
./configure && make && make test
