#include "rs-unicode/string.hpp"
#include "rs-unicode/encoding.hpp"
#include "test/unit-test.hpp"
#include <algorithm>
#include <format>
#include <iterator>
#include <ranges>
#include <string>

using namespace RS::Unicode;
namespace rs = std::ranges;

void test_rs_unicode_iterators_grapheme_view() {

    std::string str;
    GraphemeIterator b, e, i;

    auto gv = grapheme_view(str);
    b = rs::begin(gv);
    e = rs::end(gv);
    TEST(b == e);

    str =
        "Hello\n"
        "αβγδε\n"
        "áéíóú\n"                                // composed
        "a\u0301e\u0301i\u0301o\u0301u\u0301\n"  // decomposed
        "😀👍👩\n"                              // simple emoji
        "😀👍🏽👩🏽\n"                              // modified emoji
        "🇳🇿🇺🇸🇩🇪🇦🇺\n";                           // flags

    gv = grapheme_view(str);
    b = rs::begin(gv);
    e = rs::end(gv);

    TRY(i = b);  REQUIRE(i != e);  TEST_EQUAL(*i, "H");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "e");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "l");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "l");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "o");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "α");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "β");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "γ");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "δ");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "ε");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "á");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "é");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "í");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "ó");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "ú");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "a\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "e\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "i\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "o\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "u\u0301");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "😀");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "👍");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "👩");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "😀");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "👍🏽");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "👩🏽");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "🇳🇿");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "🇺🇸");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "🇩🇪");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "🇦🇺");
    TRY(++i);    REQUIRE(i != e);  TEST_EQUAL(*i, "\n");

    TRY(++i);
    TEST(i == e);

}

