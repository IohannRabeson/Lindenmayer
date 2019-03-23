#!/usr/bin/env bash

set -e

# Run unit tests
./builds/tests/unit-tests/unit-tests
# Run simple functional tests
./builds/app/Lindenmayer --platform offscreen --auto-test ./tests/lcode-examples
# Run stress tests
./builds/tests/stress-tests/stress-tests
