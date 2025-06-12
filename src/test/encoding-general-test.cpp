#include "rs-unicode/encoding.hpp"
#include "rs-core/unit-test.hpp"
#include <string_view>

using namespace RS::Unicode;
using namespace std::literals;

void test_rs_unicode_encoding_detail_hex_string() {

    using namespace Detail;

    TEST_EQUAL(hex_string(""sv),        "");
    TEST_EQUAL(hex_string("abcde"sv),   "61 62 63 64 65");
    TEST_EQUAL(hex_string("αβγδε"sv),   "ce b1 ce b2 ce b3 ce b4 ce b5");
    TEST_EQUAL(hex_string(u""sv),       "");
    TEST_EQUAL(hex_string(u"abcde"sv),  "0061 0062 0063 0064 0065");
    TEST_EQUAL(hex_string(u"αβγδε"sv),  "03b1 03b2 03b3 03b4 03b5");
    TEST_EQUAL(hex_string(U""sv),       "");
    TEST_EQUAL(hex_string(U"abcde"sv),  "00000061 00000062 00000063 00000064 00000065");
    TEST_EQUAL(hex_string(U"αβγδε"sv),  "000003b1 000003b2 000003b3 000003b4 000003b5");

}

void test_rs_unicode_encoding_utf8_code_units() {

    TEST_EQUAL(int(utf8_unit_kind('\x00')),  int(Utf8UnitKind::single));
    TEST_EQUAL(int(utf8_unit_kind('\x7f')),  int(Utf8UnitKind::single));
    TEST_EQUAL(int(utf8_unit_kind('\x80')),  int(Utf8UnitKind::trailing));
    TEST_EQUAL(int(utf8_unit_kind('\xbf')),  int(Utf8UnitKind::trailing));
    TEST_EQUAL(int(utf8_unit_kind('\xc0')),  int(Utf8UnitKind::illegal));
    TEST_EQUAL(int(utf8_unit_kind('\xc1')),  int(Utf8UnitKind::illegal));
    TEST_EQUAL(int(utf8_unit_kind('\xc2')),  int(Utf8UnitKind::leading2));
    TEST_EQUAL(int(utf8_unit_kind('\xdf')),  int(Utf8UnitKind::leading2));
    TEST_EQUAL(int(utf8_unit_kind('\xe0')),  int(Utf8UnitKind::leading3));
    TEST_EQUAL(int(utf8_unit_kind('\xef')),  int(Utf8UnitKind::leading3));
    TEST_EQUAL(int(utf8_unit_kind('\xf0')),  int(Utf8UnitKind::leading4));
    TEST_EQUAL(int(utf8_unit_kind('\xf4')),  int(Utf8UnitKind::leading4));
    TEST_EQUAL(int(utf8_unit_kind('\xf5')),  int(Utf8UnitKind::illegal));
    TEST_EQUAL(int(utf8_unit_kind('\xff')),  int(Utf8UnitKind::illegal));

    TEST(is_utf8_leading_unit('\x00'));
    TEST(is_utf8_leading_unit('\x7f'));
    TEST(! is_utf8_leading_unit('\x80'));
    TEST(! is_utf8_leading_unit('\xbf'));
    TEST(! is_utf8_leading_unit('\xc0'));
    TEST(! is_utf8_leading_unit('\xc1'));
    TEST(is_utf8_leading_unit('\xc2'));
    TEST(is_utf8_leading_unit('\xdf'));
    TEST(is_utf8_leading_unit('\xe0'));
    TEST(is_utf8_leading_unit('\xef'));
    TEST(is_utf8_leading_unit('\xf0'));
    TEST(is_utf8_leading_unit('\xf4'));
    TEST(! is_utf8_leading_unit('\xf5'));
    TEST(! is_utf8_leading_unit('\xff'));

}