void test_rs_unicode_iterators_split_view() {

    auto view = split_view("");                                           TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view(" "));                                          TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view("\r\n"));                                       TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view("Hello"));                                      TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["Hello"])");
    TRY(view = split_view("Hello world"));                                TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");
    TRY(view = split_view("\r\nHello\r\nworld\r\n"));                     TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");
    TRY(view = split_view("Hello world; goodbye"));                       TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world;", "goodbye"])");
    TRY(view = split_view("Greek αβγδε"));                                TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Greek", "αβγδε"])");
    TRY(view = split_view("😀👍👩"));                                    TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["😀👍👩"])");
    TRY(view = split_view("😀 👍 👩"));                                  TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["😀", "👍", "👩"])");
    TRY(view = split_view("😀👍🏽👩🏽"));                                    TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["😀👍🏽👩🏽"])");
    TRY(view = split_view("😀 👍🏽 👩🏽"));                                  TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["😀", "👍🏽", "👩🏽"])");
    TRY(view = split_view("🇳🇿🇺🇸🇩🇪🇦🇺"));                                  TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["🇳🇿🇺🇸🇩🇪🇦🇺"])");
    TRY(view = split_view("🇳🇿 🇺🇸 🇩🇪 🇦🇺"));                               TEST_EQUAL(rs::distance(view), 4);  TEST_EQUAL(std::format("{}", view), R"(["🇳🇿", "🇺🇸", "🇩🇪", "🇦🇺"])");
    TRY(view = split_view("", ""));                                       TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view("Hello world", ""));                            TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["Hello world"])");
    TRY(view = split_view("", "+-"));                                     TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view("+-", "+-"));                                   TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["", ""])");
    TRY(view = split_view("+-+-", "+-"));                                 TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["", "", ""])");
    TRY(view = split_view("Hello world", "+-"));                          TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["Hello world"])");
    TRY(view = split_view("Hello+-world", "+-"));                         TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");
    TRY(view = split_view("+-Hello+-world+-", "+-"));                     TEST_EQUAL(rs::distance(view), 4);  TEST_EQUAL(std::format("{}", view), R"(["", "Hello", "world", ""])");
    TRY(view = split_view("+-+-Hello+-+-world+-+-", "+-"));               TEST_EQUAL(rs::distance(view), 7);  TEST_EQUAL(std::format("{}", view), R"(["", "", "Hello", "", "world", "", ""])");
    TRY(view = split_view("", "🇦🇺"));                                     TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view("🇦🇺", "🇦🇺"));                                   TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["", ""])");
    TRY(view = split_view("🇦🇺🇦🇺", "🇦🇺"));                                 TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["", "", ""])");
    TRY(view = split_view("Hello world", "🇦🇺"));                          TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["Hello world"])");
    TRY(view = split_view("Hello🇦🇺world", "🇦🇺"));                         TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");
    TRY(view = split_view("🇦🇺Hello🇦🇺world🇦🇺", "🇦🇺"));                    TEST_EQUAL(rs::distance(view), 4);  TEST_EQUAL(std::format("{}", view), R"(["", "Hello", "world", ""])");
    TRY(view = split_view("🇦🇺🇦🇺Hello🇦🇺🇦🇺world🇦🇺🇦🇺", "🇦🇺"));             TEST_EQUAL(rs::distance(view), 7);  TEST_EQUAL(std::format("{}", view), R"(["", "", "Hello", "", "world", "", ""])");
    TRY(view = split_view("", U""));                                      TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view("Hello world", U""));                           TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["Hello world"])");
    TRY(view = split_view("", U"⋀⋁"));                                    TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view("Hello", U"⋀⋁"));                               TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["Hello"])");
    TRY(view = split_view("Hello⋀world", U"⋀⋁"));                         TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");
    TRY(view = split_view("Hello⋀world⋁goodbye", U"⋀⋁"));                 TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world", "goodbye"])");
    TRY(view = split_view("⋁Hello⋀world⋁goodbye⋀", U"⋀⋁"));               TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world", "goodbye"])");
    TRY(view = split_view("⋀⋁⋀⋁Hello⋀⋁⋀⋁world⋀⋁⋀⋁", U"⋀⋁"));            TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");
    TRY(view = split_view("", is_pattern_syntax));                        TEST_EQUAL(rs::distance(view), 0);  TEST_EQUAL(std::format("{}", view), R"([])");
    TRY(view = split_view("Hello", is_pattern_syntax));                   TEST_EQUAL(rs::distance(view), 1);  TEST_EQUAL(std::format("{}", view), R"(["Hello"])");
    TRY(view = split_view("Hello*world", is_pattern_syntax));             TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");
    TRY(view = split_view("Hello*world/goodbye", is_pattern_syntax));     TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world", "goodbye"])");
    TRY(view = split_view("/Hello*world/goodbye*", is_pattern_syntax));   TEST_EQUAL(rs::distance(view), 3);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world", "goodbye"])");
    TRY(view = split_view("*/*/Hello*/*/world*/*/", is_pattern_syntax));  TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");
    TRY(view = split_view("∇∇Hello∇∇world∇∇", is_pattern_syntax));        TEST_EQUAL(rs::distance(view), 2);  TEST_EQUAL(std::format("{}", view), R"(["Hello", "world"])");

}

