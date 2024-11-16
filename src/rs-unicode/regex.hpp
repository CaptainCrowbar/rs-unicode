#pragma once

#include "rs-unicode/character.hpp"
#include "rs-unicode/enum.hpp"
#include <compare>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace RS::Unicode {

    enum class RegexFlags: std::uint16_t {

        none,

        anchor         = 0x0001,  // Must start at beginning             [Any]              PCRE2_ANCHORED                      --     --
        byte           = 0x0002,  // Byte mode (default is UTF-8)        [Compile only]     ~ (PCRE2_UCP | PCRE2_UTF)           (?b)*  --
        dotall         = 0x0004,  // . matches LF                        [Compile only]     PCRE2_DOTALL                        (?s)   Perl /s
        extended       = 0x0008,  // Free form layout                    [Compile only]     PCRE2_EXTENDED                      (?x)   Perl /x
        firstline      = 0x0010,  // Must start in first line            [Compile only]     PCRE2_FIRSTLINE                     --     --
        full           = 0x0020,  // Match complete string               [Any]              PCRE2_ANCHORED | PCRE2_ENDANCHORED  --     --
        global         = 0x0040,  // Replace all matches                 [Format only]      PCRE2_SUBSTITUTE_GLOBAL             --     Perl /g
        icase          = 0x0080,  // Case insensitive                    [Compile only]     PCRE2_CASELESS                      (?i)   Perl /i
        multiline      = 0x0100,  // ^ and $ match each line             [Compile only]     PCRE2_MULTILINE                     (?m)   Perl /m
        nocapture      = 0x0200,  // Groups are not captured             [Compile only]     PCRE2_NO_AUTO_CAPTURE               --     --
        notbol         = 0x0400,  // ^ does not match beginning          [Match or format]  PCRE2_NOTBOL                        --     --
        notempty       = 0x0800,  // Do not match empty string           [Match or format]  PCRE2_NOTEMPTY                      --     --
        notemptystart  = 0x1000,  // Do not match empty string at start  [Match or format]  PCRE2_NOTEMPTY_ATSTART              --     --
        noteol         = 0x2000,  // $ does not match end                [Match or format]  PCRE2_NOTEOL                        --     --
        partial        = 0x4000,  // Partial matching                    [Match only]       PCRE2_PARTIAL_HARD                  --     --

    };

    RS_UNICODE_DEFINE_BITMASK_OPERATORS(RegexFlags)

    class Regex {

    public:

        using enum RegexFlags;

        class error;
        class match;
        class match_iterator;
        class split_iterator;

        Regex() = default;
        explicit Regex(std::string_view pattern, RegexFlags flags = {});
        ~Regex() noexcept;
        Regex(const Regex& re);
        Regex(Regex&& re) noexcept;
        Regex& operator=(const Regex& re);
        Regex& operator=(Regex&& re) noexcept;

        std::size_t groups() const noexcept;
        match search(std::string_view subject, RegexFlags flags = {}, std::size_t offset = 0) const;
        match operator()(std::string_view subject, RegexFlags flags = {}, std::size_t offset = 0) const;
        auto grep(std::string_view subject, RegexFlags flags = {}, std::size_t offset = 0) const;
        auto split(std::string_view subject, RegexFlags flags = {}, std::size_t offset = 0) const;
        std::string format(std::string_view subject, std::string_view replace, RegexFlags flags = {}) const;
        void swap(Regex& re) noexcept;
        friend void swap(Regex& a, Regex& b) noexcept { a.swap(b); }

        static std::pair<int, int> pcre_version() noexcept;

    private:

        void* code_ {};
        RegexFlags flags_ {};

    };

    class Regex::error:
    public std::runtime_error {

    public:

        int code() const noexcept { return code_; }

    private:

        friend class Regex;

        explicit error(const std::string& message);
        explicit error(int code);

        int code_ = 0;

    };

    class Regex::match {

    public:

        bool matched(std::size_t index = 0) const noexcept;
        operator bool() const noexcept { return matched(); }
        bool partial() const noexcept { return partial_; }
        std::size_t pos(std::size_t index = 0) const noexcept;
        std::size_t endpos(std::size_t index = 0) const noexcept;
        std::size_t len(std::size_t index = 0) const noexcept { return endpos(index) - pos(index); }
        std::string_view str(std::size_t index = 0) const noexcept;
        std::string_view operator[](std::size_t index) const noexcept { return str(index); }
        operator std::string_view() const noexcept { return str(0); }

    private:

        friend class Regex;

        std::vector<std::pair<std::size_t, std::size_t>> groups_;
        std::string_view subject_;
        bool partial_ = false;

    };

    class Regex::match_iterator {

    public:

        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using pointer = const match*;
        using reference = const match&;
        using value_type = match;

        const match& operator*() const noexcept { return match_; }
        const match* operator->() const noexcept { return &**this; }
        match_iterator& operator++() { next(match_.endpos()); return *this; }
        match_iterator operator++(int) { auto i = *this; ++*this; return i; }
        bool operator==(const match_iterator& i) const noexcept { return match_.pos() == i.match_.pos(); }
        bool operator==(std::nullptr_t) const noexcept { return ! match_; }

    private:

        friend class Regex;

        const Regex* regex_ {};
        match match_;
        std::string_view subject_;
        RegexFlags flags_ {};

        match_iterator() = default;
        match_iterator(const Regex& regex, std::string_view subject, RegexFlags flags, std::size_t offset);

        void next(std::size_t pos);

    };

    class Regex::split_iterator {

    public:

        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using pointer = const std::string_view*;
        using reference = const std::string_view&;
        using value_type = std::string_view;

        const std::string_view& operator*() const noexcept { return current_; }
        const std::string_view* operator->() const noexcept { return &**this; }
        split_iterator& operator++() { next(); return *this; }
        split_iterator operator++(int) { auto i = *this; ++*this; return i; }
        bool operator==(const split_iterator& i) const noexcept { return current_.data() == i.current_.data(); }
        bool operator==(std::nullptr_t) const noexcept { return current_.data() == nullptr; }

    private:

        friend class Regex;

        const Regex* regex_ {};
        std::string_view current_;
        std::string_view subject_;
        std::size_t restart_ {npos};
        RegexFlags flags_ {};

        split_iterator() = default;
        split_iterator(const Regex& regex, std::string_view subject, RegexFlags flags, std::size_t offset);

        void next();

    };

    inline auto Regex::grep(std::string_view subject, RegexFlags flags, std::size_t offset) const {
        match_iterator it{*this, subject, flags, offset};
        return std::ranges::subrange{it, nullptr};
    }

    inline auto Regex::split(std::string_view subject, RegexFlags flags, std::size_t offset) const {
        split_iterator it{*this, subject, flags, offset};
        return std::ranges::subrange{it, nullptr};
    }

    namespace Literals {

        inline Regex operator""_re(const char* ptr, std::size_t len) { return Regex{std::string_view{ptr, len}}; }

    }

}
