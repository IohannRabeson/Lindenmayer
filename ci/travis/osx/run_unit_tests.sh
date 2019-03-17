#!/usr/bin/env bash

set -e

# Run unit tests
./builds/tests/unit-tests/unit-tests

# Run simple functional tests
./builds/app/Lindenmayer.app/Contents/MacOS/Lindenmayer --auto-test ./tests/lcode-examples