void test_rs_unicode_iterators_split_vector() {

    auto vec = split_vector("");                                           TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector(" "));                                          TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector("\r\n"));                                       TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector("Hello"));                                      TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello"])");
    TRY(vec = split_vector("Hello world"));                                TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");
    TRY(vec = split_vector("\r\nHello\r\nworld\r\n"));                     TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");
    TRY(vec = split_vector("Hello world; goodbye"));                       TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world;", "goodbye"])");
    TRY(vec = split_vector("Greek αβγδε"));                                TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Greek", "αβγδε"])");
    TRY(vec = split_vector("😀👍👩"));                                    TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["😀👍👩"])");
    TRY(vec = split_vector("😀 👍 👩"));                                  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["😀", "👍", "👩"])");
    TRY(vec = split_vector("😀👍🏽👩🏽"));                                    TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["😀👍🏽👩🏽"])");
    TRY(vec = split_vector("😀 👍🏽 👩🏽"));                                  TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["😀", "👍🏽", "👩🏽"])");
    TRY(vec = split_vector("🇳🇿🇺🇸🇩🇪🇦🇺"));                                  TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["🇳🇿🇺🇸🇩🇪🇦🇺"])");
    TRY(vec = split_vector("🇳🇿 🇺🇸 🇩🇪 🇦🇺"));                               TEST_EQUAL(vec.size(), 4u);  TEST_EQUAL(std::format("{}", vec), R"(["🇳🇿", "🇺🇸", "🇩🇪", "🇦🇺"])");
    TRY(vec = split_vector("", ""));                                       TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector("Hello world", ""));                            TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello world"])");
    TRY(vec = split_vector("", "+-"));                                     TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector("+-", "+-"));                                   TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["", ""])");
    TRY(vec = split_vector("+-+-", "+-"));                                 TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["", "", ""])");
    TRY(vec = split_vector("Hello world", "+-"));                          TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello world"])");
    TRY(vec = split_vector("Hello+-world", "+-"));                         TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");
    TRY(vec = split_vector("+-Hello+-world+-", "+-"));                     TEST_EQUAL(vec.size(), 4u);  TEST_EQUAL(std::format("{}", vec), R"(["", "Hello", "world", ""])");
    TRY(vec = split_vector("+-+-Hello+-+-world+-+-", "+-"));               TEST_EQUAL(vec.size(), 7u);  TEST_EQUAL(std::format("{}", vec), R"(["", "", "Hello", "", "world", "", ""])");
    TRY(vec = split_vector("", "🇦🇺"));                                     TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector("🇦🇺", "🇦🇺"));                                   TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["", ""])");
    TRY(vec = split_vector("🇦🇺🇦🇺", "🇦🇺"));                                TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["", "", ""])");
    TRY(vec = split_vector("Hello world", "🇦🇺"));                          TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello world"])");
    TRY(vec = split_vector("Hello🇦🇺world", "🇦🇺"));                         TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");
    TRY(vec = split_vector("🇦🇺Hello🇦🇺world🇦🇺", "🇦🇺"));                    TEST_EQUAL(vec.size(), 4u);  TEST_EQUAL(std::format("{}", vec), R"(["", "Hello", "world", ""])");
    TRY(vec = split_vector("🇦🇺🇦🇺Hello🇦🇺🇦🇺world🇦🇺🇦🇺", "🇦🇺"));             TEST_EQUAL(vec.size(), 7u);  TEST_EQUAL(std::format("{}", vec), R"(["", "", "Hello", "", "world", "", ""])");
    TRY(vec = split_vector("", U""));                                      TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector("Hello world", U""));                           TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello world"])");
    TRY(vec = split_vector("", U"⋀⋁"));                                    TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector("Hello", U"⋀⋁"));                               TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello"])");
    TRY(vec = split_vector("Hello⋀world", U"⋀⋁"));                         TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");
    TRY(vec = split_vector("Hello⋀world⋁goodbye", U"⋀⋁"));                 TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world", "goodbye"])");
    TRY(vec = split_vector("⋁Hello⋀world⋁goodbye⋀", U"⋀⋁"));              TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world", "goodbye"])");
    TRY(vec = split_vector("⋀⋁⋀⋁Hello⋀⋁⋀⋁world⋀⋁⋀⋁", U"⋀⋁"));            TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");
    TRY(vec = split_vector("", is_pattern_syntax));                        TEST_EQUAL(vec.size(), 0u);  TEST_EQUAL(std::format("{}", vec), R"([])");
    TRY(vec = split_vector("Hello", is_pattern_syntax));                   TEST_EQUAL(vec.size(), 1u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello"])");
    TRY(vec = split_vector("Hello*world", is_pattern_syntax));             TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");
    TRY(vec = split_vector("Hello*world/goodbye", is_pattern_syntax));     TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world", "goodbye"])");
    TRY(vec = split_vector("/Hello*world/goodbye*", is_pattern_syntax));   TEST_EQUAL(vec.size(), 3u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world", "goodbye"])");
    TRY(vec = split_vector("*/*/Hello*/*/world*/*/", is_pattern_syntax));  TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");
    TRY(vec = split_vector("∇∇Hello∇∇world∇∇", is_pattern_syntax));        TEST_EQUAL(vec.size(), 2u);  TEST_EQUAL(std::format("{}", vec), R"(["Hello", "world"])");

}
