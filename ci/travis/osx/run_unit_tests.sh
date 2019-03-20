#!/usr/bin/env bash

set -e

ASAN_OPTIONS=symbolize=1
ASAN_SYMBOLIZER_PATH="$(brew --prefix llvm)/bin/llvm-symbolizer"
TSAN_OPTIONS=symbolize=1
TSAN_SYMBOLIZER_PATH="$(brew --prefix llvm)/bin/llvm-symbolizer"
USAN_OPTIONS=symbolize=1
USAN_SYMBOLIZER_PATH="$(brew --prefix llvm)/bin/llvm-symbolizer"
# Run unit tests
./builds/tests/unit-tests/unit-tests

# Run simple functional tests
./builds/app/Lindenmayer.app/Contents/MacOS/Lindenmayer --auto-test ./tests/lcode-examples