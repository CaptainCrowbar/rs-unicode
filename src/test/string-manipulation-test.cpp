#include "rs-unicode/string.hpp"
#include "rs-unicode/character.hpp"
#include "rs-core/unit-test.hpp"
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace RS::Unicode;
using namespace std::literals;

void test_rs_unicode_string_manipulation_join() {

    std::vector<std::string> v;

    v = {};                           TEST_EQUAL(join(v),        "");
    v = {"abc"};                      TEST_EQUAL(join(v),        "abc");
    v = {"abc", "def"};               TEST_EQUAL(join(v),        "abcdef");
    v = {"abc", "def", "ghi"};        TEST_EQUAL(join(v),        "abcdefghi");
    v = {"abc", "", "ghi"};           TEST_EQUAL(join(v),        "abcghi");
    v = {"abc", "🇳🇿🇺🇸🇩🇪🇦🇺", "ghi"};  TEST_EQUAL(join(v),        "abc🇳🇿🇺🇸🇩🇪🇦🇺ghi");
    v = {};                           TEST_EQUAL(join(v, "/"),   "");
    v = {"abc"};                      TEST_EQUAL(join(v, "/"),   "abc");
    v = {"abc", "def"};               TEST_EQUAL(join(v, "/"),   "abc/def");
    v = {"abc", "def", "ghi"};        TEST_EQUAL(join(v, "/"),   "abc/def/ghi");
    v = {"abc", "", "ghi"};           TEST_EQUAL(join(v, "/"),   "abc//ghi");
    v = {"abc", "🇳🇿🇺🇸🇩🇪🇦🇺", "ghi"};  TEST_EQUAL(join(v, "/"),   "abc/🇳🇿🇺🇸🇩🇪🇦🇺/ghi");
    v = {};                           TEST_EQUAL(join(v, "😀"),  "");
    v = {"abc"};                      TEST_EQUAL(join(v, "😀"),  "abc");
    v = {"abc", "def"};               TEST_EQUAL(join(v, "😀"),  "abc😀def");
    v = {"abc", "def", "ghi"};        TEST_EQUAL(join(v, "😀"),  "abc😀def😀ghi");
    v = {"abc", "", "ghi"};           TEST_EQUAL(join(v, "😀"),  "abc😀😀ghi");
    v = {"abc", "🇳🇿🇺🇸🇩🇪🇦🇺", "ghi"};  TEST_EQUAL(join(v, "😀"),  "abc😀🇳🇿🇺🇸🇩🇪🇦🇺😀ghi");

}

void test_rs_unicode_string_manipulation_padding() {

    TEST_EQUAL(pad_left("", 0, U'*'),                "");
    TEST_EQUAL(pad_left("", 10, U'*'),               "**********");
    TEST_EQUAL(pad_left("Hello world", 0, U'*'),     "Hello world");
    TEST_EQUAL(pad_left("Hello world", 10, U'*'),    "Hello world");
    TEST_EQUAL(pad_left("Hello world", 20, U'*'),    "*********Hello world");
    TEST_EQUAL(pad_left("Hello world", 20, U'😀'),   "😀😀😀😀Hello world");
    TEST_EQUAL(pad_right("", 0, U'*'),               "");
    TEST_EQUAL(pad_right("", 10, U'*'),              "**********");
    TEST_EQUAL(pad_right("Hello world", 0, U'*'),    "Hello world");
    TEST_EQUAL(pad_right("Hello world", 10, U'*'),   "Hello world");
    TEST_EQUAL(pad_right("Hello world", 20, U'*'),   "Hello world*********");
    TEST_EQUAL(pad_right("Hello world", 20, U'😀'),  "Hello world😀😀😀😀");

}

