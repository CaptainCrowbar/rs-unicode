#include "rs-unicode/encoding.hpp"
#include <algorithm>
#include <utility>

namespace rs = std::ranges;

namespace RS::Unicode {

    // EncodingError class

    namespace {

        template <typename CodeUnit>
        std::string error_message(std::basic_string_view<CodeUnit> subject, std::size_t pos) {
            return std::format("UTF-{} encoding error at offset {}: {}",
                8 * sizeof(CodeUnit), pos, Detail::hex_string(subject.substr(pos, 4 / sizeof(CodeUnit))));
        }

    }

    EncodingError::EncodingError(std::string_view subject, std::size_t pos):
    std::runtime_error(error_message(subject, pos)), pos_(pos) {}

    EncodingError::EncodingError(std::u16string_view subject, std::size_t pos):
    std::runtime_error(error_message(subject, pos)), pos_(pos) {}

    EncodingError::EncodingError(std::u32string_view subject, std::size_t pos):
    std::runtime_error(error_message(subject, pos)), pos_(pos) {}

    // Encoding conversion functions

    std::size_t decode_utf8_char(const char* in, std::size_t max_units, char32_t& out) noexcept {

        if (max_units == 0) {
            return 0;
        }

        auto length = utf8_length(*in);

        if (length == 0 || length > max_units) {
            return 0;
        }

        auto in_bytes = reinterpret_cast<const unsigned char*>(in);

        if (std::any_of(in_bytes + 1, in_bytes + length, [] (unsigned char u) { return u <= 0x7f || u >= 0xc0; })) {
            return 0;
        }

        auto ch = U'\0';
        decode_utf8_char(in, ch);

        if (! is_unicode(ch)) {
            return 0;
        }

        out = ch;

        return length;

    }

    std::size_t decode_utf8_char(const char* in, char32_t& out) noexcept {

        auto byte_ptr = reinterpret_cast<const unsigned char*>(in);
        auto length = utf8_length(*in);

        auto f = [byte_ptr] (int index, unsigned char and_mask, int shift) {
            return static_cast<char32_t>(byte_ptr[index] & and_mask) << shift;
        };

        switch (length) {

            case 1:
                out = byte_ptr[0];
                break;

            case 2:
                out = f(0, 0x1f, 6)
                    + f(1, 0x3f, 0);
                break;

            case 3:
                out = f(0, 0x0f, 12)
                    + f(1, 0x3f, 6)
                    + f(2, 0x3f, 0);
                break;

            case 4:
                out = f(0, 0x07, 18)
                    + f(1, 0x3f, 12)
                    + f(2, 0x3f, 6)
                    + f(3, 0x3f, 0);
                break;

            default:
                std::unreachable();

        }

        return length;

    }

    std::size_t decode_utf16_char(const char16_t* in, std::size_t max_units, char32_t& out) noexcept {
        out = 0;
        if (max_units == 0) {
            return 0;
        }
        auto kind = utf16_unit_kind(in[0]);
        if (kind == Utf16UnitKind::single
                || (kind == Utf16UnitKind::high
                    && max_units >= 2
                    && utf16_unit_kind(in[1]) == Utf16UnitKind::low)) {
            return decode_utf16_char(in, out);
        } else {
            return 0;
        }
    }

