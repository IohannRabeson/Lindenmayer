#!/usr/bin/env bash

set -e

LLVM_PATH="$(brew --prefix llvm)/bin"

mkdir builds && cd builds
cmake .. -G Ninja -DCMAKE_PREFIX_PATH="$(brew --prefix qt)" -DCMAKE_BUILD_TYPE="$TRAVIS_CONFIGURATION_NAME" -DCMAKE_C_COMPILER="$LLVM_PATH/clang" -DCMAKE_CXX_COMPILER="$LLVM_PATH/clang++"
cmake --build ./ --clean-first --target all