void test_rs_unicode_string_manipulation_partition() {

    std::pair<std::string_view, std::string_view> p;

    TRY(p = partition(""));                             TEST_EQUAL(p.first, "");             TEST_EQUAL(p.second, "");
    TRY(p = partition("hello"));                        TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "");
    TRY(p = partition("hello "));                       TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "");
    TRY(p = partition("hello world"));                  TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "world");
    TRY(p = partition("hello\r\nworld"));               TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "world");
    TRY(p = partition(" hello world "));                TEST_EQUAL(p.first, "");             TEST_EQUAL(p.second, "hello world ");
    TRY(p = partition("abc def ghi jkl"));              TEST_EQUAL(p.first, "abc");          TEST_EQUAL(p.second, "def ghi jkl");
    TRY(p = partition("abc\r\ndef\r\nghi\r\njkl"));     TEST_EQUAL(p.first, "abc");          TEST_EQUAL(p.second, "def\r\nghi\r\njkl");
    TRY(p = partition("", ";"));                        TEST_EQUAL(p.first, "");             TEST_EQUAL(p.second, "");
    TRY(p = partition("hello", ";"));                   TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "");
    TRY(p = partition("hello;", ";"));                  TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "");
    TRY(p = partition("hello world", ";"));             TEST_EQUAL(p.first, "hello world");  TEST_EQUAL(p.second, "");
    TRY(p = partition("hello;world", ";"));             TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "world");
    TRY(p = partition(" hello ; world ", ";"));         TEST_EQUAL(p.first, " hello ");      TEST_EQUAL(p.second, " world ");
    TRY(p = partition("hello;;;world", ";"));           TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, ";;world");
    TRY(p = partition(";hello;world;", ";"));           TEST_EQUAL(p.first, "");             TEST_EQUAL(p.second, "hello;world;");
    TRY(p = partition("abc;def;ghi;jkl", ";"));         TEST_EQUAL(p.first, "abc");          TEST_EQUAL(p.second, "def;ghi;jkl");
    TRY(p = partition("abc;;;def;;;ghi;;;jkl", ";"));   TEST_EQUAL(p.first, "abc");          TEST_EQUAL(p.second, ";;def;;;ghi;;;jkl");
    TRY(p = partition("", U";"));                       TEST_EQUAL(p.first, "");             TEST_EQUAL(p.second, "");
    TRY(p = partition("hello", U";"));                  TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "");
    TRY(p = partition("hello;", U";"));                 TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "");
    TRY(p = partition("hello world", U";"));            TEST_EQUAL(p.first, "hello world");  TEST_EQUAL(p.second, "");
    TRY(p = partition("hello;world", U";"));            TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "world");
    TRY(p = partition(" hello ; world ", U";"));        TEST_EQUAL(p.first, " hello ");      TEST_EQUAL(p.second, " world ");
    TRY(p = partition("hello;;;world", U";"));          TEST_EQUAL(p.first, "hello");        TEST_EQUAL(p.second, "world");
    TRY(p = partition(";hello;world;", U";"));          TEST_EQUAL(p.first, "");             TEST_EQUAL(p.second, "hello;world;");
    TRY(p = partition("abc;def;ghi;jkl", U";"));        TEST_EQUAL(p.first, "abc");          TEST_EQUAL(p.second, "def;ghi;jkl");
    TRY(p = partition("abc;;;def;;;ghi;;;jkl", U";"));  TEST_EQUAL(p.first, "abc");          TEST_EQUAL(p.second, "def;;;ghi;;;jkl");

}

