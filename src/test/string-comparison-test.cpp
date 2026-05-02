#include "rs-unicode/string.hpp"
#include "rs-core/unit-test.hpp"
#include <algorithm>
#include <format>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

using namespace RS::Unicode;

void test_rs_unicode_string_comparison_dynamic_sorting() {

    StringCompare compare;
    std::vector<std::string> vec;

    vec = {
        "charlie",
        "bravo",
        "alpha",
        "Charlie",
        "Bravo",
        "Alpha",
    };

    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"Alpha\", "
        "\"Bravo\", "
        "\"Charlie\", "
        "\"alpha\", "
        "\"bravo\", "
        "\"charlie\"]"
    );

    TRY(compare = StringCompare{Sort::reverse});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"charlie\", "
        "\"bravo\", "
        "\"alpha\", "
        "\"Charlie\", "
        "\"Bravo\", "
        "\"Alpha\"]"
    );

    TRY(compare = StringCompare{Sort::icase});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"Alpha\", "
        "\"alpha\", "
        "\"Bravo\", "
        "\"bravo\", "
        "\"Charlie\", "
        "\"charlie\"]"
    );

    TRY(compare = StringCompare{Sort::icase | Sort::reverse});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"charlie\", "
        "\"Charlie\", "
        "\"bravo\", "
        "\"Bravo\", "
        "\"alpha\", "
        "\"Alpha\"]"
    );

    vec = {
        "charlie",
        "bravo",
        "alpha",
        "\r\ncharlie\r\n",
        "\r\nbravo\r\n",
        "\r\nalpha\r\n",
    };

    TRY(compare = StringCompare{Sort::trim});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"\\r\\nalpha\\r\\n\", "
        "\"alpha\", "
        "\"\\r\\nbravo\\r\\n\", "
        "\"bravo\", "
        "\"\\r\\ncharlie\\r\\n\", "
        "\"charlie\"]"
    );

    // 00E1 LATIN SMALL LETTER A WITH ACUTE
    // 00E9 LATIN SMALL LETTER E WITH ACUTE
    // 00ED LATIN SMALL LETTER I WITH ACUTE
    // 00F3 LATIN SMALL LETTER O WITH ACUTE
    // 00FA LATIN SMALL LETTER U WITH ACUTE
    // 0301 COMBINING ACUTE ACCENT

    vec = {
        "aeiou",
        "uoiea",
        "a\u0301e\u0301i\u0301o\u0301u\u0301",  // decomposed
        "u\u0301o\u0301i\u0301e\u0301a\u0301",  // decomposed
        "\u00e1\u00e9\u00ed\u00f3\u00fa",       // composed
        "\u00fa\u00f3\u00ed\u00e9\u00e1",       // composed
    };

    TRY(compare = StringCompare{});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"aeiou\", "
        "\"a\u0301e\u0301i\u0301o\u0301u\u0301\", "
        "\"uoiea\", "
        "\"u\u0301o\u0301i\u0301e\u0301a\u0301\", "
        "\"\u00e1\u00e9\u00ed\u00f3\u00fa\", "
        "\"\u00fa\u00f3\u00ed\u00e9\u00e1\"]"
    );

    TRY(compare = StringCompare{Sort::nfc});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"aeiou\", "
        "\"uoiea\", "
        "\"a\u0301e\u0301i\u0301o\u0301u\u0301\", "
        "\"\u00e1\u00e9\u00ed\u00f3\u00fa\", "
        "\"u\u0301o\u0301i\u0301e\u0301a\u0301\", "
        "\"\u00fa\u00f3\u00ed\u00e9\u00e1\"]"
    );

    TRY(compare = StringCompare{Sort::nfd});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"aeiou\", "
        "\"a\u0301e\u0301i\u0301o\u0301u\u0301\", "
        "\"\u00e1\u00e9\u00ed\u00f3\u00fa\", "
        "\"uoiea\", "
        "\"u\u0301o\u0301i\u0301e\u0301a\u0301\", "
        "\"\u00fa\u00f3\u00ed\u00e9\u00e1\"]"
    );

    vec = {
        "abc567xyz",
        "abc89xyz",
        "abc1234xyz",
    };

    TRY(compare = StringCompare{});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"abc1234xyz\", "
        "\"abc567xyz\", "
        "\"abc89xyz\"]"
    );

    TRY(compare = StringCompare{Sort::numeric});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"abc89xyz\", "
        "\"abc567xyz\", "
        "\"abc1234xyz\"]"
    );

    vec = {
        "abc-123xyz",
        "abc+123xyz",
        "abc-456xyz",
        "abc+456xyz",
    };

    TRY(compare = StringCompare{Sort::numeric});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"abc+123xyz\", "
        "\"abc+456xyz\", "
        "\"abc-123xyz\", "
        "\"abc-456xyz\"]"
    );

    TRY(compare = StringCompare{Sort::sign});
    TRY(std::ranges::sort(vec, compare));
    TEST_EQUAL(std::format("{}", vec),
        "[\"abc-456xyz\", "
        "\"abc-123xyz\", "
        "\"abc+123xyz\", "
        "\"abc+456xyz\"]"
    );

    TEST_THROW(compare = StringCompare{Sort::nfc | Sort::nfd}, std::invalid_argument, "Inconsistent normalization");

}

