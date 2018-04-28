# Lindenmayer [![Build Status](https://travis-ci.com/IohannRabeson/Lindenmayer.svg?token=oSgYDG8ZHmxB1gxGNZxP&branch=master)](https://travis-ci.com/IohannRabeson/Lindenmayer)

Provide a tool with a dedicated language to describe and visualize Lindenmayer systems.

## Git and submodule
We use git submodules. Keep in mind to update submodules after
checkout and clone using `--recursive`.

```
git clone --recursive git@github.com:IohannRabeson/Lindenmayer.git
```

## Install dependencies
```bash
brew install qt
```

### Note
boost::optional (1.67.0) is directly embedded using bcp.

## Build

Require Qt 5 installed on your system.
Qt tool are also required.
Also a C++ compiler with sparse support for C++17 is required.

Qt directory should be added to `CMAKE_PREFIX_PATH`.

```bash
cmake <project_dir> -G <generator> -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
```

Tested generators:
 - GNU Make
 - Ninja
 - Xcode
 - MSVC 2015
 
### Windows with MSVC
On Windows don't forget to select a x64 generator, default or x86 will not works.
Also you will have some missing Qt DLL, so you have to update your PATH in Visual Studio:
Right click on the project "Lindenmayer" > Properties > Debugging > Environment then add a new entry like that:
PATH=$(PATH);$(QTDIR)\bin;

