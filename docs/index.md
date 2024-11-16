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

## Index

* `[rs-unicode/character](character.html)` -- Character properties
* `[rs-unicode/encoding](encoding.html)` -- Character encodings
* `[rs-unicode/legacy](legacy.html)` -- Legacy encodings
* `[rs-unicode/regex](regex.html)` -- Regular expressions
* `[rs-unicode/string](string.html)` -- String manipulation
* `[rs-unicode/version](version.html)` -- Version information
