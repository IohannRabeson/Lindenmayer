#!/usr/bin/env bash

set -e

brew update
brew install qt ninja boost

QT_PATH="/usr/local/opt/qt"

mkdir builds
cd builds
cmake .. -G Ninja -DCMAKE_PREFIX_PATH="$QT_PATH" -DCMAKE_BUILD_TYPE="$TRAVIS_CONFIGURATION_NAME"
cmake --build ./ --clean-first --target all
