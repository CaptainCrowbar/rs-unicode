# Version Information

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/version.hpp"
namespace RS::Unicode;
```

This header supplies library version information.

```c++
std::array<int, 3> library_version() noexcept;
```

Returns an array containing the major, minor, and patch version numbers for
this library.

```c++
std::array<int, 3> unicode_version() noexcept;
```

Returns an array containing the major, minor, and patch version numbers for
the Unicode character database used when the library was built.

```c++
std::string library_version_string();
std::string unicode_version_string();
```

Return the version numbers as a string (e.g. `"1.23.456"`).
