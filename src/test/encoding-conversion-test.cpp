#include "rs-unicode/encoding.hpp"
#include "test/unit-test.hpp"
#include <cstddef>
#include <cstdint>
#include <format>
#include <optional>
#include <string>

using namespace RS::Unicode;
using namespace std::literals;

namespace {

    constexpr auto u16(char16_t c) noexcept { return static_cast<std::uint16_t>(c); }
    constexpr auto u32(char32_t c) noexcept { return static_cast<std::uint32_t>(c); }

}

void test_rs_unicode_encoding_decode_utf8_characters() {

    std::string s;
    auto c = U'\0';
    auto n = 0uz;

    s = "\0hello"s;         TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0u);
    s = " hello";           TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x20u);
    s = "~hello";           TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x7eu);
    s = "\u0080hello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u32(c), 0x80u);
    s = "\u07ffhello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u32(c), 0x7ffu);
    s = "\u0800hello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0x800u);
    s = "\u0fffhello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0xfffu);
    s = "\u1000hello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0x1000u);
    s = "\ud7ffhello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0xd7ffu);
    s = "\ue000hello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0xe000u);
    s = "\uffffhello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0xffffu);
    s = "\U00010000hello";  TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 4u);  TEST_EQUAL(u32(c), 0x1'0000u);
    s = "\U000fffffhello";  TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 4u);  TEST_EQUAL(u32(c), 0xf'ffffu);
    s = "\U00100000hello";  TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 4u);  TEST_EQUAL(u32(c), 0x10'0000u);
    s = "\U0010ffffhello";  TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 4u);  TEST_EQUAL(u32(c), u32(max_unicode));

    s = "\0hello"s;         TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0u);
    s = " hello";           TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x20u);
    s = "~hello";           TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x7eu);
    s = "\u0080hello";      TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u32(c), 0x80u);
    s = "\u07ffhello";      TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u32(c), 0x7ffu);
    s = "\u0800hello";      TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0x800u);
    s = "\u0fffhello";      TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0xfffu);
    s = "\u1000hello";      TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0x1000u);
    s = "\ud7ffhello";      TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0xd7ffu);
    s = "\ue000hello";      TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0xe000u);
    s = "\uffffhello";      TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 3u);  TEST_EQUAL(u32(c), 0xffffu);
    s = "\U00010000hello";  TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 4u);  TEST_EQUAL(u32(c), 0x1'0000u);
    s = "\U000fffffhello";  TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 4u);  TEST_EQUAL(u32(c), 0xf'ffffu);
    s = "\U00100000hello";  TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 4u);  TEST_EQUAL(u32(c), 0x10'0000u);
    s = "\U0010ffffhello";  TRY(n = decode_utf8_char(s.data(), c));  TEST_EQUAL(n, 4u);  TEST_EQUAL(u32(c), u32(max_unicode));

    s = "";                       TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\x80hello";              TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\xbfhello";              TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\xc0hello";              TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\xc1hello";              TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\xf5hello";              TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\xffhello";              TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\xed\xa0\x80hello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\xed\xbf\xbfhello";      TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = "\xf4\x90\x80\x80hello";  TRY(n = decode_utf8_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);

}

