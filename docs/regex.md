# Regular Expressions

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/regex.hpp"
namespace RS::Unicode;
```

This is a thin C++ wrapper around the [PCRE2](https://www.pcre.org) library.
(I think we're all agreed by now that `std::regex` is beyond saving.) The API
is a bit simplified; not all PCRE features are currently exported.

## Contents

* TOC
{:toc}

## Regex behaviour flags

```c++
enum class RegexFlags: std::uint16_t;
```

| Flag             | Description                                          | Phases              | Prefix             |
| ----             | -----------                                          | ------              | ------             |
| `none`           | No flags                                             |                     |                    |
| `anchor`         | The match must start at the beginning of the string  | All                 |                    |
| `byte`           | Byte mode matching (default is UTF-8 mode)           | Compile&nbsp;only   | <nobr>(?b)</nobr>  |
| `dotall`         | Dot matches any character including LF               | Compile&nbsp;only   | <nobr>(?s)</nobr>  |
| `extended`       | Free form pattern layout                             | Compile&nbsp;only   | <nobr>(?x)</nobr>  |
| `firstline`      | The match must start in the first line               | Compile&nbsp;only   |                    |
| `full`           | The pattern must match the complete string           | All                 |                    |
| `global`         | Replace all matches                                  | Format&nbsp;only    |                    |
| `icase`          | Case insensitive                                     | Compile&nbsp;only   | <nobr>(?i)</nobr>  |
| `multiline`      | ^ and $ match the beginning and end of each line     | Compile&nbsp;only   | <nobr>(?m)</nobr>  |
| `nocapture`      | Groups are not captured unless they are named        | Compile&nbsp;only   |                    |
| `notbol`         | ^ does not match the beginning of the string         | Match,&nbsp;format  |                    |
| `notempty`       | Do not match an empty string                         | Match,&nbsp;format  |                    |
| `notemptystart`  | Do not match an empty string at the start            | Match,&nbsp;format  |                    |
| `noteol`         | $ does not match the end of the string               | Match,&nbsp;format  |                    |
| `partial`        | Enable partial matching                              | Match&nbsp;only     |                    |

These are bitmasks defined originally in the `RegexFlags` enumeration, but
they are imported into the `Regex` class via `using enum RegexFlags` and
should normally be used as member constants of the class, e.g.
`Regex::anchor.` A full set of bitwise operators are defined to allow these
to be combined.

The _Phases_ column lists which phases of regex processing the flag is
relevant to: compiling (constructing the `Regex` object), searching, or
formatting.

The effects of some of the compile time flags can be replicated by attaching a
prefix to the pattern. This can be useful in conjunction with the custom
literal operator (see below).

All flags can be supplied at compile time even if they are listed here as
applying only to match or format time. Any flags that cannot be used at
compile time will be saved and applied at match or format time.

All of the matching functions -- `search(), operator(), grep(), split(),` and
`format()` -- will throw `Regex::error` if they are passed a flag that is
only valid at compile time.

## Regex class

```c++
class Regex;
```

The regular expression class. This is basically a wrapper around
`pcre_code*.`

```c++
using enum RegexFlags;
```

Member constants (see below).

```c++
class Regex::error;
class Regex::match;
class Regex::match_iterator;
class Regex::split_iterator;
```

Member types. These are described in detail below.

```c++
Regex::Regex() noexcept;
```

The default constructor creates a null regex that will never match anything.

```c++
explicit Regex::Regex(std::string_view pattern, RegexFlags flags = {});
```

The primary regex constructor. Refer to the PCRE documentation for the details
of the regex dialect it uses. The flags bitmask is described below. This will
throw `Regex::error` if the pattern contains invalid syntax, or is too
complicated for PCRE to handle.

```c++
Regex::~Regex() noexcept;
Regex::Regex(const Regex& re);
Regex::Regex(Regex&& re) noexcept;
Regex& Regex::operator=(const Regex& re);
Regex& Regex::operator=(Regex&& re) noexcept;
```

Other life cycle functions.

```c++
std::size_t Regex::groups() const noexcept;
```

Returns the number of capture groups in the expression, including the complete
match (`$0`). The only time this will return zero is if it is called on a
default constructed or moved-from object.

```c++
Regex::match Regex::search(std::string_view subject,
    RegexFlags flags = {}, std::size_t offset = 0) const;
