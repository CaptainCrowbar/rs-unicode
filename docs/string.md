# String Manipulation

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/string.hpp"
namespace RS::Unicode;
```

## Contents

* TOC
{:toc}

## Important

**All functions in this header assume that their input is valid UTF-8.
Behaviour is undefined if they are passed invalid encoding.**

## Types

```c++
using CharacterPredicate = std::function<bool(char32_t)>;
```

Callback type used in some of these functions.

## Grapheme clusters

```c++
class GraphemeIterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::string_view;
    GraphemeIterator() = default;
    explicit GraphemeIterator(std::string_view str);
    const std::string_view& operator*() const noexcept { return current_; }
};

[grapheme iterator range] grapheme_view(std::string_view str);
```

Iterator over the grapheme clusters in a UTF-8 string. It dereferences to a
string view representing a single grapheme cluster.

## String segmentation

```c++
class SplitIterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::string_view;
    SplitIterator() = default;
    explicit SplitIterator(std::string_view str);
    explicit SplitIterator(std::string_view str,
        std::string_view delimiter);
    explicit SplitIterator(std::string_view str,
        std::u32string_view delimiter);
    explicit SplitIterator(std::string_view str,
        CharacterPredicate delimiter);
    const std::string_view& operator*() const noexcept { return current_; }
};

[split iterator range] split_view(std::string_view str);
[split iterator range] split_view(std::string_view str,
    std::string_view delimiter);
[split iterator range] split_view(std::string_view str,
    std::u32string_view delimiter);
[split iterator range] split_view(std::string_view str,
    CharacterPredicate delimiter);
```

Iterator over parts of a string. The segmentation method depends on the
delimiter argument:

* Default: Split on any subsequence of `White_Space` characters.
* String view argument: Split on each occurrence of the substring. The number
  of segments will always be one more than the number of occurrences of the
  delimiter, unless the subject string is empty. This is the only version
  that can generate empty segments.
* UTF-32 string view argument: Split on any subsequence of these scalar
  values.
* Predicate argument: Split on any subsequence of scalar values that match
  this predicate. Behaviour is undefined if a null function is passed.

For the second and third versions, if the delimiter is empty (but the subject
string is not), the complete subject string will be treated as a single
unbroken segment.

```c++
std::vector<std::string> split_vector(std::string_view str);
std::vector<std::string> split_vector(std::string_view str,
    std::string_view delimiter);
std::vector<std::string> split_vector(std::string_view str,
    std::u32string_view delimiter);
std::vector<std::string> split_vector(std::string_view str,
    CharacterPredicate delimiter);
```

Convenience functions to turn a split view into a vector of strings.

## String metrics

```c++
enum class Unit: unsigned char {
    bytes,      // Bytes (UTF-8 code units)
    utf16,      // UTF-16 code units
    scalars,    // Unicode scalar values (default)
    graphemes,  // Grapheme clusters
    columns,    // Virtual columns (Western text, see below)
    wide,       // Virtual columns (East Asian text, see below)
};

std::size_t length(std::string_view str, Unit u = Unit::scalars);
```

String length using various measures.

The two "virtual columns" modes are intended to yield the number of character
columns taken up by the string when displayed in a fixed width font, based on
the `East_Asian_Width` property. Characters with ambiguous width are counted
as narrow (one column) in `columns` mode, or as wide (two columns) in `wide`
mode. All control characters, formatting characters, and default ignorable
characters are counted as zero width (note that this includes tabs and line
breaks). In practise this will only be an approximation; most nominally "fixed
width" fonts contain small irregularities that prevent text containing
characters from different scripts from lining up perfectly.

```c++
std::pair<std::size_t, std::size_t> line_and_column(std::string_view text,
    std::size_t pos, Unit u = Unit::scalars) noexcept;
```

Converts a byte offset to a line and column number within a string. Only LF
characters are recognised as line breaks. Columns are counted according to
the units argument. The input position is clamped to the length of the
string. All measures are zero based.

## Case conversion

```c++
std::string to_casefold(std::string_view str);
std::string to_lowercase(std::string_view str);
std::string to_titlecase(std::string_view str);
std::string to_uppercase(std::string_view str);
```

Case conversion functions for UTF-8 strings. These use the full locale
invariant case conversion rules from the Unicode standard. The output string
may be longer than the input string, but can never be shorter. Normalization
may not be preserved.

The `to_titlecase()` function uses the same simple segmentation algorithm as
the default version of `split_view(),` and then applies title casing to the
first alphanumeric character (general category starts with `L` or `N`) in a
segment, and lower casing to all subsequent characters. Anything more
sophisticated will require the caller to handle word segmentation itself, and
then call `to_titlecase()` on each word.

## Normalization

```c++
std::string to_nfc(std::string_view str);
std::string to_nfd(std::string_view str);
```

Normalization functions. We only support NFC and NFD formats.

## Subscripts and superscripts

```c++
std::optional<std::string> to_subscript(std::string_view str);
std::optional<std::string> to_superscript(std::string_view str);
```

Convert a string to subscripts or superscripts. These will return null if the
input string contains characters that have no corresponding subscript or
superscript character in Unicode.

## String manipulation functions

```c++
template <rs::range R> std::string join(const R& range,
    std::string_view delimiter = {});
```

Join a range of strings into a single string, with an optional delimiter
between them.

```c++
std::string pad_left(std::string_view str, std::size_t to_length,
    char32_t padding = U' ', Unit u = Unit::columns);
