#include "rs-unicode/regex.hpp"
#include "rs-core/unit-test.hpp"
#include <algorithm>
#include <format>
#include <iterator>
#include <ranges>
#include <string>

using namespace RS::Unicode;
using namespace RS::Unicode::Literals;
namespace rs = std::ranges;

void test_rs_unicode_regex_pcre_version() {

    auto v = Regex::pcre_version();
    TEST(v.first >= 10);

}

void test_rs_unicode_regex_match() {

    Regex re;
    Regex::match m;

    TEST_EQUAL(re.groups(), 0u);
    TRY(m = re("Hello world"));
    TEST(! m);
    TEST_EQUAL(m.pos(), RS::npos);
    TEST_EQUAL(m.len(), 0u);
    TEST_EQUAL(m.str(), "");
    TEST_EQUAL(m[0], "");

    TRY(re = "[a-z]+"_re);
    TEST_EQUAL(re.groups(), 1u);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 1u);
    TEST_EQUAL(m.len(), 4u);
    TEST_EQUAL(m.str(), "ello");
    TEST_EQUAL(m[0], "ello");

    TRY(re = Regex("[a-z]+", Regex::icase));
    TEST_EQUAL(re.groups(), 1u);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.len(), 5u);
    TEST_EQUAL(m.str(), "Hello");
    TEST_EQUAL(m[0], "Hello");

    TRY(re = "(?i)[a-z]+"_re);
    TEST_EQUAL(re.groups(), 1u);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.len(), 5u);
    TEST_EQUAL(m.str(), "Hello");
    TEST_EQUAL(m[0], "Hello");

    TRY(re = "([a-z]+)\\s+([a-z]+)"_re);
    TEST_EQUAL(re.groups(), 3u);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 1u);   TEST_EQUAL(m.len(), 10u);  TEST_EQUAL(m.str(), "ello world");  TEST_EQUAL(m[0], "ello world");
    TEST_EQUAL(m.pos(1), 1u);  TEST_EQUAL(m.len(1), 4u);  TEST_EQUAL(m.str(1), "ello");       TEST_EQUAL(m[1], "ello");
    TEST_EQUAL(m.pos(2), 6u);  TEST_EQUAL(m.len(2), 5u);  TEST_EQUAL(m.str(2), "world");      TEST_EQUAL(m[2], "world");

}

void test_rs_unicode_regex_flags() {

    Regex re;
    Regex::match m;

    TRY(re = "[a-z]+"_re);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 1u);
    TEST_EQUAL(m.str(), "ello");
    TRY(m = re("Hello world", Regex::anchor));
    TEST(! m);
    TRY(m = re("Hello world", Regex::anchor, 1));
    TEST(m);
    TEST_EQUAL(m.pos(), 1u);
    TEST_EQUAL(m.str(), "ello");

    TRY(re = ".+"_re);
    TRY(m = re("Hello world\nGoodbye\n"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.str(), "Hello world");
    TRY(re = Regex(".+", Regex::dotall));
    TRY(m = re("Hello world\nGoodbye\n"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.str(), "Hello world\nGoodbye\n");

    TRY(re = "[a-z]+"_re);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 1u);
    TEST_EQUAL(m.str(), "ello");
    TRY(m = re("Hello world", Regex::full));
    TEST(! m);
    TRY(m = re("Hello world", Regex::full, 6));
    TEST(m);
    TEST_EQUAL(m.pos(), 6u);
    TEST_EQUAL(m.str(), "world");
    TRY(re = Regex("[\\s\\w]+", Regex::full));
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.str(), "Hello world");

    TRY(re = Regex(R"(
        ^             # Begin
        [A-Z] [a-z]+  # First word
        \s +          # Space
        [a-z]+        # Second word
        $             # End
        )", Regex::extended));
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.str(), "Hello world");

    TRY(re = R"((?x)
        ^             # Begin
        [A-Z] [a-z]+  # First word
        \s +          # Space
        [a-z]+        # Second word
        $             # End
        )"_re);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.str(), "Hello world");

    TRY(re = "([A-Z][a-z]+) ([a-z]+)"_re);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.str(), "Hello world");
    TEST(m.matched(1));
    TEST_EQUAL(m.str(1), "Hello");
    TEST(m.matched(2));
    TEST_EQUAL(m.str(2), "world");
    TRY(re = Regex("([A-Z][a-z]+) ([a-z]+)", Regex::nocapture));
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.str(), "Hello world");
    TEST(! m.matched(1));
    TEST(! m.matched(2));

    TRY(re = "(?=H)"_re);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.str(), "");
    TRY(m = re("Hello world", Regex::notempty));
    TEST(! m);
    TRY(re = Regex("[a-z]+(?= )", Regex::notempty));
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 1u);
    TEST_EQUAL(m.str(), "ello");

    TRY(re = "[a-z]{10}"_re);
    TRY(m = re("Hello world"));
    TEST(! m);
    TRY(m = re("Hello world", Regex::partial));
    TEST(m);
    TEST(m.partial());
    TEST_EQUAL(m.pos(), 6u);
    TEST_EQUAL(m.str(), "world");

}

