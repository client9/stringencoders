#!/bin/sh

find . -name '*.[ch]' | xargs clang-format -i -style=WebKit