void test_rs_unicode_string_manipulation_repeat() {

    TEST_EQUAL(repeat(U'A', 0),     "");
    TEST_EQUAL(repeat(U'A', 1),     "A");
    TEST_EQUAL(repeat(U'A', 2),     "AA");
    TEST_EQUAL(repeat(U'A', 3),     "AAA");
    TEST_EQUAL(repeat(U'A', 4),     "AAAA");
    TEST_EQUAL(repeat(U'A', 5),     "AAAAA");
    TEST_EQUAL(repeat(U'A', 6),     "AAAAAA");
    TEST_EQUAL(repeat(U'A', 7),     "AAAAAAA");
    TEST_EQUAL(repeat(U'A', 8),     "AAAAAAAA");
    TEST_EQUAL(repeat(U'A', 9),     "AAAAAAAAA");
    TEST_EQUAL(repeat(U'A', 10),    "AAAAAAAAAA");
    TEST_EQUAL(repeat(U'😀', 0),    "");
    TEST_EQUAL(repeat(U'😀', 1),    "😀");
    TEST_EQUAL(repeat(U'😀', 2),    "😀😀");
    TEST_EQUAL(repeat(U'😀', 3),    "😀😀😀");
    TEST_EQUAL(repeat(U'😀', 4),    "😀😀😀😀");
    TEST_EQUAL(repeat(U'😀', 5),    "😀😀😀😀😀");
    TEST_EQUAL(repeat(U'😀', 6),    "😀😀😀😀😀😀");
    TEST_EQUAL(repeat(U'😀', 7),    "😀😀😀😀😀😀😀");
    TEST_EQUAL(repeat(U'😀', 8),    "😀😀😀😀😀😀😀😀");
    TEST_EQUAL(repeat(U'😀', 9),    "😀😀😀😀😀😀😀😀😀");
    TEST_EQUAL(repeat(U'😀', 10),   "😀😀😀😀😀😀😀😀😀😀");
    TEST_EQUAL(repeat("abc", 0),    "");
    TEST_EQUAL(repeat("abc", 1),    "abc");
    TEST_EQUAL(repeat("abc", 2),    "abcabc");
    TEST_EQUAL(repeat("abc", 3),    "abcabcabc");
    TEST_EQUAL(repeat("abc", 4),    "abcabcabcabc");
    TEST_EQUAL(repeat("abc", 5),    "abcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 6),    "abcabcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 7),    "abcabcabcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 8),    "abcabcabcabcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 9),    "abcabcabcabcabcabcabcabcabc");
    TEST_EQUAL(repeat("abc", 10),   "abcabcabcabcabcabcabcabcabcabc");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 0),   "");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 1),   "🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 2),   "🇩🇪🇦🇺🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 3),   "🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 4),   "🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 5),   "🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 6),   "🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 7),   "🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 8),   "🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 9),   "🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺");
    TEST_EQUAL(repeat("🇩🇪🇦🇺", 10),  "🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺🇩🇪🇦🇺");

}

void test_rs_unicode_string_manipulation_replace() {

    TEST_EQUAL(replace("", "", ""),                                "");
    TEST_EQUAL(replace("", "abc", "XYZ"),                          "");
    TEST_EQUAL(replace("abcdef", "uvw", "XYZ"),                    "abcdef");
    TEST_EQUAL(replace("abcdef", "abc", "XYZ"),                    "XYZdef");
    TEST_EQUAL(replace("abcdefabcdef", "abc", "XYZ"),              "XYZdefXYZdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "XYZ"),        "XYZdefXYZdefXYZdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "def", "XYZ"),        "abcXYZabcXYZabcXYZ");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "XYZ", 0),     "abcdefabcdefabcdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "XYZ", 1),     "XYZdefabcdefabcdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "XYZ", 2),     "XYZdefXYZdefabcdef");
    TEST_EQUAL(replace("abcdef", "abc", ""),                       "def");
    TEST_EQUAL(replace("abcdefabcdef", "abc", ""),                 "defdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", ""),           "defdefdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "def", ""),           "abcabcabc");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "", 1),        "defabcdefabcdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "", 2),        "defdefabcdef");
    TEST_EQUAL(replace("abcdef", "abc", "XX"),                     "XXdef");
    TEST_EQUAL(replace("abcdefabcdef", "abc", "XX"),               "XXdefXXdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "XX"),         "XXdefXXdefXXdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "def", "XX"),         "abcXXabcXXabcXX");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "XX", 1),      "XXdefabcdefabcdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "XX", 2),      "XXdefXXdefabcdef");
    TEST_EQUAL(replace("abcdef", "abc", "UVWXYZ"),                 "UVWXYZdef");
    TEST_EQUAL(replace("abcdefabcdef", "abc", "UVWXYZ"),           "UVWXYZdefUVWXYZdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "UVWXYZ"),     "UVWXYZdefUVWXYZdefUVWXYZdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "def", "UVWXYZ"),     "abcUVWXYZabcUVWXYZabcUVWXYZ");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "UVWXYZ", 1),  "UVWXYZdefabcdefabcdef");
    TEST_EQUAL(replace("abcdefabcdefabcdef", "abc", "UVWXYZ", 2),  "UVWXYZdefUVWXYZdefabcdef");

}

