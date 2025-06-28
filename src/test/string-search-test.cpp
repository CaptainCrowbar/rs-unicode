#include "rs-unicode/string.hpp"
#include "rs-unicode/character.hpp"
#include "rs-core/unit-test.hpp"
#include <cstddef>
#include <string>
#include <string_view>

using namespace RS::Unicode;

void test_rs_unicode_string_search_find_first_of() {

    std::string s;
    std::string_view v;

    auto pos = [&] {
        return v.data() == nullptr ? npos : static_cast<std::size_t>(v.data() - s.data());
    };

    s = "";             TRY(v = find_first_of(s, U'a'));               TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_of(s, U'l'));               TEST_EQUAL(v, "l");  TEST_EQUAL(pos(), 2u);
    s = "Hello world";  TRY(v = find_first_of(s, U'a'));               TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "αβγδε αβγδε";  TRY(v = find_first_of(s, U'γ'));               TEST_EQUAL(v, "γ");  TEST_EQUAL(pos(), 4u);

    s = "";             TRY(v = find_first_of(s, U"abc"));             TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_of(s, U"rld"));             TEST_EQUAL(v, "l");  TEST_EQUAL(pos(), 2u);
    s = "Hello world";  TRY(v = find_first_of(s, U"abc"));             TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_of(s, U""));                TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "αβγδε αβγδε";  TRY(v = find_first_of(s, U"βγδ"));             TEST_EQUAL(v, "β");  TEST_EQUAL(pos(), 2u);

    s = "";             TRY(v = find_first_of(s, "abc"));              TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_of(s, "rld"));              TEST_EQUAL(v, "l");  TEST_EQUAL(pos(), 2u);
    s = "Hello world";  TRY(v = find_first_of(s, "abc"));              TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_of(s, ""));                 TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "αβγδε αβγδε";  TRY(v = find_first_of(s, "βγδ"));              TEST_EQUAL(v, "β");  TEST_EQUAL(pos(), 2u);

    s = "";             TRY(v = find_first_of(s, is_white_space));     TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_of(s, is_white_space));     TEST_EQUAL(v, " ");  TEST_EQUAL(pos(), 5u);
    s = "Hello world";  TRY(v = find_first_of(s, is_pattern_syntax));  TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "αβγδε αβγδε";  TRY(v = find_first_of(s, is_white_space));     TEST_EQUAL(v, " ");  TEST_EQUAL(pos(), 10u);

}

void test_rs_unicode_string_search_find_first_not_of() {

    std::string s;
    std::string_view v;

    auto pos = [&] {
        return v.data() == nullptr ? npos : static_cast<std::size_t>(v.data() - s.data());
    };

    s = "";             TRY(v = find_first_not_of(s, U'a'));               TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_not_of(s, U'H'));               TEST_EQUAL(v, "e");  TEST_EQUAL(pos(), 1u);
    s = "αβγδε αβγδε";  TRY(v = find_first_not_of(s, U'α'));               TEST_EQUAL(v, "β");  TEST_EQUAL(pos(), 2u);

    s = "";             TRY(v = find_first_not_of(s, U"abc"));             TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_not_of(s, U"HIJ"));             TEST_EQUAL(v, "e");  TEST_EQUAL(pos(), 1u);
    s = "Hello world";  TRY(v = find_first_not_of(s, U""));                TEST_EQUAL(v, "H");  TEST_EQUAL(pos(), 0u);
    s = "αβγδε αβγδε";  TRY(v = find_first_not_of(s, U"αβγ"));             TEST_EQUAL(v, "δ");  TEST_EQUAL(pos(), 6u);
    s = "αβγδε αβγδε";  TRY(v = find_first_not_of(s, U""));                TEST_EQUAL(v, "α");  TEST_EQUAL(pos(), 0u);

    s = "";             TRY(v = find_first_not_of(s, "abc"));              TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_not_of(s, "HIJ"));              TEST_EQUAL(v, "e");  TEST_EQUAL(pos(), 1u);
    s = "Hello world";  TRY(v = find_first_not_of(s, ""));                 TEST_EQUAL(v, "H");  TEST_EQUAL(pos(), 0u);
    s = "αβγδε αβγδε";  TRY(v = find_first_not_of(s, "αβγ"));              TEST_EQUAL(v, "δ");  TEST_EQUAL(pos(), 6u);
    s = "αβγδε αβγδε";  TRY(v = find_first_not_of(s, ""));                 TEST_EQUAL(v, "α");  TEST_EQUAL(pos(), 0u);

    s = "";             TRY(v = find_first_not_of(s, is_alphanumeric));     TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_first_not_of(s, is_alphanumeric));     TEST_EQUAL(v, " ");  TEST_EQUAL(pos(), 5u);
    s = "αβγδε αβγδε";  TRY(v = find_first_not_of(s, is_alphanumeric));     TEST_EQUAL(v, " ");  TEST_EQUAL(pos(), 10u);

}