void test_rs_unicode_encoding_encode_utf8_characters() {

    std::string s(10, '\0');
    std::string t;
    auto n = 0uz;

    t = s;  TRY(n = encode_utf8_char(0, t.data()));            TEST_EQUAL(n, 1u);  TEST_EQUAL(t[0], '\0');
    t = s;  TRY(n = encode_utf8_char(0x41, t.data()));         TEST_EQUAL(n, 1u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"A\"");
    t = s;  TRY(n = encode_utf8_char(0x80, t.data()));         TEST_EQUAL(n, 2u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\\u{80}\"");
    t = s;  TRY(n = encode_utf8_char(0x7ff, t.data()));        TEST_EQUAL(n, 2u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\u07ff\"");
    t = s;  TRY(n = encode_utf8_char(0x800, t.data()));        TEST_EQUAL(n, 3u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\u0800\"");
    t = s;  TRY(n = encode_utf8_char(0xfff, t.data()));        TEST_EQUAL(n, 3u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\\u{fff}\"");
    t = s;  TRY(n = encode_utf8_char(0x1000, t.data()));       TEST_EQUAL(n, 3u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\u1000\"");
    t = s;  TRY(n = encode_utf8_char(0xd7ff, t.data()));       TEST_EQUAL(n, 3u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\\u{d7ff}\"");
    t = s;  TRY(n = encode_utf8_char(0xe800, t.data()));       TEST_EQUAL(n, 3u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\\u{e800}\"");
    t = s;  TRY(n = encode_utf8_char(0xffff, t.data()));       TEST_EQUAL(n, 3u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\\u{ffff}\"");
    t = s;  TRY(n = encode_utf8_char(0x10000, t.data()));      TEST_EQUAL(n, 4u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\U00010000\"");
    t = s;  TRY(n = encode_utf8_char(0xfffff, t.data()));      TEST_EQUAL(n, 4u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\\u{fffff}\"");
    t = s;  TRY(n = encode_utf8_char(0x100000, t.data()));     TEST_EQUAL(n, 4u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\\u{100000}\"");
    t = s;  TRY(n = encode_utf8_char(max_unicode, t.data()));  TEST_EQUAL(n, 4u);  TEST_EQUAL(std::format("{0:?}", t.data()), "\"\\u{10ffff}\"");

    TRY(n = encode_utf8_char(min_surrogate, t.data()));  TEST_EQUAL(n, 0u);
    TRY(n = encode_utf8_char(max_surrogate, t.data()));  TEST_EQUAL(n, 0u);

}

void test_rs_unicode_encoding_decode_utf16_characters() {

    std::u16string s;
    auto c = U'\0';
    auto n = 0uz;

    s = u"\0hello"s;         TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0u);
    s = u" hello";           TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x20u);
    s = u"~hello";           TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x7eu);
    s = u"\u0080hello";      TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x80u);
    s = u"\u07ffhello";      TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x7ffu);
    s = u"\u0800hello";      TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x800u);
    s = u"\u0fffhello";      TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0xfffu);
    s = u"\u1000hello";      TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0x1000u);
    s = u"\ud7ffhello";      TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0xd7ffu);
    s = u"\ue000hello";      TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0xe000u);
    s = u"\uffffhello";      TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 1u);  TEST_EQUAL(u32(c), 0xffffu);
    s = u"\U00010000hello";  TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u32(c), 0x1'0000u);
    s = u"\U000fffffhello";  TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u32(c), 0xf'ffffu);
    s = u"\U00100000hello";  TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u32(c), 0x10'0000u);
    s = u"\U0010ffffhello";  TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u32(c), u32(max_unicode));

    s = {};               TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = {0xd800};         TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = {0xdc00};         TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = {0xdc00,0xd800};  TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = {0xd800,'A'};     TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);
    s = {0xdc00,'A'};     TRY(n = decode_utf16_char(s.data(), s.size(), c));  TEST_EQUAL(n, 0u);

}