std::string pad_right(std::string_view str, std::size_t to_length,
    char32_t padding = U' ', Unit u = Unit::columns);
```

Pad a string on the left or right to a specified length, measured in virtual
columns by default. If matching the exact length is impossible, because the
padding character's width is greater than 1 and does not divide into the
number of extra columns needed, the longest padded string that will fit into
the specified width will be returned. The string will be returned unchanged
if it is already longer than the requested length, or if the padding
character has zero or variable width.

```c++
std::pair<std::string_view, std::string_view>
    partition(std::string_view str) noexcept;
std::pair<std::string_view, std::string_view>
    partition(std::string_view str, std::string_view delimiter) noexcept;
std::pair<std::string_view, std::string_view>
    partition(std::string_view str, std::u32string_view delimiter) noexcept;
std::pair<std::string_view, std::string_view>
    partition(std::string_view str, CharacterPredicate delimiter) noexcept;
```

Split a string at the first occurrence of one or more delimiter characters.
The delimiter options work the same as for `SplitIterator.` If no delimiters
are found, or the delimiter list is empty, this returns the original string
and an empty string.

```c++
std::string repeat(char32_t c, std::size_t n);
std::string repeat(std::string_view str, std::size_t n);
```

Generate a UTF-8 string containing a character or string repeated a given
number of times.

```c++
std::string replace(std::string_view source, std::string_view target,
    std::string_view replacement, std::size_t number = npos);
```

Replace a substring with another. The number argument indicates the maximum
number of replacements to be made; by default there is no limit.

```c++
std::string_view trim(std::string_view str);
std::string_view trim(std::string_view str, std::string_view substr);
std::string_view trim(std::string_view str, std::u32string_view chars);
std::string_view trim(std::string_view str, CharacterPredicate pred);
std::string_view trim_left(std::string_view str);
std::string_view trim_left(std::string_view str, std::string_view substr);
std::string_view trim_left(std::string_view str, std::u32string_view chars);
std::string_view trim_left(std::string_view str, CharacterPredicate pred);
std::string_view trim_right(std::string_view str);
std::string_view trim_right(std::string_view str, std::string_view substr);
std::string_view trim_right(std::string_view str, std::u32string_view chars);
std::string_view trim_right(std::string_view str, CharacterPredicate pred);
```

Trim leading or trailing characters from a string.

* Default: Trim all `White_Space` characters.
* String view argument: Trim any prefix or suffix that matches the target
  string. It will be removed at most once from each end even if there are
  multiple copies of the substring.
* UTF-32 string view argument: Trim all characters in the string.
* Predicate argument: Trim all characters that match the predicate
  (behaviour is undefined if a null function is passed).

```c++
std::string word_wrap(std::string_view text, std::size_t width = npos,
    std::size_t indent1 = 0, std::size_t indent2 = npos,
    Unit u = Unit::columns);
```

Wrap text to a given width, measured in virtual columns by default. Word
segmentation uses the default version of `SplitIterator.` If no width is
given, the current terminal width minus 1 will be used. Optional indentation
can be supplied, for the first line and subsequent lines; `indent1` will be
used for all lines if `indent2=npos.` The width includes the indentation. If
a single word is too long to fit in the width, it will be allowed to violate
the right margin.

## String search functions

```c++
std::string_view find_first_of(std::string_view str,
    char32_t c) noexcept;
std::string_view find_first_of(std::string_view str,
    std::string_view chars) noexcept;
std::string_view find_first_of(std::string_view str,
    std::u32string_view chars) noexcept;
std::string_view find_first_of(std::string_view str,
    CharacterPredicate pred);
std::string_view find_first_not_of(std::string_view str,
    char32_t c) noexcept;
std::string_view find_first_not_of(std::string_view str,
    std::string_view chars) noexcept;
std::string_view find_first_not_of(std::string_view str,
    std::u32string_view chars) noexcept;
std::string_view find_first_not_of(std::string_view str,
    CharacterPredicate pred);
std::string_view find_last_of(std::string_view str,
    char32_t c) noexcept;
std::string_view find_last_of(std::string_view str,
    std::string_view chars) noexcept;
std::string_view find_last_of(std::string_view str,
    std::u32string_view chars) noexcept;
std::string_view find_last_of(std::string_view str,
    CharacterPredicate pred);
std::string_view find_last_not_of(std::string_view str,
    char32_t c) noexcept;
std::string_view find_last_not_of(std::string_view str,
    std::string_view chars) noexcept;
std::string_view find_last_not_of(std::string_view str,
    std::u32string_view chars) noexcept;
std::string_view find_last_not_of(std::string_view str,
    CharacterPredicate pred);
```

Find the first or last UTF-8 encoded character matching, or failing to match,
a specific character, any one of a set of characters (passed as either UTF-8
or UTF-32), or any character matching a predicate. On success, the function
returns a string view over the encoded character; on failure, it returns a
null (default constructed) string view.

## String literals

```c++
namespace Literals {
    std::string operator""_doc(const char* ptr, std::size_t len);
}
```

Define a multiline raw string literal without breaking the indentation of the
surrounding context. Any leading whitespace common to all lines of the string
is removed from all lines. A leading line break is ignored.

Example:

```c++
auto str = R"(
    Hello world
        Goodnight moon
    )"_doc;
```

This is equivalent to:

```c++
auto str =
    "Hello world\n"
    "    Goodnight moon\n";
```