    std::size_t decode_utf16_char(const char16_t* in, char32_t& out) noexcept {
        if (utf16_unit_kind(in[0]) == Utf16UnitKind::single) {
            out = static_cast<char32_t>(in[0]);
            return 1;
        } else {
            auto high = static_cast<char32_t>(in[0] - 0xd800);
            auto low = static_cast<char32_t>(in[1] - 0xdc00);
            out = char32_t{0x1'0000} + (high << 10) + low;
            return 2;
        }
    }

    std::size_t encode_utf8_char(char32_t in, char* out) noexcept {

        auto length = utf8_length(in);

        auto f = [in] (int shift, unsigned char and_mask, unsigned char or_mask) {
            return static_cast<char>(static_cast<unsigned char>((in >> shift) & and_mask) | or_mask);
        };

        switch (length) {

            case 1:
                out[0] = static_cast<char>(in);
                break;

            case 2:
                out[0] = f(6, 0xff, 0xc0);
                out[1] = f(0, 0x3f, 0x80);
                break;

            case 3:
                out[0] = f(12, 0xff, 0xe0);
                out[1] = f(6, 0x3f, 0x80);
                out[2] = f(0, 0x3f, 0x80);
                break;

            case 4:
                out[0] = f(18, 0xff, 0xf0);
                out[1] = f(12, 0x3f, 0x80);
                out[2] = f(6, 0x3f, 0x80);
                out[3] = f(0, 0x3f, 0x80);
                break;

        }

        return length;

    }

    std::size_t encode_utf16_char(char32_t in, char16_t* out) noexcept {
        if (! is_unicode(in)) {
            return 0;
        } else if (in <= 0xffff) {
            out[0] = static_cast<char16_t>(in);
            return 1;
        } else {
            auto u = in - 0x1'0000;
            out[0] = static_cast<char16_t>(0xd800 + (u >> 10));
            out[1] = static_cast<char16_t>(0xdc00 + (u & 0x3ff));
            return 2;
        }
    }

    void append_utf8(char32_t c, std::string& utf8) {
        auto pos = utf8.size();
        utf8.resize(pos + 4);
        auto len = encode_utf8_char(c, utf8.data() + pos);
        utf8.resize(pos + len);
    }

    std::optional<std::size_t> utf8_error_check(std::string_view text) noexcept {

        auto ptr = text.data();
        auto len = text.size();
        auto ch = U'\0';

        while (len > 0) {
            auto n = decode_utf8_char(ptr, len, ch);
            if (n == 0) {
                return text.size() - len;
            }
            ptr += n;
            len -= n;
        }

        return {};

    }

    std::optional<std::size_t> utf16_error_check(std::u16string_view text) noexcept {

        auto ptr = rs::data(text);
        auto len = rs::size(text);
        auto i = 0uz;

        while (i < len) {

            switch (utf16_unit_kind(ptr[i])) {

                case Utf16UnitKind::single:
                    ++i;
                    break;

                case Utf16UnitKind::high:
                    if (i + 1 == len || utf16_unit_kind(ptr[i + 1]) != Utf16UnitKind::low) {
                        return i;
                    }
                    i += 2;
                    break;

                default:
                    return i;

            }

        }

        return {};

    }

    std::optional<std::size_t> utf32_error_check(std::u32string_view text) noexcept {
        auto it = rs::find_if_not(text, is_unicode);
        if (it == text.end()) {
            return {};
        } else {
            return static_cast<std::size_t>(it - text.begin());
        }
    }

    std::u32string utf8_to_utf32(std::string_view utf8, Convert mode, char32_t replace) {

        std::u32string out;
        auto ptr = utf8.data();
        auto len = utf8.size();
        auto ch = U'\0';

        while (len > 0) {

            auto n = decode_utf8_char(ptr, len, ch);

            if (n == 0) {
                switch (mode) {
                    case Convert::checked:
                        throw EncodingError(utf8, static_cast<std::size_t>(ptr - utf8.data()));
                    case Convert::replace:
                        for (n = 1; n < len && n < 4uz && ! is_utf8_leading_unit(ptr[n]); ++n) {}
                        out += replace;
                        break;
                    default:
                        std::unreachable();
                }
            } else {
                out += ch;
            }

            ptr += n;
            len -= n;

        }

        return out;

    }

    std::u32string utf16_to_utf32(std::u16string_view utf16, Convert mode, char32_t replace) {

        std::u32string out;
        auto ptr = utf16.data();
        auto len = utf16.size();
        auto ch = U'\0';

        while (len > 0) {

            auto n = decode_utf16_char(ptr, len, ch);

            if (n == 0) {
                switch (mode) {
                    case Convert::checked:
                        throw EncodingError(utf16, static_cast<std::size_t>(ptr - utf16.data()));
                    case Convert::replace:
                        for (n = 1; n < len && n < 2uz && ! is_utf16_leading_unit(ptr[n]); ++n) {}
                        out += replace;
                        break;
                    default:
                        std::unreachable();
                }
            } else {
                out += ch;
            }

            ptr += n;
            len -= n;

        }

        return out;

    }

    std::string utf32_to_utf8(std::u32string_view utf32, Convert mode, char32_t replace) {

        std::string out;
        out.reserve(utf32.size() + 4);
        auto len = 0uz;

        for (const auto& ch: utf32) {
            out.resize(len + 4);
            auto n = encode_utf8_char(ch, out.data() + len);
            if (n == 0) {
                switch (mode) {
                    case Convert::checked:
                        throw EncodingError(utf32, static_cast<std::size_t>(&ch - utf32.data()));
                    case Convert::replace:
                        n = encode_utf8_char(replace, out.data() + len);
                        break;
                    default:
                        std::unreachable();
                }
            }
            len += n;
        }

        out.resize(len);

        return out;

    }

    std::u16string utf32_to_utf16(std::u32string_view utf32, Convert mode, char32_t replace) {

        std::u16string out;
        out.reserve(utf32.size() + 2);
        auto len = 0uz;

        for (const auto& ch: utf32) {
            out.resize(len + 2);
            auto n = encode_utf16_char(ch, out.data() + len);
            if (n == 0) {
                switch (mode) {
                    case Convert::checked:
                        throw EncodingError(utf32, static_cast<std::size_t>(&ch - utf32.data()));
                    case Convert::replace:
                        n = encode_utf16_char(replace, out.data() + len);
                        break;
                    default:
                        std::unreachable();
                }
            }
            len += n;
        }

        out.resize(len);

        return out;

    }

    // DecodeUtf8Iterator class

    const char32_t& DecodeUtf8Iterator::operator*() const noexcept {
        count_ = decode_utf8_char(utf8_, utf32_);
        return utf32_;
    }

    DecodeUtf8Iterator& DecodeUtf8Iterator::operator++() noexcept {
        utf8_ += utf8_length(*utf8_);
        count_ = 0;
        return *this;
    }

    DecodeUtf8Iterator& DecodeUtf8Iterator::operator--() noexcept {
        do {
            --utf8_;
        } while (! is_utf8_leading_unit(*utf8_));
        count_ = utf8_length(*utf8_);
        return *this;
    }

    std::string_view DecodeUtf8Iterator::view() const noexcept {
        if (count_ == 0) {
            count_ = decode_utf8_char(utf8_, utf32_);
        }
        return std::string_view(utf8_, count_);
    }

    // EncodeUtf8Iterator class

    const char& EncodeUtf8Iterator::operator*() const noexcept {
        count_ = encode_utf8_char(*utf32_, utf8_.data());
        return utf8_[index_];
    }

    EncodeUtf8Iterator& EncodeUtf8Iterator::operator++() noexcept {
        if (++index_ >= count_) {
            ++utf32_;
            count_ = index_ = 0;
        }
        return *this;
    }

    EncodeUtf8Iterator& EncodeUtf8Iterator::operator--() noexcept {
        if (index_ == 0) {
            --utf32_;
            count_ = encode_utf8_char(*utf32_, utf8_.data());
            index_ = count_ - 1;
        } else {
            --index_;
        }
        return *this;
    }

}
