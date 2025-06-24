#pragma once

#include "rs-unicode/character.hpp"
#include "rs-core/enum.hpp"
#include "rs-core/global.hpp"
#include <compare>
#include <cstddef>
#include <functional>
#include <iterator>
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

    class GraphemeIterator {

    public:

        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using pointer = const std::string_view*;
        using reference = const std::string_view&;
        using value_type = std::string_view;

        GraphemeIterator() = default;
        explicit GraphemeIterator(std::string_view str): tail_(str) { next(); }

        const std::string_view& operator*() const noexcept { return current_; }
        const std::string_view* operator->() const noexcept { return &**this; }
        GraphemeIterator& operator++();
        GraphemeIterator operator++(int) { auto i = *this; ++*this; return i; }
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

    class SplitIterator {

    public:

        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using pointer = const std::string_view*;
        using reference = const std::string_view&;
        using value_type = std::string_view;

        SplitIterator() = default;
        explicit SplitIterator(std::string_view str);
        explicit SplitIterator(std::string_view str, std::string_view delimiter);
        explicit SplitIterator(std::string_view str, std::u32string_view delimiter);
        explicit SplitIterator(std::string_view str, CharacterPredicate delimiter);

        const std::string_view& operator*() const noexcept { return current_; }
        const std::string_view* operator->() const noexcept { return &**this; }
        SplitIterator& operator++();
        SplitIterator operator++(int) { auto i = *this; ++*this; return i; }
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

    // String literals

    namespace Literals {

        std::string operator""_doc(const char* ptr, std::size_t len);

    }

}
