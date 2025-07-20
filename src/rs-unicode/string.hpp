#pragma once

#include "rs-unicode/character.hpp"
#include "rs-unicode/encoding.hpp"
#include "rs-core/enum.hpp"
#include "rs-core/global.hpp"
#include "rs-core/iterator.hpp"
#include <compare>
#include <concepts>
#include <cstddef>
#include <functional>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace RS::Unicode {

    // Types

    using CharacterPredicate = std::function<bool(char32_t)>;

    // Grapheme clusters

    class GraphemeIterator:
    public ForwardIterator<GraphemeIterator, const std::string_view> {

    public:

        GraphemeIterator() = default;
        explicit GraphemeIterator(std::string_view str): tail_(str) { next(); }

        const std::string_view& operator*() const noexcept { return current_; }
        GraphemeIterator& operator++();
        bool operator==(const GraphemeIterator& i) const noexcept;

    private:

        std::string_view current_;
        std::string_view tail_;

        void next();

    };

    inline auto grapheme_view(std::string_view str) {
        GraphemeIterator i{str};
        GraphemeIterator j{str.substr(str.size())};
        return std::ranges::subrange{i, j};
    }

    // String segmentation

    class SplitIterator:
    public ForwardIterator<SplitIterator, const std::string_view> {

    public:

        SplitIterator() = default;
        explicit SplitIterator(std::string_view str);
        explicit SplitIterator(std::string_view str, std::string_view delimiter);
        explicit SplitIterator(std::string_view str, std::u32string_view delimiter);
        explicit SplitIterator(std::string_view str, CharacterPredicate delimiter);

        const std::string_view& operator*() const noexcept { return current_; }
        SplitIterator& operator++();
        bool operator==(const SplitIterator& i) const noexcept;

    private:

        CharacterPredicate predicate_;
        std::string delimiter_;
        std::string_view current_;
        std::string_view tail_;

        void next();

    };

    inline auto split_view(std::string_view str) {
        SplitIterator i{str};
        SplitIterator j;
        return std::ranges::subrange{i, j};
    }

    inline auto split_view(std::string_view str, std::string_view delimiter) {
        SplitIterator i{str, delimiter};
        SplitIterator j;
        return std::ranges::subrange{i, j};
    }

    inline auto split_view(std::string_view str, std::u32string_view delimiter) {
        SplitIterator i{str, delimiter};
        SplitIterator j;
        return std::ranges::subrange{i, j};
    }

    inline auto split_view(std::string_view str, CharacterPredicate delimiter) {
        SplitIterator i{str, delimiter};
        SplitIterator j;
        return std::ranges::subrange{i, j};
    }

    inline auto split_vector(std::string_view str) {
        return split_view(str)
            | std::views::transform([] (std::string_view s) { return std::string{s}; })
            | std::ranges::to<std::vector>();
    }

    inline auto split_vector(std::string_view str, std::string_view delimiter) {
        return split_view(str, delimiter)
            | std::views::transform([] (std::string_view s) { return std::string{s}; })
            | std::ranges::to<std::vector>();
    }

    inline auto split_vector(std::string_view str, std::u32string_view delimiter) {
        return split_view(str, delimiter)
            | std::views::transform([] (std::string_view s) { return std::string{s}; })
            | std::ranges::to<std::vector>();
    }

    inline auto split_vector(std::string_view str, CharacterPredicate delimiter) {
        return split_view(str, delimiter)
            | std::views::transform([] (std::string_view s) { return std::string{s}; })
            | std::ranges::to<std::vector>();
    }

    // String metrics

    RS_ENUM(Unit, unsigned char,
        bytes,      // UTF-8 code units
        utf16,      // UTF-16 code units
        scalars,    // Unicode scalar values (default)
        graphemes,  // Grapheme clusters
        columns,    // Virtual columns based on East Asian width, defaulting to narrow
        wide,       // Same as columns, but defaulting to wide
    )

    std::size_t length(std::string_view str, Unit u = Unit::scalars);
    std::pair<std::size_t, std::size_t> line_and_column(std::string_view text,
        std::size_t pos, Unit u = Unit::scalars) noexcept;

    // Case conversion

    std::string to_casefold(std::string_view str);
    std::string to_lowercase(std::string_view str);
    std::string to_titlecase(std::string_view str);
    std::string to_uppercase(std::string_view str);

    // Normalization

    std::string to_nfc(std::string_view str);
    std::string to_nfd(std::string_view str);

    // Subscripts and superscripts

    std::optional<std::string> to_subscript(std::string_view str);
    std::optional<std::string> to_superscript(std::string_view str);

    // String manipulation functions

    namespace Detail {

        inline void cat_once(std::string& s, const std::string& t) { s += t; }
        inline void cat_once(std::string& s, const std::u8string& t) { s.append(reinterpret_cast<const char*>(t.data()), t.size()); }
        inline void cat_once(std::string& s, const std::u16string& t) { s += utf32_to_utf8(utf16_to_utf32(t)); }
        inline void cat_once(std::string& s, const std::u32string& t) { s += utf32_to_utf8(t); }
        inline void cat_once(std::string& s, const std::string_view& t) { s += t; }
        inline void cat_once(std::string& s, const std::u8string_view& t) { s.append(reinterpret_cast<const char*>(t.data()), t.size()); }
        inline void cat_once(std::string& s, const std::u16string_view& t) { s += utf32_to_utf8(utf16_to_utf32(t)); }
        inline void cat_once(std::string& s, const std::u32string_view& t) { s += utf32_to_utf8(t); }

        inline void cat_once(std::string& s, const std::wstring& t) {
            if constexpr (sizeof(wchar_t) == 2) {
                std::u16string_view u16(reinterpret_cast<const char16_t*>(t.data()), t.size());
                cat_once(s, u16);
            } else {
                std::u32string_view u32(reinterpret_cast<const char32_t*>(t.data()), t.size());
                cat_once(s, u32);
            }
        }

        inline void cat_once(std::string& s, const std::wstring_view& t) {
            if constexpr (sizeof(wchar_t) == 2) {
                std::u16string_view u16(reinterpret_cast<const char16_t*>(t.data()), t.size());
                cat_once(s, u16);
            } else {
                std::u32string_view u32(reinterpret_cast<const char32_t*>(t.data()), t.size());
                cat_once(s, u32);
            }
        }

        inline void cat_once(std::string& s, char t) { s += t; }
        inline void cat_once(std::string& s, char8_t t) { s += static_cast<char>(t); }
        inline void cat_once(std::string& s, char16_t t) { append_utf8(static_cast<char32_t>(t), s); }
        inline void cat_once(std::string& s, char32_t t) { append_utf8(t, s); }
        inline void cat_once(std::string& s, wchar_t t) { append_utf8(static_cast<char32_t>(t), s); }
        inline void cat_once(std::string& s, const char* t) { s += t; }
        inline void cat_once(std::string& s, const char8_t* t) { s += reinterpret_cast<const char*>(t); }
        inline void cat_once(std::string& s, const char16_t* t) { cat_once(s, std::u16string_view(t)); }
        inline void cat_once(std::string& s, const char32_t* t) { cat_once(s, std::u32string_view(t)); }
        inline void cat_once(std::string& s, const wchar_t* t) { cat_once(s, std::wstring_view(t)); }
        inline void cat_once(std::string& s, bool t) { s += t ? "true" : "false"; }

        template <std::integral T>
        void cat_once(std::string& s, T t) {
            s += std::to_string(t);
        }

        template <typename T>
        void cat_once(std::string&, const T*) = delete;

        template <typename T>
        concept CatStringType = requires (std::string& s, T t) {
            cat_once(s, t);
        };

        inline void cat_many(std::string&) {}

        template <CatStringType T>
        void cat_many(std::string& s, T&& t) {
            cat_once(s, std::forward<T>(t));
        }

        template <CatStringType T, CatStringType... TS>
        void cat_many(std::string& s, T&& t, TS&&... ts) {
            cat_once(s, std::forward<T>(t));
            cat_many(s, std::forward<TS>(ts)...);
        }

    }

    template <Detail::CatStringType... TS>
    std::string cat(TS&&... ts) {
        std::string s;
        Detail::cat_many(s, std::forward<TS>(ts)...);
        return s;
    }

    template <std::ranges::range R>
    std::string join(const R& range, std::string_view delimiter = {}) {

        std::string out;

        for (const auto& item: range) {
            out += item;
            out += delimiter;
        }

        if (! out.empty() && ! delimiter.empty()) {
            out.resize(out.size() - delimiter.size());
        }

        return out;

    }

    std::string pad_left(std::string_view str, std::size_t to_length,
        char32_t padding = U' ', Unit u = Unit::columns);
    std::string pad_right(std::string_view str, std::size_t to_length,
        char32_t padding = U' ', Unit u = Unit::columns);
    std::pair<std::string_view, std::string_view> partition(std::string_view str) noexcept;
    std::pair<std::string_view, std::string_view> partition(std::string_view str,
        std::string_view delimiter) noexcept;
    std::pair<std::string_view, std::string_view> partition(std::string_view str,
        std::u32string_view delimiter) noexcept;
    std::pair<std::string_view, std::string_view> partition(std::string_view str,
        CharacterPredicate delimiter) noexcept;
    std::string repeat(char32_t c, std::size_t n);
    std::string repeat(std::string_view str, std::size_t n);
    std::string replace(std::string_view source, std::string_view target,
        std::string_view replacement, std::size_t number = npos);
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
    std::string word_wrap(std::string_view text, std::size_t width = npos,
        std::size_t indent1 = 0, std::size_t indent2 = npos, Unit u = Unit::columns);

    // String search functions

    std::string_view find_first_of(std::string_view str, char32_t c) noexcept;
    std::string_view find_first_of(std::string_view str, std::string_view chars) noexcept;
    std::string_view find_first_of(std::string_view str, std::u32string_view chars) noexcept;
    std::string_view find_first_of(std::string_view str, CharacterPredicate pred);
    std::string_view find_first_not_of(std::string_view str, char32_t c) noexcept;
    std::string_view find_first_not_of(std::string_view str, std::string_view chars) noexcept;
    std::string_view find_first_not_of(std::string_view str, std::u32string_view chars) noexcept;
    std::string_view find_first_not_of(std::string_view str, CharacterPredicate pred);
    std::string_view find_last_of(std::string_view str, char32_t c) noexcept;
    std::string_view find_last_of(std::string_view str, std::string_view chars) noexcept;
    std::string_view find_last_of(std::string_view str, std::u32string_view chars) noexcept;
    std::string_view find_last_of(std::string_view str, CharacterPredicate pred);
    std::string_view find_last_not_of(std::string_view str, char32_t c) noexcept;
    std::string_view find_last_not_of(std::string_view str, std::string_view chars) noexcept;
    std::string_view find_last_not_of(std::string_view str, std::u32string_view chars) noexcept;
    std::string_view find_last_not_of(std::string_view str, CharacterPredicate pred);

    // String literals

    namespace Literals {

        std::string operator""_doc(const char* ptr, std::size_t len);
        std::vector<std::string> operator""_qw(const char* ptr, std::size_t len);

    }

}
