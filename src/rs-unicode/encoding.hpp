#pragma once

#include "rs-unicode/character.hpp"
#include <array>
#include <compare>
#include <cstddef>
#include <format>
#include <iterator>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

namespace RS::Unicode {

    namespace Detail {

        template <typename T>
        std::string hex_string(std::basic_string_view<T> str) {
            using U = std::make_unsigned_t<T>;
            static constexpr auto digits = 2 * sizeof(T);
            if (str.empty()) {
                return {};
            }
            std::string out;
            for (auto t: str) {
                auto u = static_cast<U>(t);
                out += std::format("{:0{}x} ", u, digits);
            }
            out.pop_back();
            return out;
        }

    }

    // Exceptions

    class EncodingError:
    public std::runtime_error {
    public:
        explicit EncodingError(std::string_view subject, std::size_t pos);
        explicit EncodingError(std::u16string_view subject, std::size_t pos);
        explicit EncodingError(std::u32string_view subject, std::size_t pos);
        std::size_t pos() const noexcept { return pos_; }
    private:
        std::size_t pos_;
    };

    // Categorization of UTF code units

    enum class Utf8UnitKind: unsigned char {
        single,    // 0x00-7f        Single byte ASCII character
        leading2,  // 0xc2-df        First byte of a 2 byte character
        leading3,  // 0xe0-ef        First byte of a 3 byte character
        leading4,  // 0xf0-f4        First byte of a 4 byte character
        trailing,  // 0x80-bf        Second or later byte of a multibyte character
        illegal,   // 0xc0-c1,f5-ff  Not used
    };

    enum class Utf16UnitKind: unsigned char {
        single,  // 0x0000-d7ff,e000-ffff  Single unit BMP character
        high,    // 0xd800-dbff            High surrogate (first half) of an astral plane character
        low,     // 0xdc00-dfff            Low surrogate (second half) of an astral plane character
    };

    constexpr Utf8UnitKind utf8_unit_kind(char c) noexcept {
        auto u = static_cast<unsigned char>(c);
        if (u <= 0x7f)       { return Utf8UnitKind::single; }
        else if (u <= 0xbf)  { return Utf8UnitKind::trailing; }
        else if (u <= 0xc1)  { return Utf8UnitKind::illegal; }
        else if (u <= 0xdf)  { return Utf8UnitKind::leading2; }
        else if (u <= 0xef)  { return Utf8UnitKind::leading3; }
        else if (u <= 0xf4)  { return Utf8UnitKind::leading4; }
        else                 { return Utf8UnitKind::illegal; }
    }

    constexpr Utf16UnitKind utf16_unit_kind(char16_t c) noexcept {
        if (c <= 0xd7ff)       { return Utf16UnitKind::single; }
        else if (c <= 0xdbff)  { return Utf16UnitKind::high; }
        else if (c <= 0xdfff)  { return Utf16UnitKind::low; }
        else                   { return Utf16UnitKind::single; }
    }

    constexpr bool is_utf8_leading_unit(char c) noexcept {
        return utf8_unit_kind(c) <= Utf8UnitKind::leading4;
    }

    constexpr bool is_utf16_leading_unit(char16_t c) noexcept {
        return utf16_unit_kind(c) != Utf16UnitKind::low;
    }

    // Encoding properties of characters

    constexpr std::size_t utf8_length(char32_t c) noexcept {
        if (c <= 0x7f)                { return 1; }
        else if (c <= 0x7ff)          { return 2; }
        else if (c < min_surrogate)   { return 3; }
        else if (c <= max_surrogate)  { return 0; }
        else if (c <= max_bmp)        { return 3; }
        else if (c <= max_unicode)    { return 4; }
        else                          { return 0; }
    }

    constexpr std::size_t utf8_length(char c) noexcept {
        switch (utf8_unit_kind(c)) {
            case Utf8UnitKind::single:    return 1;
            case Utf8UnitKind::leading2:  return 2;
            case Utf8UnitKind::leading3:  return 3;
            case Utf8UnitKind::leading4:  return 4;
            default:                      return 0;
        }
    }

    constexpr std::size_t utf16_length(char32_t c) noexcept {
        if (c < min_surrogate)        { return 1; }
        else if (c <= max_surrogate)  { return 0; }
        else if (c <= max_bmp)        { return 1; }
        else if (c <= max_unicode)    { return 2; }
        else                          { return 0; }
    }

    constexpr std::size_t utf16_length(char16_t c) noexcept {
        switch (utf16_unit_kind(c)) {
            case Utf16UnitKind::single:  return 1;
            case Utf16UnitKind::high:    return 2;
            default:                     return 0;
        }
    }