void test_rs_unicode_encoding_encode_utf16_characters() {

    std::u16string s(10, u'\0');
    std::u16string t;
    auto n = 0uz;

    t = s;  TRY(n = encode_utf16_char(0, t.data()));            TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0);
    t = s;  TRY(n = encode_utf16_char(0x41, t.data()));         TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0x41);
    t = s;  TRY(n = encode_utf16_char(0x80, t.data()));         TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0x80);
    t = s;  TRY(n = encode_utf16_char(0x7ff, t.data()));        TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0x7ff);
    t = s;  TRY(n = encode_utf16_char(0x800, t.data()));        TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0x800);
    t = s;  TRY(n = encode_utf16_char(0xfff, t.data()));        TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0xfff);
    t = s;  TRY(n = encode_utf16_char(0x1000, t.data()));       TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0x1000);
    t = s;  TRY(n = encode_utf16_char(0xd7ff, t.data()));       TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0xd7ff);
    t = s;  TRY(n = encode_utf16_char(0xe800, t.data()));       TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0xe800);
    t = s;  TRY(n = encode_utf16_char(0xffff, t.data()));       TEST_EQUAL(n, 1u);  TEST_EQUAL(u16(t[0]), 0xffff);
    t = s;  TRY(n = encode_utf16_char(0x10000, t.data()));      TEST_EQUAL(n, 2u);  TEST_EQUAL(u16(t[0]), 0xd800); TEST_EQUAL(u16(t[1]), 0xdc00);
    t = s;  TRY(n = encode_utf16_char(0xfffff, t.data()));      TEST_EQUAL(n, 2u);  TEST_EQUAL(u16(t[0]), 0xdbbf); TEST_EQUAL(u16(t[1]), 0xdfff);
    t = s;  TRY(n = encode_utf16_char(0x100000, t.data()));     TEST_EQUAL(n, 2u);  TEST_EQUAL(u16(t[0]), 0xdbc0); TEST_EQUAL(u16(t[1]), 0xdc00);
    t = s;  TRY(n = encode_utf16_char(max_unicode, t.data()));  TEST_EQUAL(n, 2u);  TEST_EQUAL(u16(t[0]), 0xdbff); TEST_EQUAL(u16(t[1]), 0xdfff);

    TRY(n = encode_utf16_char(min_surrogate, t.data()));  TEST_EQUAL(n, 0u);
    TRY(n = encode_utf16_char(max_surrogate, t.data()));  TEST_EQUAL(n, 0u);

}

void test_rs_unicode_encoding_append_utf8() {

    std::string s;

    TRY(append_utf8(U'a', s));  TEST_EQUAL(s.size(), 1u);   TEST_EQUAL(s, "a");
    TRY(append_utf8(U'b', s));  TEST_EQUAL(s.size(), 2u);   TEST_EQUAL(s, "ab");
    TRY(append_utf8(U'c', s));  TEST_EQUAL(s.size(), 3u);   TEST_EQUAL(s, "abc");
    TRY(append_utf8(U'α', s));  TEST_EQUAL(s.size(), 5u);   TEST_EQUAL(s, "abcα");
    TRY(append_utf8(U'β', s));  TEST_EQUAL(s.size(), 7u);   TEST_EQUAL(s, "abcαβ");
    TRY(append_utf8(U'γ', s));  TEST_EQUAL(s.size(), 9u);   TEST_EQUAL(s, "abcαβγ");
    TRY(append_utf8(U'€', s));  TEST_EQUAL(s.size(), 12u);  TEST_EQUAL(s, "abcαβγ€");

}

