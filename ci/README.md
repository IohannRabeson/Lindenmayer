# Continuous integration

## Linux and OSX
Both are very similar. The only difference is currently there are no
tests with sanitizers on Linux.
`setup.sh` installs everything needed to build the project.  
`build.sh` builds everything.
`run_unit_tests.sh` run tests.

### Tests
All test code is in the `tests` directory.  
We have unit tests which are fast. You can run them after each small modification if you want.  
We also have stress tests, but we need to improve them.  
On Travis with OSX we have 3 specials configurations: `ASAN`, `TSAN` and `USAN` respectively
for [address sanitizer](https://clang.llvm.org/docs/AddressSanitizer.html), [thread sanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html) and undefined behaviour sanitizer.  
You can use them locally too. Take a look to [/tests/travis/osx/run_unit_tests.sh](https://github.com/IohannRabeson/Lindenmayer/blob/master/ci/travis/osx/run_unit_tests.sh)
for an example.  

In these configurations flags to enable sanitizers are passed to the C++ compiler and the linker (this is handled by CMake).  
When tests run any issue detected by a sanitizer will stop the build with an error.