# Character Encodings

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/encoding.hpp"
namespace RS::Unicode;
```

## Contents

* TOC
{:toc}

## Exceptions

```c++
class EncodingError: public std::runtime_error {
    explicit EncodingError(std::string_view subject, std::size_t pos);
    explicit EncodingError(std::u16string_view subject, std::size_t pos);
    explicit EncodingError(std::u32string_view subject, std::size_t pos);
    std::size_t pos() const noexcept;
};
```

Exception representing an invalid encoding. The `pos()` function returns the
position of the erroneous data, as an offset in code units from the beginning
of the subject string.

## Categorization of UTF code units

```c++
enum class Utf8UnitKind: unsigned char {
    single,    // 0x00-7f        Single byte ASCII character
    leading2,  // 0xc2-df        First byte of a 2 byte character
    leading3,  // 0xe0-ef        First byte of a 3 byte character
    leading4,  // 0xf0-f4        First byte of a 4 byte character
    trailing,  // 0x80-bf        Second or later byte
    illegal,   // 0xc0-c1,f5-ff  Not used
};

enum class Utf16UnitKind: unsigned char {
    single,  // Single unit BMP character
    high,    // High (leading) surrogate
    low,     // Low (trailing) surrogate
};

constexpr Utf8UnitKind utf8_unit_kind(char c) noexcept;
constexpr Utf16UnitKind utf16_unit_kind(char16_t c) noexcept;
constexpr bool is_utf8_leading_unit(char c) noexcept;
constexpr bool is_utf16_leading_unit(char16_t c) noexcept;
```

Properties of individual code units.

## Encoding properties of characters

```c++
constexpr std::size_t utf8_length(char32_t c) noexcept;
constexpr std::size_t utf8_length(char c) noexcept;
constexpr std::size_t utf16_length(char32_t c) noexcept;
constexpr std::size_t utf16_length(char16_t c) noexcept;
```

Length of an encoded character, based on either the encoded USV or the first
code unit. The first version of each function will return zero if the
argument is not a legal USV; the second will return zero if the argument is
not a legal leading code unit.

## Character encoding and decoding functions

```c++
std::size_t decode_utf8_char(const char* in, std::size_t max_units,
    char32_t& out) noexcept;
std::size_t decode_utf8_char(const char* in, char32_t& out) noexcept;
std::size_t decode_utf16_char(const char16_t* in, std::size_t max_units,
    char32_t& out) noexcept;
std::size_t decode_utf16_char(const char16_t* in, char32_t& out) noexcept;
```

Decode a UTF encoded character. On success, the decoded character is written
to the output argument and the number of units read is returned. The first
version of each function returns zero and leaves the output argument
unchanged if the input is empty or is not valid UTF. The second version
assumes that the input is valid UTF and contains at least one encoded
character; behaviour is undefined if the input is invalid.

```c++
std::size_t encode_utf8_char(char32_t in, char* out) noexcept;
std::size_t encode_utf16_char(char32_t in, char16_t* out) noexcept;
```

Encode a UTF character. On success, the encoded character is written to the
output array, which is assumed to have enough space for the appropriate
number of code units, and the function returns the number of code units
written. If the input is not a valid USV, the function returns zero and
leaves the output array unchanged. Behaviour is undefined if the underlying
array used for output does not have room for the encoded character.

## String encoding and decoding functions

```c++
void append_utf8(char32_t c, std::string& utf8);
```

Append a character to a string. Behaviour is undefined if the character is not
a valid USV.

```c++
std::optional<std::size_t>
    utf8_error_check(std::string_view text) noexcept;
std::optional<std::size_t>
    utf16_error_check(std::u16string_view text) noexcept;
std::optional<std::size_t>
    utf32_error_check(std::u32string_view text) noexcept;
```

Check for valid UTF encoding. These return the position (offset in code units)
of the first invalid encoding if any is found, otherwise null.

```c++
void valid_utf8(std::string_view text);
void valid_utf16(std::u16string_view text);
void valid_utf32(std::u32string_view text);
```

Check for valid UTF encoding. These throw `EncodingError` if invalid encoding
is found.

```c++
enum class Convert {
    unchecked,
    checked,
    replace,
};

std::u32string utf8_to_utf32(std::string_view utf8,
    Convert mode = Convert::unchecked, char32_t replace = replacement_char);
std::u32string utf16_to_utf32(std::u16string_view utf16,
    Convert mode = Convert::unchecked, char32_t replace = replacement_char);
std::string utf32_to_utf8(std::u32string_view utf32,
    Convert mode = Convert::unchecked, char32_t replace = replacement_char);
std::u16string utf32_to_utf16(std::u32string_view utf32,
    Convert mode = Convert::unchecked, char32_t replace = replacement_char);
```

Convert between UTF encodings, optionally checking for invalid encoding. The
`mode` argument controls the behaviour if the input string contains invalid
UTF encoding:

* `unchecked` -- Unchecked conversion. Behaviour is undefined on invalid
  encoding. This is the default.
* `checked` -- An `EncodingError` exception is thrown when invalid encoding is
  encountered.
* `replace` -- Invalid encoding is replaced with the specified character,
  defaulting to the standard replacement character `U+FFFD.` Behaviour is
  undefined if the replacement character is not a valid Unicode scalar value.
  The `replace` argument is ignored in any other conversion mode.

## Encoding and decoding iterators

To save space, not all members are documented explicitly. You can assume that
all of the implied standard iterator operations are supplied.

```c++
class DecodeUtf8Iterator {
    using iterator_category = std::bidirectional_iterator;
    using value_type = char32_t;
    DecodeUtf8Iterator() noexcept;
    explicit DecodeUtf8Iterator(const char* utf8) noexcept;
    const char32_t& operator*() const noexcept;
    const char* ptr() const noexcept;
    std::string_view view() const noexcept;
};

[subrange view] decoded_utf8_view(std::string_view utf8) noexcept;
```

Bidirectional iterator over the encoded Unicode scalar values in a UTF-8
string. This requires the input to be valid UTF-8; behaviour is undefined if
invalid encoding is encountered.

The `ptr()` function returns a pointer to the beginning of the current encoded
character. It can safely be called on a past-the-end iterator, returning a
past-the-end pointer to the underlying byte array. On a default constructed
iterator it will return a null pointer.

The `view()` function returns a view over the code units of the current
encoded character. Behaviour is undefined if `view()` is called on a default
constructed or past-the-end iterator.

```c++
class EncodeUtf8Iterator {
    using iterator_category = std::bidirectional_iterator;
    using value_type = char;
    EncodeUtf8Iterator() noexcept;
    explicit EncodeUtf8Iterator(const char32_t* utf32) noexcept;
    const char& operator*() const noexcept;
    bool operator==(const EncodeUtf8Iterator& i) const noexcept;
};

[subrange view] encoded_utf8_view(std::u32string_view utf32) noexcept;
```

Bidirectional iterator over the UTF-8 code units of a UTF-32 string. This
requires the input to be valid UTF-32; behaviour is undefined if an invalid
USV is encountered.
