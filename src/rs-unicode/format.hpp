#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>

namespace RS::Unicode {

    namespace Detail {

        template <typename> constexpr auto dependent_false = false;

        template <typename T>
        concept StringOrView = std::convertible_to<T, std::string>
            || std::convertible_to<T, std::string_view>;

        template <typename T>
        concept FormatMarker = requires(T t) {
            rs_unicode_format(t);
        };

        template <typename T>
        concept FormatByStrMethod = requires (const T& t) {
            { t.str() } -> StringOrView;
        };

        template <typename T>
        concept FormatByStrMethodWithFlags = requires (const T& t, std::string_view s) {
            { t.str(s) } -> StringOrView;
        };

        template <typename T>
        concept FormatByStrMethodWithSize = requires (const T& t, std::size_t n) {
            { t.str(n) } -> StringOrView;
        };

        template <typename T>
        concept FormatByStrMethodWithBoth = requires (const T& t, std::string_view s, std::size_t n) {
            { t.str(s, n) } -> StringOrView;
        };

        template <typename T>
        concept FormatByToString = requires (const T& t) {
            { to_string(t) } -> StringOrView;
        };

        template <typename T>
        concept FormatByToStringWithFlags = requires (const T& t, std::string_view s) {
            { to_string(t, s) } -> StringOrView;
        };

        template <typename T>
        concept FormatByToStringWithSize = requires (const T& t, std::size_t n) {
            { to_string(t, n) } -> StringOrView;
        };

        template <typename T>
        concept FormatByToStringWithBoth = requires (const T& t, std::string_view s, std::size_t n) {
            { to_string(t, s, n) } -> StringOrView;
        };

        template <typename T>
        concept AutoFormat = FormatMarker<T>
            && (FormatByStrMethod<T>
                || FormatByStrMethodWithFlags<T>
                || FormatByStrMethodWithSize<T>
                || FormatByStrMethodWithBoth<T>
                || FormatByToString<T>
                || FormatByToStringWithFlags<T>
                || FormatByToStringWithSize<T>
                || FormatByToStringWithBoth<T>);

    }

}

template <::RS::Unicode::Detail::AutoFormat T>
struct std::formatter<T> {

private:

    static constexpr auto with_flags =
        ::RS::Unicode::Detail::FormatByStrMethodWithFlags<T>
        || ::RS::Unicode::Detail::FormatByStrMethodWithBoth<T>
        || ::RS::Unicode::Detail::FormatByToStringWithFlags<T>
        || ::RS::Unicode::Detail::FormatByToStringWithBoth<T>;

    static constexpr auto with_size =
        ::RS::Unicode::Detail::FormatByStrMethodWithSize<T>
        || ::RS::Unicode::Detail::FormatByStrMethodWithBoth<T>
        || ::RS::Unicode::Detail::FormatByToStringWithSize<T>
        || ::RS::Unicode::Detail::FormatByToStringWithBoth<T>;

    template <int> class dummy_type {};

    struct flags_base {
        std::array<char, 128> flags;
        std::size_t n_flags = 0;
    };

    struct size_base {
        std::size_t size = 0;
    };

    struct info_type:
    std::conditional_t<with_flags, flags_base, dummy_type<1>>,
    std::conditional_t<with_size, size_base, dummy_type<2>> {};

    info_type info_;

    constexpr std::string_view flags() const noexcept {
        if constexpr (with_flags) {
            return std::string_view(info_.flags.data(), info_.n_flags);
        } else {
            return {};
        }
    }

    static constexpr bool legit_flag(char c) noexcept {
        return c >= ' ' && c <= '~'
            && ! (c >= '0' && c <= '9')
            && c != '(' && c != ')'
            && c != '[' && c != ']'
            && c != '{' && c != '}';
    }

public:

    constexpr auto parse(std::format_parse_context& ctx) {

        auto it = ctx.begin();

        for (; it != ctx.end() && *it != '}'; ++it) {

            if constexpr (with_flags) {
                if (legit_flag(*it)) {
                    info_.flags[info_.n_flags++] = *it;
                    continue;
                }
            }

            if constexpr (with_size) {
                if (*it >= '0' && *it <= '9') {
                    info_.size = info_.size * 10uz + static_cast<std::size_t>(*it - '0');
                    continue;
                }
            }

            throw std::format_error{std::format("Invalid format: {:?}", *it)};

        }

        return it;

    }

    // The gratuitous template here is a workaround for a libc++ bug.
    // https://github.com/llvm/llvm-project/issues/66466

    template <typename FormatContext>
    auto format(const T& t, FormatContext& ctx) const {

        using namespace ::RS::Unicode::Detail;

        std::string out;

        if constexpr (FormatByStrMethodWithBoth<T>) {
            out = t.str(flags(), info_.size);
        } else if constexpr (FormatByStrMethodWithFlags<T>) {
            out = t.str(flags());
        } else if constexpr (FormatByStrMethodWithSize<T>) {
            out = t.str(info_.size);
        } else if constexpr (FormatByStrMethod<T>) {
            out = t.str();
        } else if constexpr (FormatByToStringWithBoth<T>) {
            out = to_string(t, flags(), info_.size);
        } else if constexpr (FormatByToStringWithFlags<T>) {
            out = to_string(t, flags());
        } else if constexpr (FormatByToStringWithSize<T>) {
            out = to_string(t, info_.size);
        } else if constexpr (FormatByToString<T>) {
            out = to_string(t);
        } else {
            static_assert(dependent_false<T>);
        }

        return std::ranges::copy(out, ctx.out()).out;

    }

};
