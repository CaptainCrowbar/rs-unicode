# Formatting Helpers

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/format.hpp"
namespace RS::Unicode;
```

## Contents

* TOC
{:toc}

## Standard formatters

```c++
template <typename T> struct std::formatter<T>;
```

If this header is included, a formatter is automatically generated for any
type for which a function exists with the signature:

```c++
[void] rs_unicode_format(T);
```

The function need not be defined; the existence of the declaration is enough.
The return type is not important and will never be used. I chose this
approach, with a dummy function declared to request the creation of a
formatter, rather than having this function do the formatting itself, to make
it easier to add a formatter after the fact to existing types.

In addition to the dummy signalling function, the type must be formattable
using either a `str()` member function or a `to_string()` namespace scope
function that can be found by ADL. Besides the main object argument, these
may optionally have an argument that takes a string of flag letters that
control formatting behaviour, and one that takes an unsigned integer, usually
specifying the display precision for number-like types.

In summary, the following formatting functions will be recognized and called
by the formatter:

* `std::string T::str(std::string_view flags, std::size_t prec) const;`
* `std::string T::str(std::string_view flags) const;`
* `std::string T::str(std::size_t prec) const;`
* `std::string T::str() const;`
* `std::string to_string(const T& t, std::string_view flags, std::size_t prec) const;`
* `std::string to_string(const T& t, std::string_view flags) const;`
* `std::string to_string(const T& t, std::size_t prec) const;`
* `std::string to_string(const T& t) const;`

The formatter will allow non-digit characters as formatting flags if the
formatting function takes a flags argument, and an integer formatting flag if
the function takes a precision argument. Specific flags are not checked; if a
flags argument is allowed at all, any character will be accepted and passed
on to the underlying formatting function, except control characters, digits,
and braces.

UTF-8 encoded non-ASCII Unicode characters are valid flags. Behaviour is
undefined if the format string contains invalid UTF-8.

Formatting to output string types other than plain `std::string` is not
supported.

Example:

```c++
class MyClass {
    std::string str(std::string_view flags) const;
};

MyClass x;
std::println("{}", x);      // OK, generates default format
std::println("{:abc}", x);  // OK, calls x.str("abc")
std::println("{:42}", x);   // Compile error, str() has no integer argument
```