void test_rs_unicode_string_manipulation_trim() {

    TEST_EQUAL(trim(""),                               "");
    TEST_EQUAL(trim("\r\n"),                           "");
    TEST_EQUAL(trim("\u3000"),                         "");
    TEST_EQUAL(trim("hello world"),                    "hello world");
    TEST_EQUAL(trim("\r\nhello world\r\n"),            "hello world");
    TEST_EQUAL(trim("\u3000hello world\u3000"),        "hello world");
    TEST_EQUAL(trim_left(""),                          "");
    TEST_EQUAL(trim_left("\r\n"),                      "");
    TEST_EQUAL(trim_left("\u3000"),                    "");
    TEST_EQUAL(trim_left("hello world"),               "hello world");
    TEST_EQUAL(trim_left("\r\nhello world\r\n"),       "hello world\r\n");
    TEST_EQUAL(trim_left("\u3000hello world\u3000"),   "hello world\u3000");
    TEST_EQUAL(trim_right(""),                         "");
    TEST_EQUAL(trim_right("\r\n"),                     "");
    TEST_EQUAL(trim_right("\u3000"),                   "");
    TEST_EQUAL(trim_right("hello world"),              "hello world");
    TEST_EQUAL(trim_right("\r\nhello world\r\n"),      "\r\nhello world");
    TEST_EQUAL(trim_right("\u3000hello world\u3000"),  "\u3000hello world");

    TEST_EQUAL(trim("",                              ""),    "");
    TEST_EQUAL(trim("",                              "+-"),  "");
    TEST_EQUAL(trim("hello world",                   "+-"),  "hello world");
    TEST_EQUAL(trim("\thello\tworld\t",              "+-"),  "\thello\tworld\t");
    TEST_EQUAL(trim("+-+-hello+-+-world+-+-",        "+-"),  "+-hello+-+-world+-");
    TEST_EQUAL(trim("Δ∇Δ∇helloΔ∇Δ∇worldΔ∇Δ∇",        "Δ∇"),  "Δ∇helloΔ∇Δ∇worldΔ∇");
    TEST_EQUAL(trim_left("",                         ""),    "");
    TEST_EQUAL(trim_left("",                         "+-"),  "");
    TEST_EQUAL(trim_left("hello world",              "+-"),  "hello world");
    TEST_EQUAL(trim_left("\thello\tworld\t",         "+-"),  "\thello\tworld\t");
    TEST_EQUAL(trim_left("+-+-hello+-+-world+-+-",   "+-"),  "+-hello+-+-world+-+-");
    TEST_EQUAL(trim_left("Δ∇Δ∇helloΔ∇Δ∇worldΔ∇Δ∇",   "Δ∇"),  "Δ∇helloΔ∇Δ∇worldΔ∇Δ∇");
    TEST_EQUAL(trim_right("",                        ""),    "");
    TEST_EQUAL(trim_right("",                        "+-"),  "");
    TEST_EQUAL(trim_right("hello world",             "+-"),  "hello world");
    TEST_EQUAL(trim_right("\thello\tworld\t",        "+-"),  "\thello\tworld\t");
    TEST_EQUAL(trim_right("+-+-hello+-+-world+-+-",  "+-"),  "+-+-hello+-+-world+-");
    TEST_EQUAL(trim_right("Δ∇Δ∇helloΔ∇Δ∇worldΔ∇Δ∇",  "Δ∇"),  "Δ∇Δ∇helloΔ∇Δ∇worldΔ∇");

    TEST_EQUAL(trim("",                              U""),    "");
    TEST_EQUAL(trim("",                              U"+-"),  "");
    TEST_EQUAL(trim("hello world",                   U"+-"),  "hello world");
    TEST_EQUAL(trim("\thello\tworld\t",              U"+-"),  "\thello\tworld\t");
    TEST_EQUAL(trim("+-+-hello+-+-world+-+-",        U"+-"),  "hello+-+-world");
    TEST_EQUAL(trim("Δ∇Δ∇helloΔ∇Δ∇worldΔ∇Δ∇",        U"Δ∇"),  "helloΔ∇Δ∇world");
    TEST_EQUAL(trim_left("",                         U""),    "");
    TEST_EQUAL(trim_left("",                         U"+-"),  "");
    TEST_EQUAL(trim_left("hello world",              U"+-"),  "hello world");
    TEST_EQUAL(trim_left("\thello\tworld\t",         U"+-"),  "\thello\tworld\t");
    TEST_EQUAL(trim_left("+-+-hello+-+-world+-+-",   U"+-"),  "hello+-+-world+-+-");
    TEST_EQUAL(trim_left("Δ∇Δ∇helloΔ∇Δ∇worldΔ∇Δ∇",   U"Δ∇"),  "helloΔ∇Δ∇worldΔ∇Δ∇");
    TEST_EQUAL(trim_right("",                        U""),    "");
    TEST_EQUAL(trim_right("",                        U"+-"),  "");
    TEST_EQUAL(trim_right("hello world",             U"+-"),  "hello world");
    TEST_EQUAL(trim_right("\thello\tworld\t",        U"+-"),  "\thello\tworld\t");
    TEST_EQUAL(trim_right("+-+-hello+-+-world+-+-",  U"+-"),  "+-+-hello+-+-world");
    TEST_EQUAL(trim_right("Δ∇Δ∇helloΔ∇Δ∇worldΔ∇Δ∇",  U"Δ∇"),  "Δ∇Δ∇helloΔ∇Δ∇world");

    TEST_EQUAL(trim("",                              is_pattern_syntax),  "");
    TEST_EQUAL(trim("",                              is_pattern_syntax),  "");
    TEST_EQUAL(trim("hello world",                   is_pattern_syntax),  "hello world");
    TEST_EQUAL(trim("\thello\tworld\t",              is_pattern_syntax),  "\thello\tworld\t");
    TEST_EQUAL(trim("+-+-hello+-+-world+-+-",        is_pattern_syntax),  "hello+-+-world");
    TEST_EQUAL(trim("√∫√∫hello√∫√∫world√∫√∫",        is_pattern_syntax),  "hello√∫√∫world");
    TEST_EQUAL(trim_left("",                         is_pattern_syntax),  "");
    TEST_EQUAL(trim_left("",                         is_pattern_syntax),  "");
    TEST_EQUAL(trim_left("hello world",              is_pattern_syntax),  "hello world");
    TEST_EQUAL(trim_left("\thello\tworld\t",         is_pattern_syntax),  "\thello\tworld\t");
    TEST_EQUAL(trim_left("+-+-hello+-+-world+-+-",   is_pattern_syntax),  "hello+-+-world+-+-");
    TEST_EQUAL(trim_left("√∫√∫hello√∫√∫world√∫√∫",   is_pattern_syntax),  "hello√∫√∫world√∫√∫");
    TEST_EQUAL(trim_right("",                        is_pattern_syntax),  "");
    TEST_EQUAL(trim_right("",                        is_pattern_syntax),  "");
    TEST_EQUAL(trim_right("hello world",             is_pattern_syntax),  "hello world");
    TEST_EQUAL(trim_right("\thello\tworld\t",        is_pattern_syntax),  "\thello\tworld\t");
    TEST_EQUAL(trim_right("+-+-hello+-+-world+-+-",  is_pattern_syntax),  "+-+-hello+-+-world");
    TEST_EQUAL(trim_right("√∫√∫hello√∫√∫world√∫√∫",  is_pattern_syntax),  "√∫√∫hello√∫√∫world");

}

