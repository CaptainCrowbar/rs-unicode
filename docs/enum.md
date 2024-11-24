# Enumeration Helpers

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/enum.hpp"
namespace RS::Unicode;
```

## Contents

* TOC
{:toc}

## Scoped enumeration types

```c++
#define RS_UNICODE_ENUM(EnumType, IntType, ...)
```

This macro defines a scoped enumeration type with a user-supplied list of
values. It also defines a few additional features around the new type:

* `const std::set<EnumType>& enum_values(EnumType)`
    * Returns a collection of all the enumeration values. This uses a dummy
      argument to identify the type (instead of a template parameter) so the
      function can be found by ADL.
* `std::string to_string(EnumType t)`
    * Returns the unqualified enumeration value name as a string.
* `std::formatter<EnumType>`
    * Formatter that calls `to_strig().`

The list of enumeration values can include explicit integer values, but the
macro does not understand values based on other identifiers or arithmetic
expressions. For example, `my_volue=0x100` is fine, but
`my_value=another_value+2` will not work.

Example:

```c++
RS_UNICODE_ENUM(MyEnum, int,
    alpha = 1,
    bravo,
    charlie,
)
```

This is equivalent to:

```c++
enum class MyEnum: int {
    alpha = 1,
    bravo,
    charlie,
};

const std::set<MyEnum>& enum_values(MyEnum);
std::string to_string(MyEnum t);
template <> struct std::formatter<MyEnum>;
```

## Bitmask types

```c++
#define RS_UNICODE_BITMASK(EnumType, IntType, ...)
```

This generates all the same features as `RS_UNICODE_ENUM(),` but also creates
a full set of operators that perform bitwise operations on the enumeration
type:

```c++
constexpr bool operator!(EnumType t) noexcept;
constexpr EnumType operator~(EnumType t) noexcept;
constexpr EnumType operator&(EnumType t, EnumType u) noexcept;
constexpr EnumType operator|(EnumType t, EnumType u) noexcept;
constexpr EnumType operator^(EnumType t, EnumType u) noexcept;
constexpr EnumType& operator&=(EnumType& t, EnumType u) noexcept;
constexpr EnumType& operator|=(EnumType& t, EnumType u) noexcept;
constexpr EnumType& operator^=(EnumType& t, EnumType u) noexcept;
```

The version of `to_string()` generated for bitmasks will break down a
composite value into its individual named bitmask elements.

Example:

```c++
RS_UNICODE_BITMASK(MyBitmask, unsigned,
    none     = 0,
    alpha    = 1,
    bravo    = 2,
    charlie  = 4,
)

auto mask = MyBitmask::alpha | MyBitmask::bravo | MyBitmask::charlie;
std::println("{}", mask);
```

This will print `"alpha|bravo|charlie".`
