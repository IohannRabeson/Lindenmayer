#!/usr/bin/env bash

set -e

# Address sanitizer options
ASAN_OPTIONS=symbolize=1
ASAN_SYMBOLIZER_PATH="$(brew --prefix llvm)/bin/llvm-symbolizer"
ASAN_OPTIONS=check_initialization_order=1

# Thread sanitizer options
TSAN_OPTIONS=symbolize=1
TSAN_SYMBOLIZER_PATH="$(brew --prefix llvm)/bin/llvm-symbolizer"

# Undefined behavior sanitizer options
USAN_OPTIONS=symbolize=1
USAN_SYMBOLIZER_PATH="$(brew --prefix llvm)/bin/llvm-symbolizer"

# Run unit tests
./builds/tests/unit-tests/unit-tests

# Run simple functional tests
./builds/app/Lindenmayer.app/Contents/MacOS/Lindenmayer --auto-test ./tests/lcode-examples

# Run stress tests
./builds/tests/stress-tests/stress-tests