void test_rs_unicode_regex_grep() {

    Regex re;

    auto matches = re.grep("Hello world");
    TEST(rs::empty(matches));

    TRY(re = "[a-z]+"_re);
    TRY(matches = re.grep("Hello world"));
    TEST_EQUAL(rs::distance(matches), 2);
    TEST_EQUAL(std::format("{}", matches), "[ello, world]");

    TRY(re = "(?i)[a-z]{2}"_re);
    TRY(matches = re.grep("Hello world"));
    TEST_EQUAL(rs::distance(matches), 4);
    TEST_EQUAL(std::format("{}", matches), "[He, ll, wo, rl]");

}

void test_rs_unicode_regex_split() {

    Regex re;

    auto words = re.split("Hello world\r\nGoodbye");
    TEST_EQUAL(rs::distance(words), 1);
    TEST_EQUAL(std::format("{}", words), R"(["Hello world\r\nGoodbye"])");

    TRY(re = "\\s+"_re);
    TRY(words = re.split("Hello world\r\nGoodbye"));
    TEST_EQUAL(rs::distance(words), 3);
    TEST_EQUAL(std::format("{}", words), R"(["Hello", "world", "Goodbye"])");
    TRY(words = re.split("Hello world\r\nGoodbye\r\n"));
    TEST_EQUAL(rs::distance(words), 4);
    TEST_EQUAL(std::format("{}", words), R"(["Hello", "world", "Goodbye", ""])");

}

void test_rs_unicode_regex_format() {

    Regex re;
    std::string in, out, rep;

    TRY(out = re.format(in, rep));
    TEST_EQUAL(out, "");

    in = "Hello world, goodbye";
    TRY(out = re.format(in, rep));
    TEST_EQUAL(out, in);

    TRY(re = "\\w+"_re);
    rep = "*";
    TRY(out = re.format(in, rep));
    TEST_EQUAL(out, "* world, goodbye");
    TRY(out = re.format(in, rep, Regex::global));
    TEST_EQUAL(out, "* *, *");

    TRY(re = "[a-z]+"_re);
    rep = "*";
    TRY(out = re.format(in, rep));
    TEST_EQUAL(out, "H* world, goodbye");
    TRY(out = re.format(in, rep, Regex::global));
    TEST_EQUAL(out, "H* *, *");

    TRY(re = "([A-Z]+)|([a-z]+)"_re);
    rep = "\\L$1\\U$2\\E";
    TRY(out = re.format(in, rep));
    TEST_EQUAL(out, "hello world, goodbye");
    TRY(out = re.format(in, rep, Regex::global));
    TEST_EQUAL(out, "hELLO WORLD, GOODBYE");

}

void test_rs_unicode_regex_transform() {

    Regex re;
    Regex::transform tr;
    std::string in, out, rep;

    TRY(out = tr(in));
    TEST_EQUAL(out, "");

    in = "Hello world, goodbye";
    TRY(out = tr(in));
    TEST_EQUAL(out, in);

    TRY(re = "\\w+"_re);
    TRY(tr = Regex::transform(re, "*"));
    TRY(out = tr(in));
    TEST_EQUAL(out, "* world, goodbye");
    TRY(out = tr(in, Regex::global));
    TEST_EQUAL(out, "* *, *");

    TRY(re = "\\w+"_re);
    TRY(tr = Regex::transform(re, "*", Regex::global));
    TRY(out = tr(in));
    TEST_EQUAL(out, "* *, *");

    TRY(tr = Regex::transform("[a-z]+", "*"));
    TRY(out = tr(in));
    TEST_EQUAL(out, "H* world, goodbye");
    TRY(out = tr(in, Regex::global));
    TEST_EQUAL(out, "H* *, *");

    TRY(tr = Regex::transform("([A-Z]+)|([a-z]+)", "\\L$1\\U$2\\E"));
    TRY(out = tr(in));
    TEST_EQUAL(out, "hello world, goodbye");
    TRY(out = tr(in, Regex::global));
    TEST_EQUAL(out, "hELLO WORLD, GOODBYE");

    TRY(tr = Regex::transform("([A-Z]+)|([a-z]+)", "\\L$1\\U$2\\E", Regex::global));
    TRY(out = tr(in, Regex::global));
    TEST_EQUAL(out, "hELLO WORLD, GOODBYE");

}

void test_rs_unicode_regex_dfa() {

    Regex re;
    Regex::match m;

    TRY(re = Regex("[a-z]+", Regex::dfa));
    TEST_EQUAL(re.groups(), 1u);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 1u);
    TEST_EQUAL(m.len(), 4u);
    TEST_EQUAL(m.str(), "ello");
    TEST_EQUAL(m[0], "ello");

    TRY(re = Regex("[a-z]+", Regex::dfa | Regex::icase));
    TEST_EQUAL(re.groups(), 1u);
    TRY(m = re("Hello world"));
    TEST(m);
    TEST_EQUAL(m.pos(), 0u);
    TEST_EQUAL(m.len(), 5u);
    TEST_EQUAL(m.str(), "Hello");
    TEST_EQUAL(m[0], "Hello");

}