    // Character encoding and decoding functions

    std::size_t decode_utf8_char(const char* in, std::size_t max_units, char32_t& out) noexcept;
    std::size_t decode_utf8_char(const char* in, char32_t& out) noexcept;
    std::size_t decode_utf16_char(const char16_t* in, std::size_t max_units, char32_t& out) noexcept;
    std::size_t decode_utf16_char(const char16_t* in, char32_t& out) noexcept;
    std::size_t encode_utf8_char(char32_t in, char* out) noexcept;
    std::size_t encode_utf16_char(char32_t in, char16_t* out) noexcept;

    // String encoding and decoding functions

    enum class Convert {
        unchecked,
        checked,
        replace,
    };

    void append_utf8(char32_t c, std::string& utf8);
    std::optional<std::size_t> utf8_error_check(std::string_view text) noexcept;
    std::optional<std::size_t> utf16_error_check(std::u16string_view text) noexcept;
    std::optional<std::size_t> utf32_error_check(std::u32string_view text) noexcept;
    std::u32string utf8_to_utf32(std::string_view utf8, Convert mode = Convert::unchecked, char32_t replace = replacement_char);
    std::u32string utf16_to_utf32(std::u16string_view utf16, Convert mode = Convert::unchecked, char32_t replace = replacement_char);
    std::string utf32_to_utf8(std::u32string_view utf32, Convert mode = Convert::unchecked, char32_t replace = replacement_char);
    std::u16string utf32_to_utf16(std::u32string_view utf32, Convert mode = Convert::unchecked, char32_t replace = replacement_char);

    // Encoding and decoding iterators

    class DecodeUtf8Iterator {

    public:

        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using pointer = const char32_t*;
        using reference = const char32_t&;
        using value_type = char32_t;

        DecodeUtf8Iterator() = default;
        explicit DecodeUtf8Iterator(const char* utf8) noexcept: utf8_(utf8) {}

        const char32_t& operator*() const noexcept;
        const char32_t* operator->() const noexcept { return &**this; }
        DecodeUtf8Iterator& operator++() noexcept;
        DecodeUtf8Iterator operator++(int) noexcept { auto i = *this; ++*this; return i; }
        DecodeUtf8Iterator& operator--() noexcept;
        DecodeUtf8Iterator operator--(int) noexcept { auto i = *this; --*this; return i; }
        bool operator==(const DecodeUtf8Iterator& i) const noexcept { return utf8_ == i.utf8_; }
        const char* ptr() const noexcept { return utf8_; }
        std::string_view view() const noexcept;

    private:

        const char* utf8_ = nullptr;     // Start of current UTF-8 encoded character
        mutable char32_t utf32_ = 0;     // Decoded UTF-32 character
        mutable std::size_t count_ = 0;  // Number of UTF-8 units (0 if not decoded yet)

    };

    inline auto decoded_utf8_view(std::string_view utf8) noexcept {
        DecodeUtf8Iterator i{utf8.data()};
        DecodeUtf8Iterator j{utf8.data() + utf8.size()};
        return std::ranges::subrange{i, j};
    }

    class EncodeUtf8Iterator {

    public:

        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using pointer = const char*;
        using reference = const char&;
        using value_type = char;

        EncodeUtf8Iterator() = default;
        explicit EncodeUtf8Iterator(const char32_t* utf32) noexcept: utf32_(utf32) {}

        const char& operator*() const noexcept;
        const char* operator->() const noexcept { return &**this; }
        EncodeUtf8Iterator& operator++() noexcept;
        EncodeUtf8Iterator operator++(int) noexcept { auto i = *this; ++*this; return i; }
        EncodeUtf8Iterator& operator--() noexcept;
        EncodeUtf8Iterator operator--(int) noexcept { auto i = *this; --*this; return i; }
        bool operator==(const EncodeUtf8Iterator& i) const noexcept { return utf32_ == i.utf32_ && index_ == i.index_; }

    private:

        const char32_t* utf32_ = nullptr;   // Current input character
        mutable std::array<char, 4> utf8_;  // UTF-8 encoded character
        mutable std::size_t count_ = 0;     // Number of UTF-8 units (0 if not decoded yet)
        mutable std::size_t index_ = 0;     // Current position in UTF-8 array

    };

    inline auto encoded_utf8_view(std::u32string_view utf32) noexcept {
        EncodeUtf8Iterator i{utf32.data()};
        EncodeUtf8Iterator j{utf32.data() + utf32.size()};
        return std::ranges::subrange{i, j};
    }

}