Regex::match Regex::operator()(std::string_view subject,
    RegexFlags flags = {}, std::size_t offset = 0) const;
```

Search for a pattern within a string. Optionally a starting offset can be
supplied; if this is not zero, any potential match will start no earlier than
this point, but look-behind assertions can still see the earlier part of the
subject string.

```c++
[match_iterator range] Regex::grep(std::string_view subject,
    RegexFlags flags = {}, std::size_t offset = 0) const;
```

Find all non-overlapping matches in the subject string. The returned iterators
dereference to a `Regex::match.`

```c++
[split_iterator range] Regex::split(std::string_view subject,
    RegexFlags flags = {}, std::size_t offset = 0) const;
```

Split the subject string on every non-overlapping match. The returned
iterators dereference to a `std::string_view.`

```c++
std::string Regex::format(std::string_view subject, std::string_view replace,
    RegexFlags flags = {}) const;
```

Transform the subject string, replacing the first matching substring with the
transformed replacement string. If the `Regex::global` flag is used, all
non-overlapping matches will be replaced.

```c++
void Regex::swap(Regex& re) noexcept;
void swap(Regex& a, Regex& b) noexcept;
```

Swap two objects.

```c++
static std::pair<int, int> Regex::pcre_version() noexcept;
```

Returns the version number of the PCRE library, as a `(major,minor)` pair.

## Exceptions

```c++
class Regex::error:
public std::runtime_error {
public:
    int code() const noexcept;
};
```

An exception used to report errors in regular expression processing. If the
error was reported by the PCRE API, the `code()` function will return the
original error code; otherwise it will return zero.

## Regex match class

```c++
class Regex::match {
public:
    bool matched(std::size_t index = 0) const noexcept;
    operator bool() const noexcept;
    bool partial() const noexcept;
    std::size_t pos(std::size_t index = 0) const noexcept;
    std::size_t endpos(std::size_t index = 0) const noexcept;
    std::size_t len(std::size_t index = 0) const noexcept;
    std::string_view str(std::size_t index = 0) const noexcept;
    std::string_view operator[](std::size_t index) const noexcept;
    operator std::string_view() const noexcept;
};
```

This class represents a regular expression match.

Most of the member functions take an index argument that selects a specific
capture group, or the complete match if the index is zero. An index greater
than the number of groups in the original pattern will be treated as an
unmatched group.

The `matched()` function indicates whether the match was successful, or
whether any particular capture group was matched. The boolean operator is
equivalent to `matched(0).`

The `partial()` function indicates whether a partial match occurred. If this
is true, all capture groups are treated as unmatched, even if they could have
matched within the partially matched substring.

The `pos(), endpos(),` and `len()` functions report the position and length of
the match, or a specific capture group, as byte offsets. If the search was
started at a non-zero offset, positions are relative to the start of the
subject string, not the start of the search. If the group failed to match,
`pos()` and `endpos()` will return `npos,` and `len()` will return zero.

The `str()` function, or its `operator[]` equivalent, returns the substring
matching the regex or a given capture group. They will return a null view
(one whose `data()` pointer is null) if the pattern failed to match, or for a
capture group that was not matched. Groups that matched an empty substring
will also return an empty view, but in this case the view's `data()` will
point to the appropriate location in the subject string. The string view
operator is equivalent to `str(0).`

## Iterators

To save space, not all iterator class members are documented explicitly. You
can assume that all of the implied standard iterator operations are
supplied.

### Regex match iterator

```c++
class Regex::match_iterator {
public:
    using iterator_category = std::forward_iterator;
    using value_type = Regex::match;
    const match& operator*() const noexcept;
};
```

Forward iterator over the matches returned by the `Regex::grep()` function.

### Regex split iterator

```c++
class Regex::split_iterator {
public:
    using iterator_category = std::forward_iterator;
    using value_type = std::string_view;
    const std::string_view& operator*() const noexcept;
};
```

Forward iterator over the segments returned by the `Regex::split()` function.

## Literals

```c++
Regex operator""_re(const char* ptr, std::size_t len);
```

Custom literal that constructs a regular expression. Some of the compile time
options can be set by adding a prefix to the pattern (see above).
