# Character Encodings

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/encoding.hpp"
namespace RS::Unicode;
```

## Contents

* TOC
{:toc}

## Legacy encoding conversion

```c++
class LegacyEncoding {
    LegacyEncoding() noexcept;
    explicit LegacyEncoding(const std::string& name);
    explicit LegacyEncoding(std::uint32_t key);
    ~LegacyEncoding() noexcept;
    LegacyEncoding(const LegacyEncoding& le);
    LegacyEncoding(LegacyEncoding&& le) noexcept;
    LegacyEncoding& operator=(const LegacyEncoding& le);
    LegacyEncoding& operator=(LegacyEncoding&& le) noexcept;
    std::string from_utf8(std::string_view input) const;
    std::string to_utf8(std::string_view input) const;
};
```

This class performs conversions between legacy encodings and UTF-8, using the
operating system's native encoding conversion facilities(`iconv()` on Unix,
`MultiByteToWideChar()` and `WideCharToMultiByte()` on Windows). Because of
its dependence on operating system features, some of its capabilities and
behaviour are necessarily implementation dependent.

The constructor accepts either a named encoding, or an IBM/Microsoft code page
number. The constructor will throw `std::system_error` if the name or number
is not recognized.

The `from_utf8()` and `to_utf8()` functions perform the conversions between
the specified legacy encoding and UTF-8. Invalid input in `to_utf8()` will be
replaced with the standard replacement character `U+FFFD.` Invalid UTF-8 in
`from_utf8()` will be replaced with an implementation dependent character.
These functions will not throw exceptions on encountering invalid input, but
in some cases they may throw `std::system_error` if an implementation
specific error situation is encountered (such as resource limits).

A default constructed `LegacyEncoding` object performs no conversion; its
`from/to_utf8()` functions will return their input string unchanged
(regardless of whether it contains valid UTF-8).

This API is not thread safe. Behaviour is undefined if the conversion
functions of a single `LegacyEncoding` object are called from more than one
thread.