void test_rs_unicode_string_search_find_last_of() {

    std::string s;
    std::string_view v;

    auto pos = [&] {
        return v.data() == nullptr ? npos : static_cast<std::size_t>(v.data() - s.data());
    };

    s = "";             TRY(v = find_last_of(s, U'a'));               TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_of(s, U'l'));               TEST_EQUAL(v, "l");  TEST_EQUAL(pos(), 9u);
    s = "Hello world";  TRY(v = find_last_of(s, U'a'));               TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "αβγδε αβγδε";  TRY(v = find_last_of(s, U'γ'));               TEST_EQUAL(v, "γ");  TEST_EQUAL(pos(), 15u);

    s = "";             TRY(v = find_last_of(s, U"abc"));             TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_of(s, U"rld"));             TEST_EQUAL(v, "d");  TEST_EQUAL(pos(), 10u);
    s = "Hello world";  TRY(v = find_last_of(s, U"abc"));             TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_of(s, U""));                TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "αβγδε αβγδε";  TRY(v = find_last_of(s, U"βγδ"));             TEST_EQUAL(v, "δ");  TEST_EQUAL(pos(), 17u);

    s = "";             TRY(v = find_last_of(s, "abc"));              TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_of(s, "rld"));              TEST_EQUAL(v, "d");  TEST_EQUAL(pos(), 10u);
    s = "Hello world";  TRY(v = find_last_of(s, "abc"));              TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_of(s, ""));                 TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "αβγδε αβγδε";  TRY(v = find_last_of(s, "βγδ"));              TEST_EQUAL(v, "δ");  TEST_EQUAL(pos(), 17u);

    s = "";             TRY(v = find_last_of(s, is_white_space));     TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_of(s, is_white_space));     TEST_EQUAL(v, " ");  TEST_EQUAL(pos(), 5u);
    s = "Hello world";  TRY(v = find_last_of(s, is_pattern_syntax));  TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "αβγδε αβγδε";  TRY(v = find_last_of(s, is_white_space));     TEST_EQUAL(v, " ");  TEST_EQUAL(pos(), 10u);

}

void test_rs_unicode_string_search_find_last_not_of() {

    std::string s;
    std::string_view v;

    auto pos = [&] {
        return v.data() == nullptr ? npos : static_cast<std::size_t>(v.data() - s.data());
    };

    s = "";             TRY(v = find_last_not_of(s, U'a'));               TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_not_of(s, U'd'));               TEST_EQUAL(v, "l");  TEST_EQUAL(pos(), 9u);
    s = "αβγδε αβγδε";  TRY(v = find_last_not_of(s, U'ε'));               TEST_EQUAL(v, "δ");  TEST_EQUAL(pos(), 17u);

    s = "";             TRY(v = find_last_not_of(s, U"abc"));             TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_not_of(s, U"def"));             TEST_EQUAL(v, "l");  TEST_EQUAL(pos(), 9u);
    s = "Hello world";  TRY(v = find_last_not_of(s, U""));                TEST_EQUAL(v, "d");  TEST_EQUAL(pos(), 10u);
    s = "αβγδε αβγδε";  TRY(v = find_last_not_of(s, U"γδε"));             TEST_EQUAL(v, "β");  TEST_EQUAL(pos(), 13u);
    s = "αβγδε αβγδε";  TRY(v = find_last_not_of(s, U""));                TEST_EQUAL(v, "ε");  TEST_EQUAL(pos(), 19u);

    s = "";             TRY(v = find_last_not_of(s, "abc"));              TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_not_of(s, "def"));              TEST_EQUAL(v, "l");  TEST_EQUAL(pos(), 9u);
    s = "Hello world";  TRY(v = find_last_not_of(s, ""));                 TEST_EQUAL(v, "d");  TEST_EQUAL(pos(), 10u);
    s = "αβγδε αβγδε";  TRY(v = find_last_not_of(s, "γδε"));              TEST_EQUAL(v, "β");  TEST_EQUAL(pos(), 13u);
    s = "αβγδε αβγδε";  TRY(v = find_last_not_of(s, ""));                 TEST_EQUAL(v, "ε");  TEST_EQUAL(pos(), 19u);

    s = "";             TRY(v = find_last_not_of(s, is_alphanumeric));     TEST_EQUAL(v, "");   TEST_EQUAL(pos(), npos);
    s = "Hello world";  TRY(v = find_last_not_of(s, is_alphanumeric));     TEST_EQUAL(v, " ");  TEST_EQUAL(pos(), 5u);
    s = "αβγδε αβγδε";  TRY(v = find_last_not_of(s, is_alphanumeric));     TEST_EQUAL(v, " ");  TEST_EQUAL(pos(), 10u);

}