void test_rs_unicode_encoding_utf8_error_check() {

    std::string s;
    std::optional<std::size_t> pos;

    s = "";                            TRY(pos = utf8_error_check(s)); TEST(! pos);
    s = "hello world";                 TRY(pos = utf8_error_check(s)); TEST(! pos);
    s = "αβγδε";                       TRY(pos = utf8_error_check(s)); TEST(! pos);
    s = "∀∃√∫";                        TRY(pos = utf8_error_check(s)); TEST(! pos);
    s = "\U00010000\U0010ffff";        TRY(pos = utf8_error_check(s)); TEST(! pos);
    s = "αβγδε\x80αβγδε";              TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);
    s = "αβγδε\xbfαβγδε";              TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);
    s = "αβγδε\xc0αβγδε";              TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);
    s = "αβγδε\xc1αβγδε";              TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);
    s = "αβγδε\xf5αβγδε";              TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);
    s = "αβγδε\xffαβγδε";              TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);
    s = "αβγδε\xed\xa0\x80αβγδε";      TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);
    s = "αβγδε\xed\xbf\xbfαβγδε";      TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);
    s = "αβγδε\xf4\x90\x80\x80αβγδε";  TRY(pos = utf8_error_check(s)); TEST(pos); TEST_EQUAL(pos.value(), 10u);

    s = "";                            TRY(valid_utf8(s));
    s = "hello world";                 TRY(valid_utf8(s));
    s = "αβγδε";                       TRY(valid_utf8(s));
    s = "∀∃√∫";                        TRY(valid_utf8(s));
    s = "\U00010000\U0010ffff";        TRY(valid_utf8(s));
    s = "αβγδε\x80αβγδε";              TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: 80 ce b1 ce");
    s = "αβγδε\xbfαβγδε";              TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: bf ce b1 ce");
    s = "αβγδε\xc0αβγδε";              TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: c0 ce b1 ce");
    s = "αβγδε\xc1αβγδε";              TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: c1 ce b1 ce");
    s = "αβγδε\xf5αβγδε";              TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: f5 ce b1 ce");
    s = "αβγδε\xffαβγδε";              TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: ff ce b1 ce");
    s = "αβγδε\xed\xa0\x80αβγδε";      TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: ed a0 80 ce");
    s = "αβγδε\xed\xbf\xbfαβγδε";      TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: ed bf bf ce");
    s = "αβγδε\xf4\x90\x80\x80αβγδε";  TEST_THROW(valid_utf8(s), EncodingError, "UTF-8 encoding error at offset 10: f4 90 80 80");

}

void test_rs_unicode_encoding_utf16_error_check() {

    std::u16string t;
    std::optional<std::size_t> pos;

    t = u"";                                   TRY(pos = utf16_error_check(t)); TEST(! pos);
    t = u"hello world";                        TRY(pos = utf16_error_check(t)); TEST(! pos);
    t = u"αβγδε";                              TRY(pos = utf16_error_check(t)); TEST(! pos);
    t = u"∀∃√∫";                               TRY(pos = utf16_error_check(t)); TEST(! pos);
    t = u"\U00010000\U0010ffff";               TRY(pos = utf16_error_check(t)); TEST(! pos);
    t = {u'α',u'β',0xd800,u'γ',u'δ',};         TRY(pos = utf16_error_check(t)); TEST(pos); TEST_EQUAL(pos.value(), 2u);
    t = {u'α',u'β',0xdfff,u'γ',u'δ',};         TRY(pos = utf16_error_check(t)); TEST(pos); TEST_EQUAL(pos.value(), 2u);
    t = {u'α',u'β',0xdfff,0xd800,u'γ',u'δ',};  TRY(pos = utf16_error_check(t)); TEST(pos); TEST_EQUAL(pos.value(), 2u);

    t = u"";                                   TRY(valid_utf16(t));
    t = u"hello world";                        TRY(valid_utf16(t));
    t = u"αβγδε";                              TRY(valid_utf16(t));
    t = u"∀∃√∫";                               TRY(valid_utf16(t));
    t = u"\U00010000\U0010ffff";               TRY(valid_utf16(t));
    t = {u'α',u'β',0xd800,u'γ',u'δ',};         TEST_THROW(valid_utf16(t), EncodingError, "UTF-16 encoding error at offset 2: d800 03b3");
    t = {u'α',u'β',0xdfff,u'γ',u'δ',};         TEST_THROW(valid_utf16(t), EncodingError, "UTF-16 encoding error at offset 2: dfff 03b3");
    t = {u'α',u'β',0xdfff,0xd800,u'γ',u'δ',};  TEST_THROW(valid_utf16(t), EncodingError, "UTF-16 encoding error at offset 2: dfff d800");

}

