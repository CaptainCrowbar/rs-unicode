#include "rs-unicode/string.hpp"
#include "rs-core/unit-test.hpp"

using namespace RS::Unicode;

void test_rs_unicode_string_properties_utf8_length() {

    TEST_EQUAL(length("",             Unit::bytes),  0u);
    TEST_EQUAL(length("Hello world",  Unit::bytes),  11u);
    TEST_EQUAL(length("αβγδε",        Unit::bytes),  10u);
    TEST_EQUAL(length("Greek αβγδε",  Unit::bytes),  16u);
    TEST_EQUAL(length("😀👍👩",      Unit::bytes),  12u);
    TEST_EQUAL(length("😀👍🏽👩🏽",      Unit::bytes),  20u);
    TEST_EQUAL(length("🇳🇿🇺🇸🇩🇪🇦🇺",    Unit::bytes),  32u);

}

void test_rs_unicode_string_properties_utf16_length() {

    TEST_EQUAL(length("",             Unit::utf16),  0u);
    TEST_EQUAL(length("Hello world",  Unit::utf16),  11u);
    TEST_EQUAL(length("αβγδε",        Unit::utf16),  5u);
    TEST_EQUAL(length("Greek αβγδε",  Unit::utf16),  11u);
    TEST_EQUAL(length("😀👍👩",      Unit::utf16),  6u);
    TEST_EQUAL(length("😀👍🏽👩🏽",      Unit::utf16),  10u);
    TEST_EQUAL(length("🇳🇿🇺🇸🇩🇪🇦🇺",    Unit::utf16),  16u);

}

void test_rs_unicode_string_properties_utf32_length() {

    TEST_EQUAL(length(""),             0u);
    TEST_EQUAL(length("Hello world"),  11u);
    TEST_EQUAL(length("αβγδε"),        5u);
    TEST_EQUAL(length("Greek αβγδε"),  11u);
    TEST_EQUAL(length("😀👍👩"),      3u);
    TEST_EQUAL(length("😀👍🏽👩🏽"),      5u);
    TEST_EQUAL(length("🇳🇿🇺🇸🇩🇪🇦🇺"),    8u);

}

void test_rs_unicode_string_properties_grapheme_length() {

    TEST_EQUAL(length("",                                     Unit::graphemes),  0u);   // empty
    TEST_EQUAL(length("Hello world",                          Unit::graphemes),  11u);  // ascii
    TEST_EQUAL(length("aeiou",                                Unit::graphemes),  5u);   // narrow
    TEST_EQUAL(length("a\u0301e\u0301i\u0301o\u0301u\u0301",  Unit::graphemes),  5u);   // decomposed narrow
    TEST_EQUAL(length("ÀÀÀ",                                  Unit::graphemes),  3u);   // neutral
    TEST_EQUAL(length("ààà",                                  Unit::graphemes),  3u);   // ambiguous
    TEST_EQUAL(length("\u3000\u3000\u3000",                   Unit::graphemes),  3u);   // fullwidth
    TEST_EQUAL(length("\u20a9\u20a9\u20a9",                   Unit::graphemes),  3u);   // halfwidth
    TEST_EQUAL(length("AAA",                                  Unit::graphemes),  3u);   // narrow
    TEST_EQUAL(length("\u3001\u3001\u3001",                   Unit::graphemes),  3u);   // wide
    TEST_EQUAL(length("àààÀÀÀ",                               Unit::graphemes),  6u);   // ambiguous + neutral
    TEST_EQUAL(length("ààà\u3000\u3000\u3000",                Unit::graphemes),  6u);   // ambiguous + fullwidth
    TEST_EQUAL(length("ààà\u20a9\u20a9\u20a9",                Unit::graphemes),  6u);   // ambiguous + halfwidth
    TEST_EQUAL(length("àààAAA",                               Unit::graphemes),  6u);   // ambiguous + narrow
    TEST_EQUAL(length("ààà\u3001\u3001\u3001",                Unit::graphemes),  6u);   // ambiguous + wide
    TEST_EQUAL(length("😀👍👩",                              Unit::graphemes),  3u);   // simple emoji
    TEST_EQUAL(length("😀👍🏽👩🏽",                              Unit::graphemes),  3u);   // modified emoji
    TEST_EQUAL(length("🇳🇿🇺🇸🇩🇪🇦🇺",                            Unit::graphemes),  4u);   // flags

}

