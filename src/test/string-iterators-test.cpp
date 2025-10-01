#include "rs-unicode/string.hpp"
#include "rs-unicode/encoding.hpp"
#include "rs-core/unit-test.hpp"
#include <algorithm>
#include <format>
#include <iterator>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace RS::Unicode;
namespace rs = std::ranges;

void test_rs_unicode_iterators_reify() {

    std::set<const char*> pointers {"alpha", "bravo", "charlie"};
    std::set<std::string_view> views {"delta", "echo", "foxtrot"};
    std::set<std::string> strings {"golf", "hotel", "india"};
    std::vector<std::string> vec;

    TRY(vec = reify(pointers));  TEST_EQUAL(std::format("{}", vec), R"(["alpha", "bravo", "charlie"])");
    TRY(vec = reify(views));     TEST_EQUAL(std::format("{}", vec), R"(["delta", "echo", "foxtrot"])");
    TRY(vec = reify(strings));   TEST_EQUAL(std::format("{}", vec), R"(["golf", "hotel", "india"])");

}

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

    auto parts = split_words("");                                           TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_words(" "));                                          TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_words("\r\n"));                                       TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_words("Hello"));                                      TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["Hello"])");
    TRY(parts = split_words("Hello world"));                                TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_words("\r\nHello\r\nworld\r\n"));                     TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_words("Hello world; goodbye"));                       TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world;", "goodbye"])");
    TRY(parts = split_words("Greek αβγδε"));                                TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Greek", "αβγδε"])");
    TRY(parts = split_words("😀👍👩"));                                    TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["😀👍👩"])");
    TRY(parts = split_words("😀 👍 👩"));                                  TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["😀", "👍", "👩"])");
    TRY(parts = split_words("😀👍🏽👩🏽"));                                    TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["😀👍🏽👩🏽"])");
    TRY(parts = split_words("😀 👍🏽 👩🏽"));                                  TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["😀", "👍🏽", "👩🏽"])");
    TRY(parts = split_words("🇳🇿🇺🇸🇩🇪🇦🇺"));                                  TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["🇳🇿🇺🇸🇩🇪🇦🇺"])");
    TRY(parts = split_words("🇳🇿 🇺🇸 🇩🇪 🇦🇺"));                               TEST_EQUAL(rs::distance(parts), 4);  TEST_EQUAL(std::format("{}", parts), R"(["🇳🇿", "🇺🇸", "🇩🇪", "🇦🇺"])");
    TRY(parts = split_lines(""));                                           TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_lines("\n"));                                         TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"([""])");
    TRY(parts = split_lines("\n\n"));                                       TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["", ""])");
    TRY(parts = split_lines("Hello world"));                                TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["Hello world"])");
    TRY(parts = split_lines("Hello\nworld\n"));                             TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_lines("Hello\r\nworld\r\n"));                         TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_lines("\nHello\nworld\n"));                           TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["", "Hello", "world"])");
    TRY(parts = split_lines("\r\nHello\r\nworld\r\n"));                     TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["", "Hello", "world"])");
    TRY(parts = split_lines("\n\nHello\n\nworld\n\n"));                     TEST_EQUAL(rs::distance(parts), 6);  TEST_EQUAL(std::format("{}", parts), R"(["", "", "Hello", "", "world", ""])");
    TRY(parts = split_any("", ""));                                         TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_any("Hello world", ""));                              TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["Hello world"])");
    TRY(parts = split_any("", "⋀⋁"));                                       TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_any("Hello", "⋀⋁"));                                  TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["Hello"])");
    TRY(parts = split_any("Hello⋀world", "⋀⋁"));                            TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_any("Hello⋀world⋁goodbye", "⋀⋁"));                    TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world", "goodbye"])");
    TRY(parts = split_any("⋁Hello⋀world⋁goodbye⋀", "⋀⋁"));                  TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world", "goodbye"])");
    TRY(parts = split_any("⋀⋁⋀⋁Hello⋀⋁⋀⋁world⋀⋁⋀⋁", "⋀⋁"));               TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_at("", ""));                                          TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_at("Hello world", ""));                               TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["Hello world"])");
    TRY(parts = split_at("", "+-"));                                        TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_at("+-", "+-"));                                      TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["", ""])");
    TRY(parts = split_at("+-+-", "+-"));                                    TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["", "", ""])");
    TRY(parts = split_at("Hello world", "+-"));                             TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["Hello world"])");
    TRY(parts = split_at("Hello+-world", "+-"));                            TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_at("+-Hello+-world+-", "+-"));                        TEST_EQUAL(rs::distance(parts), 4);  TEST_EQUAL(std::format("{}", parts), R"(["", "Hello", "world", ""])");
    TRY(parts = split_at("+-+-Hello+-+-world+-+-", "+-"));                  TEST_EQUAL(rs::distance(parts), 7);  TEST_EQUAL(std::format("{}", parts), R"(["", "", "Hello", "", "world", "", ""])");
    TRY(parts = split_at("", "🇦🇺"));                                        TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_at("🇦🇺", "🇦🇺"));                                      TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["", ""])");
    TRY(parts = split_at("🇦🇺🇦🇺", "🇦🇺"));                                    TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["", "", ""])");
    TRY(parts = split_at("Hello world", "🇦🇺"));                             TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["Hello world"])");
    TRY(parts = split_at("Hello🇦🇺world", "🇦🇺"));                            TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_at("🇦🇺Hello🇦🇺world🇦🇺", "🇦🇺"));                       TEST_EQUAL(rs::distance(parts), 4);  TEST_EQUAL(std::format("{}", parts), R"(["", "Hello", "world", ""])");
    TRY(parts = split_at("🇦🇺🇦🇺Hello🇦🇺🇦🇺world🇦🇺🇦🇺", "🇦🇺"));                TEST_EQUAL(rs::distance(parts), 7);  TEST_EQUAL(std::format("{}", parts), R"(["", "", "Hello", "", "world", "", ""])");
    TRY(parts = split_where("", is_pattern_syntax));                        TEST_EQUAL(rs::distance(parts), 0);  TEST_EQUAL(std::format("{}", parts), R"([])");
    TRY(parts = split_where("Hello", is_pattern_syntax));                   TEST_EQUAL(rs::distance(parts), 1);  TEST_EQUAL(std::format("{}", parts), R"(["Hello"])");
    TRY(parts = split_where("Hello*world", is_pattern_syntax));             TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_where("Hello*world/goodbye", is_pattern_syntax));     TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world", "goodbye"])");
    TRY(parts = split_where("/Hello*world/goodbye*", is_pattern_syntax));   TEST_EQUAL(rs::distance(parts), 3);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world", "goodbye"])");
    TRY(parts = split_where("*/*/Hello*/*/world*/*/", is_pattern_syntax));  TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");
    TRY(parts = split_where("∇∇Hello∇∇world∇∇", is_pattern_syntax));        TEST_EQUAL(rs::distance(parts), 2);  TEST_EQUAL(std::format("{}", parts), R"(["Hello", "world"])");

}
