#include "rs-unicode/string.hpp"
#include "rs-core/unit-test.hpp"
#include <optional>
#include <string>

using namespace RS::Unicode;

void test_rs_unicode_string_casing_to_lowercase() {

    TEST_EQUAL(to_lowercase(""),               "");
    TEST_EQUAL(to_lowercase("HELLO WORLD"),    "hello world");
    TEST_EQUAL(to_lowercase("hello world"),    "hello world");
    TEST_EQUAL(to_lowercase("Hello World"),    "hello world");
    TEST_EQUAL(to_lowercase("ΑΒΓΔΕ"),          "αβγδε");
    TEST_EQUAL(to_lowercase("αβγδε"),          "αβγδε");

}

void test_rs_unicode_string_casing_to_uppercase() {

    TEST_EQUAL(to_uppercase(""),               "");
    TEST_EQUAL(to_uppercase("HELLO WORLD"),    "HELLO WORLD");
    TEST_EQUAL(to_uppercase("hello world"),    "HELLO WORLD");
    TEST_EQUAL(to_uppercase("Hello World"),    "HELLO WORLD");
    TEST_EQUAL(to_uppercase("ΑΒΓΔΕ"),          "ΑΒΓΔΕ");
    TEST_EQUAL(to_uppercase("αβγδε"),          "ΑΒΓΔΕ");

}

void test_rs_unicode_string_casing_to_titlecase() {

    TEST_EQUAL(to_titlecase(""),                     "");
    TEST_EQUAL(to_titlecase("HELLO WORLD"),          "Hello World");
    TEST_EQUAL(to_titlecase("hello world"),          "Hello World");
    TEST_EQUAL(to_titlecase("Hello World"),          "Hello World");
    TEST_EQUAL(to_titlecase("123abc 456def789ghi"),  "123abc 456def789ghi");
    TEST_EQUAL(to_titlecase("abc123 def456ghi789"),  "Abc123 Def456ghi789");
    TEST_EQUAL(to_titlecase("ΑΒΓΔΕ"),                "Αβγδε");
    TEST_EQUAL(to_titlecase("αβγδε"),                "Αβγδε");
    TEST_EQUAL(to_titlecase("(hello-world)"),        "(Hello-world)");
    TEST_EQUAL(to_titlecase("ΜΜΜ ΜΜΜ"),              "Μμμ Μμμ");
    TEST_EQUAL(to_titlecase("µµµ µµµ"),              "Μµµ Μµµ");
    TEST_EQUAL(to_titlecase("ǄǄǄ ǄǄǄ"),              "ǅǆǆ ǅǆǆ");
    TEST_EQUAL(to_titlecase("ǅǅǅ ǅǅǅ"),              "ǅǆǆ ǅǆǆ");
    TEST_EQUAL(to_titlecase("ǆǆǆ ǆǆǆ"),              "ǅǆǆ ǅǆǆ");

}

void test_rs_unicode_string_casing_to_casefold() {

    TEST_EQUAL(to_casefold(""),                "");
    TEST_EQUAL(to_casefold("HELLO WORLD"),     "hello world");
    TEST_EQUAL(to_casefold("hello world"),     "hello world");
    TEST_EQUAL(to_casefold("Hello World"),     "hello world");
    TEST_EQUAL(to_casefold("αβγδε"),           "αβγδε");

}

void test_rs_unicode_string_subscripts_and_superscripts() {

    std::string s;
    std::optional<std::string> t;

    s = "0a";     TRY(t = to_subscript(s));    TEST(t.has_value()); TEST_EQUAL(t.value(), "\u2080\u2090");
    s = "0Aaz";   TRY(t = to_superscript(s));  TEST(t.has_value()); TEST_EQUAL(t.value(), "\u2070\u1d2c\u1d43\u1dbb");
    s = "0AZaz";  TRY(t = to_subscript(s));    TEST(! t.has_value());
    s = "0AZaz";  TRY(t = to_superscript(s));  TEST(! t.has_value());

}
