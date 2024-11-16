# Unicode Library

By Ross Smith

_[GitHub repository](https://github.com/CaptainCrowbar/rs-unicode)_

## Overview

```c++
#include "rs-unicode.hpp"
namespace RS::Unicode;
```

## Introduction

This is my new C++ Unicode library.

The library is designed on the assumption that text processing will normally
be done entirely with known-valid UTF-8 text, with unvalidated text only
being encountered during input sanitization.

My original library (Unicorn) hasn't been updated for many years; there have
been enough changes to the Unicode standard since then that it will not work
with a significant fraction of existing Unicode text (in particular, anything
that uses emoji). Along with the library's size (containing many features I
no longer consider worthwhile), and some other design decisions I wanted to
revisit, I decided that writing a new library (with some code imported form
the old one) was the best approach at this point.

## Index of headers

* [`rs-unicode/character` -- Character properties](character.html)
* [`rs-unicode/encoding` -- Character encodings](encoding.html)
* [`rs-unicode/legacy` -- Legacy encodings](legacy.html)
* [`rs-unicode/regex` -- Regular expressions](regex.html)
* [`rs-unicode/string` -- String manipulation](string.html)
* [`rs-unicode/version` -- Version information](version.html)

## Using the library

There is a `CMakeLists.txt` file that can build and install the library using
the usual [CMake](https://cmake.org) conventions. Command line usage will
typically look like this:

```bash
cd wherever/you/installed/rs-unicode
mkdir build
cd build
cmake -G "Unix Makefiles" ../src
    # or cmake -G "Visual Studio 17 2022" ../src on Windows
cmake --build . --config Release -- -j<N>
    # where <N> is your CPU core count
cmake --build . --config Release --target install'
```

The library's public headers are listed above (other headers are for internal
use only and should not be included by your code). To use the library,
`#incldue` either the individual headers you want, or `rs-unicode.hpp` to
include all of them.

Link your build with `-lrs-unicode.`
