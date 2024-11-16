# Common Infrastucture

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/common.hpp"
namespace RS::Unicode;
```

## Contents

* TOC
{:toc}

## Namespaces

```c++
namespace rs = std::ranges;
namespace vs = std::views;
```

Convenience aliases.

## Constants

```c++
constexpr std::string_view ascii_whitespace = "\t\n\r ";
constexpr auto npos = static_cast<std::size_t>(-1);
```

## Arithmetic functions

```c++
template <typename T>
    requires std::integral<T> || std::is_enum_v<T>
    constexpr bool has_bit(T x, T y) noexcept;
```

Check whether a bit is set. This returns true if the arguments have at least
one bit in common.
