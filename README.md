# Lindenmayer

Provide a tool with a dedicated language to describe and visualize Lindenmayer systems.

## Git and submodule
We use git submodules. Keep in mind to update submodules after
checkout and clone using `--recursive`.

## Install dependencies
```bash
brew install qt boost
```
## Build
Require Qt 5 installed on your system.
Also a C++ compiler with support for C++17 is required.
For now I'm using `clang-900.0.39.2` on OSX.

Qt directory should be added to `CMAKE_PREFIX_PATH`.
```bash
cmake <project_dir> -G <generator> -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
```

Tested generators:
 - GNU Make
 - Ninja
 - Xcode