void test_rs_unicode_encoding_utf8_length() {

    TEST_EQUAL(utf8_length(U'\u0000'),      1u);
    TEST_EQUAL(utf8_length(U'\u007f'),      1u);
    TEST_EQUAL(utf8_length(U'\u0080'),      2u);
    TEST_EQUAL(utf8_length(U'\u00ff'),      2u);
    TEST_EQUAL(utf8_length(U'\u0100'),      2u);
    TEST_EQUAL(utf8_length(U'\u07ff'),      2u);
    TEST_EQUAL(utf8_length(U'\u0800'),      3u);
    TEST_EQUAL(utf8_length(U'\u0fff'),      3u);
    TEST_EQUAL(utf8_length(U'\u1000'),      3u);
    TEST_EQUAL(utf8_length(U'\ud7ff'),      3u);
    TEST_EQUAL(utf8_length(min_surrogate),  0u);
    TEST_EQUAL(utf8_length(max_surrogate),  0u);
    TEST_EQUAL(utf8_length(U'\ue000'),      3u);
    TEST_EQUAL(utf8_length(U'\uffff'),      3u);
    TEST_EQUAL(utf8_length(U'\U00010000'),  4u);
    TEST_EQUAL(utf8_length(U'\U000fffff'),  4u);
    TEST_EQUAL(utf8_length(U'\U00100000'),  4u);
    TEST_EQUAL(utf8_length(max_unicode),    4u);
    TEST_EQUAL(utf8_length(not_unicode),    0u);

    TEST_EQUAL(utf8_length('\x00'), 1u);
    TEST_EQUAL(utf8_length('\x7f'), 1u);
    TEST_EQUAL(utf8_length('\x80'), 0u);
    TEST_EQUAL(utf8_length('\xbf'), 0u);
    TEST_EQUAL(utf8_length('\xc0'), 0u);
    TEST_EQUAL(utf8_length('\xc1'), 0u);
    TEST_EQUAL(utf8_length('\xc2'), 2u);
    TEST_EQUAL(utf8_length('\xdf'), 2u);
    TEST_EQUAL(utf8_length('\xe0'), 3u);
    TEST_EQUAL(utf8_length('\xef'), 3u);
    TEST_EQUAL(utf8_length('\xf0'), 4u);
    TEST_EQUAL(utf8_length('\xf4'), 4u);
    TEST_EQUAL(utf8_length('\xf5'), 0u);
    TEST_EQUAL(utf8_length('\xff'), 0u);

}

void test_rs_unicode_encoding_utf16_code_units() {

    TEST_EQUAL(int(utf16_unit_kind(u'\x0000')),  int(Utf16UnitKind::single));
    TEST_EQUAL(int(utf16_unit_kind(u'\xd7ff')),  int(Utf16UnitKind::single));
    TEST_EQUAL(int(utf16_unit_kind(u'\xd800')),  int(Utf16UnitKind::high));
    TEST_EQUAL(int(utf16_unit_kind(u'\xdbff')),  int(Utf16UnitKind::high));
    TEST_EQUAL(int(utf16_unit_kind(u'\xdc00')),  int(Utf16UnitKind::low));
    TEST_EQUAL(int(utf16_unit_kind(u'\xdfff')),  int(Utf16UnitKind::low));
    TEST_EQUAL(int(utf16_unit_kind(u'\xe000')),  int(Utf16UnitKind::single));
    TEST_EQUAL(int(utf16_unit_kind(u'\xffff')),  int(Utf16UnitKind::single));

}

void test_rs_unicode_encoding_utf16_length() {

    TEST_EQUAL(utf16_length(U'\x0000'),      1u);
    TEST_EQUAL(utf16_length(U'\xd7ff'),      1u);
    TEST_EQUAL(utf8_length(min_surrogate),   0u);
    TEST_EQUAL(utf8_length(max_surrogate),   0u);
    TEST_EQUAL(utf16_length(U'\xe000'),      1u);
    TEST_EQUAL(utf16_length(U'\xffff'),      1u);
    TEST_EQUAL(utf16_length(U'\U00010000'),  2u);
    TEST_EQUAL(utf16_length(U'\U000fffff'),  2u);
    TEST_EQUAL(utf16_length(U'\U00100000'),  2u);
    TEST_EQUAL(utf16_length(max_unicode),    2u);
    TEST_EQUAL(utf16_length(not_unicode),    0u);

}
