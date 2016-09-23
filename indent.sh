#!/bin/sh
clang-format --version
find . -name '*.[ch]' | xargs clang-format -i -style=WebKit
git status --porcelain