void test_rs_unicode_string_manipulation_word_wrap() {

    std::string original, wrapped;

    TRY(wrapped = word_wrap(original, 40));
    TEST_EQUAL(wrapped, "");
    TRY(wrapped = word_wrap(original, 40, 8, 4));
    TEST_EQUAL(wrapped, "");

    original = "Hello world";;
    TRY(wrapped = word_wrap(original, 40));
    TEST_EQUAL(wrapped, "Hello world\n");
    TRY(wrapped = word_wrap(original, 40, 8, 4));
    TEST_EQUAL(wrapped, "        Hello world\n");
    TRY(wrapped = word_wrap(original, 15, 8, 4));
    TEST_EQUAL(wrapped, "        Hello\n"
                        "    world\n");

    original = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
        "nisi ut aliquip ex ea commodo consequat.";

    TRY(wrapped = word_wrap(original, 40));

    TEST_EQUAL(wrapped,
        //...:....1....:....2....:....3....:....4
        "Lorem ipsum dolor sit amet, consectetur\n"
        "adipiscing elit, sed do eiusmod tempor\n"
        "incididunt ut labore et dolore magna\n"
        "aliqua. Ut enim ad minim veniam, quis\n"
        "nostrud exercitation ullamco laboris\n"
        "nisi ut aliquip ex ea commodo consequat.\n"
    );

    TRY(wrapped = word_wrap(original, 40, 8));

    TEST_EQUAL(wrapped,
        //...:....1....:....2....:....3....:....4
        "        Lorem ipsum dolor sit amet,\n"
        "        consectetur adipiscing elit, sed\n"
        "        do eiusmod tempor incididunt ut\n"
        "        labore et dolore magna aliqua.\n"
        "        Ut enim ad minim veniam, quis\n"
        "        nostrud exercitation ullamco\n"
        "        laboris nisi ut aliquip ex ea\n"
        "        commodo consequat.\n"
    );

    TRY(wrapped = word_wrap(original, 40, 8, 4));

    TEST_EQUAL(wrapped,
        //...:....1....:....2....:....3....:....4
        "        Lorem ipsum dolor sit amet,\n"
        "    consectetur adipiscing elit, sed do\n"
        "    eiusmod tempor incididunt ut labore\n"
        "    et dolore magna aliqua. Ut enim ad\n"
        "    minim veniam, quis nostrud\n"
        "    exercitation ullamco laboris nisi ut\n"
        "    aliquip ex ea commodo consequat.\n"
    );

    TRY(wrapped = word_wrap(original, 20, 10));

    TEST_EQUAL(wrapped,
        //...:....1....:....2....:....3....:....4
        "          Lorem\n"
        "          ipsum\n"
        "          dolor sit\n"
        "          amet,\n"
        "          consectetur\n"
        "          adipiscing\n"
        "          elit, sed\n"
        "          do eiusmod\n"
        "          tempor\n"
        "          incididunt\n"
        "          ut labore\n"
        "          et dolore\n"
        "          magna\n"
        "          aliqua. Ut\n"
        "          enim ad\n"
        "          minim\n"
        "          veniam,\n"
        "          quis\n"
        "          nostrud\n"
        "          exercitation\n"
        "          ullamco\n"
        "          laboris\n"
        "          nisi ut\n"
        "          aliquip ex\n"
        "          ea commodo\n"
        "          consequat.\n"
    );

    original =
        "ΑΒΓΔΕ ΖΗΘΙΚ ΛΜΝΞΟ ΠΡΣΤΥ ΦΧΨΩ\n"
        "αβγδε ζηθικ λμνξο πρστυ φχψω\n";

    TRY(wrapped = word_wrap(original, 20));

    TEST_EQUAL(wrapped,
        //...:....1....:....2
        "ΑΒΓΔΕ ΖΗΘΙΚ ΛΜΝΞΟ\n"
        "ΠΡΣΤΥ ΦΧΨΩ αβγδε\n"
        "ζηθικ λμνξο πρστυ\n"
        "φχψω\n"
    );

    TRY(wrapped = word_wrap(original, 20, 8, 4));

    TEST_EQUAL(wrapped,
        //...:....1....:....2
        "        ΑΒΓΔΕ ΖΗΘΙΚ\n"
        "    ΛΜΝΞΟ ΠΡΣΤΥ ΦΧΨΩ\n"
        "    αβγδε ζηθικ\n"
        "    λμνξο πρστυ φχψω\n"
    );

}