void test_rs_unicode_encoding_utf32_error_check() {

    std::u32string u;
    std::optional<std::size_t> pos;

    u = U"";                                   TRY(pos = utf32_error_check(u)); TEST(! pos);
    u = U"hello world";                        TRY(pos = utf32_error_check(u)); TEST(! pos);
    u = U"αβγδε";                              TRY(pos = utf32_error_check(u)); TEST(! pos);
    u = U"∀∃√∫";                               TRY(pos = utf32_error_check(u)); TEST(! pos);
    u = U"\U00010000\U0010ffff";               TRY(pos = utf32_error_check(u)); TEST(! pos);
    u = {U'α',U'β',0xd800,U'γ',U'δ',};         TRY(pos = utf32_error_check(u)); TEST(pos); TEST_EQUAL(pos.value(), 2u);
    u = {U'α',U'β',0xdfff,U'γ',U'δ',};         TRY(pos = utf32_error_check(u)); TEST(pos); TEST_EQUAL(pos.value(), 2u);
    u = {U'α',U'β',0xdfff,0xd800,U'γ',U'δ',};  TRY(pos = utf32_error_check(u)); TEST(pos); TEST_EQUAL(pos.value(), 2u);
    u = {U'α',U'β',0x110000,U'γ',U'δ',};       TRY(pos = utf32_error_check(u)); TEST(pos); TEST_EQUAL(pos.value(), 2u);

    u = U"";                                   TRY(valid_utf32(u));
    u = U"hello world";                        TRY(valid_utf32(u));
    u = U"αβγδε";                              TRY(valid_utf32(u));
    u = U"∀∃√∫";                               TRY(valid_utf32(u));
    u = U"\U00010000\U0010ffff";               TRY(valid_utf32(u));
    u = {U'α',U'β',0xd800,U'γ',U'δ',};         TEST_THROW(valid_utf32(u), EncodingError, "UTF-32 encoding error at offset 2: 0000d800");
    u = {U'α',U'β',0xdfff,U'γ',U'δ',};         TEST_THROW(valid_utf32(u), EncodingError, "UTF-32 encoding error at offset 2: 0000dfff");
    u = {U'α',U'β',0xdfff,0xd800,U'γ',U'δ',};  TEST_THROW(valid_utf32(u), EncodingError, "UTF-32 encoding error at offset 2: 0000dfff");
    u = {U'α',U'β',0x110000,U'γ',U'δ',};       TEST_THROW(valid_utf32(u), EncodingError, "UTF-32 encoding error at offset 2: 00110000");

}

void test_rs_unicode_encoding_utf8_to_utf32() {

    std::string s;
    std::u32string u;

    s = "";             TRY(u = utf8_to_utf32(s));  TEST_EQUAL(u.size(), 0u);   TEST(u == U"");
    s = "Hello world";  TRY(u = utf8_to_utf32(s));  TEST_EQUAL(u.size(), 11u);  TEST(u == U"Hello world");
    s = "αβγδε";        TRY(u = utf8_to_utf32(s));  TEST_EQUAL(u.size(), 5u);   TEST(u == U"αβγδε");
    s = "€😀❤️‍🔥";       TRY(u = utf8_to_utf32(s));  TEST_EQUAL(u.size(), 6u);   TEST(u == U"€😀❤️‍🔥");

    s = "Hello\xffworld";
    TEST_THROW(utf8_to_utf32(s, Convert::checked), EncodingError,
        "UTF-8 encoding error at offset 5: ff 77 6f 72");
    s = "αβγδε\xe2\x82\xc0€😀\xff\xff\xff\xff\xff❤️‍🔥";
    TEST_THROW(utf8_to_utf32(s, Convert::checked), EncodingError,
        "UTF-8 encoding error at offset 10: e2 82 c0 e2");

    s = "Hello\xffworld";
    TRY(u = utf8_to_utf32(s, Convert::replace));
    TEST_EQUAL(u.size(), 11u);
    TEST(u == U"Hello\ufffdworld");
    s = "αβγδε\xe2\x82\xc0€😀\xff\xff\xff\xff\xff❤️‍🔥";
    TRY(u = utf8_to_utf32(s, Convert::replace));
    TEST_EQUAL(u.size(), 14u);
    TEST(u == U"αβγδε\ufffd€😀\ufffd\ufffd❤️‍🔥");

}