void test_rs_unicode_string_properties_column_length() {

    TEST_EQUAL(length("",                                     Unit::columns),  0u);   // empty
    TEST_EQUAL(length("Hello world",                          Unit::columns),  11u);  // ascii
    TEST_EQUAL(length("aeiou",                                Unit::columns),  5u);   // narrow
    TEST_EQUAL(length("a\u0301e\u0301i\u0301o\u0301u\u0301",  Unit::columns),  5u);   // decomposed narrow
    TEST_EQUAL(length("ÀÀÀ",                                  Unit::columns),  3u);   // neutral
    TEST_EQUAL(length("ààà",                                  Unit::columns),  3u);   // ambiguous
    TEST_EQUAL(length("\u3000\u3000\u3000",                   Unit::columns),  6u);   // fullwidth
    TEST_EQUAL(length("\u20a9\u20a9\u20a9",                   Unit::columns),  3u);   // halfwidth
    TEST_EQUAL(length("AAA",                                  Unit::columns),  3u);   // narrow
    TEST_EQUAL(length("\u3001\u3001\u3001",                   Unit::columns),  6u);   // wide
    TEST_EQUAL(length("àààÀÀÀ",                               Unit::columns),  6u);   // ambiguous + neutral
    TEST_EQUAL(length("ààà\u3000\u3000\u3000",                Unit::columns),  9u);   // ambiguous + fullwidth
    TEST_EQUAL(length("ààà\u20a9\u20a9\u20a9",                Unit::columns),  6u);   // ambiguous + halfwidth
    TEST_EQUAL(length("àààAAA",                               Unit::columns),  6u);   // ambiguous + narrow
    TEST_EQUAL(length("ààà\u3001\u3001\u3001",                Unit::columns),  9u);   // ambiguous + wide
    TEST_EQUAL(length("😀👍👩",                              Unit::columns),  6u);   // simple emoji
    TEST_EQUAL(length("😀👍🏽👩🏽",                              Unit::columns),  6u);   // modified emoji
    TEST_EQUAL(length("🇳🇿🇺🇸🇩🇪🇦🇺",                            Unit::columns),  8u);   // flags

}

void test_rs_unicode_string_properties_wide_column_length() {

    TEST_EQUAL(length("",                                     Unit::wide),  0u);   // empty
    TEST_EQUAL(length("Hello world",                          Unit::wide),  11u);  // ascii
    TEST_EQUAL(length("aeiou",                                Unit::wide),  5u);   // narrow
    TEST_EQUAL(length("a\u0301e\u0301i\u0301o\u0301u\u0301",  Unit::wide),  5u);   // decomposed narrow
    TEST_EQUAL(length("ÀÀÀ",                                  Unit::wide),  3u);   // neutral
    TEST_EQUAL(length("ààà",                                  Unit::wide),  6u);   // ambiguous
    TEST_EQUAL(length("\u3000\u3000\u3000",                   Unit::wide),  6u);   // fullwidth
    TEST_EQUAL(length("\u20a9\u20a9\u20a9",                   Unit::wide),  3u);   // halfwidth
    TEST_EQUAL(length("AAA",                                  Unit::wide),  3u);   // narrow
    TEST_EQUAL(length("\u3001\u3001\u3001",                   Unit::wide),  6u);   // wide
    TEST_EQUAL(length("àààÀÀÀ",                               Unit::wide),  9u);   // ambiguous + neutral
    TEST_EQUAL(length("ààà\u3000\u3000\u3000",                Unit::wide),  12u);  // ambiguous + fullwidth
    TEST_EQUAL(length("ààà\u20a9\u20a9\u20a9",                Unit::wide),  9u);   // ambiguous + halfwidth
    TEST_EQUAL(length("àààAAA",                               Unit::wide),  9u);   // ambiguous + narrow
    TEST_EQUAL(length("ààà\u3001\u3001\u3001",                Unit::wide),  12u);  // ambiguous + wide
    TEST_EQUAL(length("😀👍👩",                              Unit::wide),  6u);   // simple emoji
    TEST_EQUAL(length("😀👍🏽👩🏽",                              Unit::wide),  6u);   // modified emoji
    TEST_EQUAL(length("🇳🇿🇺🇸🇩🇪🇦🇺",                            Unit::wide),  8u);   // flags

}
