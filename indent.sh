#!/bin/sh
dpkg-query -L clang-3.8

if [ -z "${INDENT}" ]; then
  INDENT=clang-format
fi
${INDENT} --version
find . -name '*.[ch]' | xargs ${INDENT} -i -style=WebKit
git status --porcelain

