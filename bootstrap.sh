#!/bin/sh
autoreconf --install --force
automake --add-missing >/dev/null 2>&1