void test_rs_unicode_encoding_utf16_to_utf32() {

    std::u16string t;
    std::u32string u;

    t = u"";             TRY(u = utf16_to_utf32(t));  TEST_EQUAL(u.size(), 0u);   TEST(u == U"");
    t = u"Hello world";  TRY(u = utf16_to_utf32(t));  TEST_EQUAL(u.size(), 11u);  TEST(u == U"Hello world");
    t = u"αβγδε";        TRY(u = utf16_to_utf32(t));  TEST_EQUAL(u.size(), 5u);   TEST(u == U"αβγδε");
    t = u"€😀❤️‍🔥";       TRY(u = utf16_to_utf32(t));  TEST_EQUAL(u.size(), 6u);   TEST(u == U"€😀❤️‍🔥");

    t = u"Hello" + std::u16string{0xd800} + u"world";
    TEST_THROW(utf16_to_utf32(t, Convert::checked), EncodingError,
        "UTF-16 encoding error at offset 5: d800 0077");

    t = u"€😀❤️‍🔥";
    TRY(u = utf16_to_utf32(t, Convert::replace));
    TEST_EQUAL(u.size(), 6u);
    TEST(u == U"€😀❤️‍🔥");
    t = u"Hello" + std::u16string{0xd800} + u"world";
    TRY(u = utf16_to_utf32(t, Convert::replace));
    TEST_EQUAL(u.size(), 11u);
    TEST(u == U"Hello\ufffdworld");

}

void test_rs_unicode_encoding_utf32_to_utf8() {

    std::string s;
    std::u32string u;

    u = U"";             TRY(s = utf32_to_utf8(u));  TEST_EQUAL(s.size(), 0u);   TEST_EQUAL(s, "");
    u = U"Hello world";  TRY(s = utf32_to_utf8(u));  TEST_EQUAL(s.size(), 11u);  TEST_EQUAL(s, "Hello world");
    u = U"αβγδε";        TRY(s = utf32_to_utf8(u));  TEST_EQUAL(s.size(), 10u);  TEST_EQUAL(s, "αβγδε");
    u = U"€😀❤️‍🔥";       TRY(s = utf32_to_utf8(u));  TEST_EQUAL(s.size(), 20u);  TEST_EQUAL(s, "€😀❤️‍🔥");

    u = U"Hello" + std::u32string{0xd800} + U"world";
    TEST_THROW(utf32_to_utf8(u, Convert::checked), EncodingError,
        "UTF-32 encoding error at offset 5: 0000d800");

    u = U"Hello" + std::u32string{0xd800} + U"world";
    TRY(s = utf32_to_utf8(u, Convert::replace));
    TEST_EQUAL(s.size(), 13u);
    TEST_EQUAL(s, "Hello\xef\xbf\xbdworld");

}

void test_rs_unicode_encoding_utf32_to_utf16() {

    std::u16string t;
    std::u32string u;

    u = U"";             TRY(t = utf32_to_utf16(u));  TEST_EQUAL(t.size(), 0u);   TEST(t == u"");
    u = U"Hello world";  TRY(t = utf32_to_utf16(u));  TEST_EQUAL(t.size(), 11u);  TEST(t == u"Hello world");
    u = U"αβγδε";        TRY(t = utf32_to_utf16(u));  TEST_EQUAL(t.size(), 5u);   TEST(t == u"αβγδε");
    u = U"€😀❤️‍🔥";       TRY(t = utf32_to_utf16(u));  TEST_EQUAL(t.size(), 8u);   TEST(t == u"€😀❤️‍🔥");

    u = U"Hello" + std::u32string{0xd800} + U"world";
    TEST_THROW(utf32_to_utf16(u, Convert::checked), EncodingError,
        "UTF-32 encoding error at offset 5: 0000d800");

    u = U"Hello" + std::u32string{0xd800} + U"world";
    TRY(t = utf32_to_utf16(u, Convert::replace));
    TEST_EQUAL(t.size(), 11u);
    TEST(t == u"Hello\ufffdworld");

}
