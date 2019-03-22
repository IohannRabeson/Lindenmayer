#!/usr/bin/env bash

# Do not update brew
HOMEBREW_NO_AUTO_UPDATE=1

set -e

brew upgrade llvm qt ninja
brew install llvm qt ninja