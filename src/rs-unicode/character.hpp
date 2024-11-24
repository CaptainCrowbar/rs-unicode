#pragma once

#include "rs-unicode/enum.hpp"
#include "rs-unicode/property-list.hpp"
#include <cstddef>
#include <limits>

namespace RS::Unicode {

    // Sanity check

    static_assert(std::numeric_limits<unsigned char>::digits == 8);

    // Constants

    constexpr auto npos = static_cast<std::size_t>(-1);

    constexpr char32_t min_surrogate     = 0xd800;
    constexpr char32_t max_surrogate     = 0xdfff;
    constexpr char32_t byte_order_mark   = 0xfeff;
    constexpr char32_t replacement_char  = 0xfffd;
    constexpr char32_t max_bmp           = 0xffff;
    constexpr char32_t max_unicode       = 0x10'ffff;
    constexpr char32_t not_unicode       = 0x11'0000;

    // Character property functions

    constexpr bool is_ascii(char32_t c) noexcept { return c <= 0x7f; }
    constexpr bool is_astral(char32_t c) noexcept { return c > max_bmp && c <= max_unicode; }
    constexpr bool is_surrogate(char32_t c) noexcept { return c >= min_surrogate && c <= max_surrogate; }
    constexpr bool is_bmp(char32_t c) noexcept { return c <= max_bmp && ! is_surrogate(c); }
    constexpr bool is_unicode(char32_t c) noexcept { return c <= max_unicode && ! is_surrogate(c); }
    inline bool is_alphanumeric(char32_t c) { auto pc = primary_category(c); return pc == 'L' || pc == 'N'; }

    constexpr bool is_noncharacter(char32_t c) noexcept {
        return ! is_unicode(c)
            || (c >= 0xfdd0 && c <= 0xfdef)
            || (c & 0xfffe) == 0xfffe;
    }

    inline bool is_xid_nonstart(char32_t c) {
        return is_xid_continue(c) && ! is_xid_start(c);
    }

    constexpr bool is_private_use(char32_t c) noexcept {
        return (c >= 0xe000 && c <= 0xf8ff)
            || (c >= 0xf'0000 && c <= 0xf'fffd)
            || (c >= 0x10'0000 && c <= 0x10'fffd);
    }

    constexpr bool is_hangul(char32_t c) noexcept {
        return (c >= 0x1100 && c <= 0x11ff)
            || (c >= 0xa960 && c <= 0xa97c)
            || (c >= 0xac00 && c <= 0xd7fb);
    }

    RS_UNICODE_ENUM(Hangul_Syllable_Type, unsigned char,
        NA, L, LV, LVT, T, V,
    )

    constexpr Hangul_Syllable_Type hangul_syllable_type(char32_t c) noexcept {
        if ((c >= 0x1100 && c <= 0x115f) || (c >= 0xa960 && c <= 0xa97c)) {
            return Hangul_Syllable_Type::L;
        } else if ((c >= 0x1160 && c <= 0x11a7) || (c >= 0xd7b0 && c <= 0xd7c6)) {
            return Hangul_Syllable_Type::V;
        } else if ((c >= 0x11a8 && c <= 0x11ff) || (c >= 0xd7cb && c <= 0xd7fb)) {
            return Hangul_Syllable_Type::T;
        } else if (c <= 0xabff || c >= 0xd7a4) {
            return Hangul_Syllable_Type::NA;
        } else if ((c - 0xac00) % 28 == 0) {
            return Hangul_Syllable_Type::LV;
        } else {
            return Hangul_Syllable_Type::LVT;
        }
    }

}
