# MidiMonitor
MIDI monitor made to discover in depth MIDI protocol.
Actually it support the following MIDI message types:
 - Note On
 - Note Off
 - Control change
 - Program change
 - System exclusive

## Git and submodule
We use git submodules. Keep in mind to update submodules after
checkout and clone using `--recursive`.

## Build
Require Qt 5 and RtMidi installed on your system.
Also a C++ compiler with support for C++17 is required.
For now I'm using `clang-900.0.39.2` on OSX.

Qt and RtMidi paths should be added to `CMAKE_PREFIX_PATH`.
```bash
cmake <project_dir> -G <generator> -DCMAKE_PREFIX_PATH="<qt_dir>/5.9.1/clang_64"
```