void test_rs_unicode_string_comparison_static_sorting() {

    std::vector<std::string> vec;

    vec = {
        "charlie",
        "bravo",
        "alpha",
        "Charlie",
        "Bravo",
        "Alpha",
    };

    TRY(std::ranges::sort(vec, BasicStringCompare<>{}));
    TEST_EQUAL(std::format("{}", vec),
        "[\"Alpha\", "
        "\"Bravo\", "
        "\"Charlie\", "
        "\"alpha\", "
        "\"bravo\", "
        "\"charlie\"]"
    );

    TRY(std::ranges::sort(vec, BasicStringCompare<Sort::reverse>{}));
    TEST_EQUAL(std::format("{}", vec),
        "[\"charlie\", "
        "\"bravo\", "
        "\"alpha\", "
        "\"Charlie\", "
        "\"Bravo\", "
        "\"Alpha\"]"
    );

    TRY(std::ranges::sort(vec, BasicStringCompare<Sort::icase>{}));
    TEST_EQUAL(std::format("{}", vec),
        "[\"Alpha\", "
        "\"alpha\", "
        "\"Bravo\", "
        "\"bravo\", "
        "\"Charlie\", "
        "\"charlie\"]"
    );

    TRY(std::ranges::sort(vec, BasicStringCompare<Sort::icase | Sort::reverse>{}));
    TEST_EQUAL(std::format("{}", vec),
        "[\"charlie\", "
        "\"Charlie\", "
        "\"bravo\", "
        "\"Bravo\", "
        "\"alpha\", "
        "\"Alpha\"]"
    );

}

void test_rs_unicode_string_comparison_common_prefix_and_suffix() {

    TEST_EQUAL(common_prefix("",             ""),             "");
    TEST_EQUAL(common_prefix("hello",        ""),             "");
    TEST_EQUAL(common_prefix("",             "world"),        "");
    TEST_EQUAL(common_prefix("hello",        "world"),        "");
    TEST_EQUAL(common_prefix("hello",        "hello"),        "hello");
    TEST_EQUAL(common_prefix("hello",        "hello world"),  "hello");
    TEST_EQUAL(common_prefix("hello world",  "hello moon"),   "hello ");
    TEST_EQUAL(common_prefix("hello αβγ",    "hello δεζ"),    "hello ");
    TEST_EQUAL(common_prefix("αβγδεζ",       "αβγηθι"),       "αβγ");

    TEST_EQUAL(common_suffix("",               ""),             "");
    TEST_EQUAL(common_suffix("hello",          ""),             "");
    TEST_EQUAL(common_suffix("",               "hello"),        "");
    TEST_EQUAL(common_suffix("hello",          "world"),        "");
    TEST_EQUAL(common_suffix("hello",          "hello"),        "hello");
    TEST_EQUAL(common_suffix("hello world",    "world"),        "world");
    TEST_EQUAL(common_suffix("goodbye world",  "hello world"),  " world");
    TEST_EQUAL(common_suffix("αβγδεζ",         "ηθιδεζ"),       "δεζ");

}
