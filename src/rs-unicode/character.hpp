#pragma once

#include "rs-unicode/property-list.hpp"
#include "rs-core/enum.hpp"
#include "rs-core/global.hpp"

namespace RS::Unicode {

    // Constants

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

    RS_ENUM(Hangul_Syllable_Type, unsigned char,
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

    inline char32_t subscript_character(char32_t c) {
        return subscript_superscript_base_table(c).first;
    }

    inline char32_t superscript_character(char32_t c) {
        return subscript_superscript_base_table(c).second;
    }

    inline bool is_combined_id_start(char32_t c) {
        return is_xid_start(c) || is_id_compat_math_start(c) || c == U'_';
    }

    inline bool is_combined_id_continue(char32_t c) {
        return is_xid_continue(c) || is_id_compat_math_continue(c);
    }

}
