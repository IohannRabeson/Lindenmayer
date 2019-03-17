#!/usr/bin/env bash

set -e

LLVM_PATH="$(brew --prefix llvm)"
CC="$LLVM_PATH/clang"
CXX="$LLVM_PATH/clang++"

mkdir builds && cd builds
cmake .. -G Ninja -DCMAKE_PREFIX_PATH="$(brew --prefix qt)" -DCMAKE_BUILD_TYPE="$TRAVIS_CONFIGURATION_NAME"
cmake --build ./ --clean-first --target all
