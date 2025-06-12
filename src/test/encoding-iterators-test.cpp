#include "rs-unicode/encoding.hpp"
#include "rs-core/unit-test.hpp"
#include <algorithm>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>

using namespace RS::Unicode;
namespace rs = std::ranges;

void test_rs_unicode_encoding_decoded_utf8_iterators() {

    std::string s;
    std::string_view sv;
    std::u32string u;
    DecodeUtf8Iterator i;

    s = "";             u.clear();  TRY(rs::copy(decoded_utf8_view(s), std::back_inserter(u)));  TEST_EQUAL(u.size(), 0u);   TEST(u == U"");
    s = "Hello world";  u.clear();  TRY(rs::copy(decoded_utf8_view(s), std::back_inserter(u)));  TEST_EQUAL(u.size(), 11u);  TEST(u == U"Hello world");
    s = "αβγδε";        u.clear();  TRY(rs::copy(decoded_utf8_view(s), std::back_inserter(u)));  TEST_EQUAL(u.size(), 5u);   TEST(u == U"αβγδε");
    s = "αβγ¹²³";       u.clear();  TRY(rs::copy(decoded_utf8_view(s), std::back_inserter(u)));  TEST_EQUAL(u.size(), 6u);   TEST(u == U"αβγ¹²³");
    s = "€😀❤️‍🔥";       u.clear();  TRY(rs::copy(decoded_utf8_view(s), std::back_inserter(u)));  TEST_EQUAL(u.size(), 6u);   TEST(u == U"€😀❤️‍🔥");

    s = "Hello";
    TRY(i = decoded_utf8_view(s).begin());
    /**/       TEST_EQUAL(i.view(), "H");
    TRY(++i);  TEST_EQUAL(i.view(), "e");
    TRY(++i);  TEST_EQUAL(i.view(), "l");
    TRY(++i);  TEST_EQUAL(i.view(), "l");
    TRY(++i);  TEST_EQUAL(i.view(), "o");
    TRY(++i);  TEST(i == decoded_utf8_view(s).end());

    s = "αβγδε";
    TRY(i = decoded_utf8_view(s).begin());
    /**/       TEST_EQUAL(i.view(), "α");
    TRY(++i);  TEST_EQUAL(i.view(), "β");
    TRY(++i);  TEST_EQUAL(i.view(), "γ");
    TRY(++i);  TEST_EQUAL(i.view(), "δ");
    TRY(++i);  TEST_EQUAL(i.view(), "ε");
    TRY(++i);  TEST(i == decoded_utf8_view(s).end());

    s = "αβγ¹²³";
    TRY(i = decoded_utf8_view(s).begin());
    /**/       TEST_EQUAL(i.view(), "α");
    TRY(++i);  TEST_EQUAL(i.view(), "β");
    TRY(++i);  TEST_EQUAL(i.view(), "γ");
    TRY(++i);  TEST_EQUAL(i.view(), "¹");
    TRY(++i);  TEST_EQUAL(i.view(), "²");
    TRY(++i);  TEST_EQUAL(i.view(), "³");
    TRY(++i);  TEST(i == decoded_utf8_view(s).end());

    s = "Aαβγ€😀";
    TRY(i = decoded_utf8_view(s).end());
    TRY(--i);  TEST(*i == U'😀');
    TRY(--i);  TEST(*i == U'€');
    TRY(--i);  TEST(*i == U'γ');
    TRY(--i);  TEST(*i == U'β');
    TRY(--i);  TEST(*i == U'α');
    TRY(--i);  TEST(*i == U'A');
    TEST(i == decoded_utf8_view(s).begin());

    // s = 41, ce b1, ce b2, ce b3, e2 82 ac, f0 9f 98 80
    sv = {s.begin() + 1, s.begin() + 7};
    TRY(i = decoded_utf8_view(sv).begin());
    /**/       TEST_EQUAL(i.view(), "α");
    TRY(++i);  TEST_EQUAL(i.view(), "β");
    TRY(++i);  TEST_EQUAL(i.view(), "γ");
    TRY(++i);  TEST(i == decoded_utf8_view(sv).end());

}

void test_rs_unicode_encoding_encoded_utf8_iterators() {

    std::string s;
    std::u32string u;
    EncodeUtf8Iterator i;

    u = U"";             s.clear();  TRY(rs::copy(encoded_utf8_view(u), std::back_inserter(s)));  TEST_EQUAL(s.size(), 0u);   TEST_EQUAL(s, "");
    u = U"Hello world";  s.clear();  TRY(rs::copy(encoded_utf8_view(u), std::back_inserter(s)));  TEST_EQUAL(s.size(), 11u);  TEST_EQUAL(s, "Hello world");
    u = U"αβγδε";        s.clear();  TRY(rs::copy(encoded_utf8_view(u), std::back_inserter(s)));  TEST_EQUAL(s.size(), 10u);  TEST_EQUAL(s, "αβγδε");
    u = U"αβγ¹²³";       s.clear();  TRY(rs::copy(encoded_utf8_view(u), std::back_inserter(s)));  TEST_EQUAL(s.size(), 12u);  TEST_EQUAL(s, "αβγ¹²³");
    u = U"€😀❤️‍🔥";       s.clear();  TRY(rs::copy(encoded_utf8_view(u), std::back_inserter(s)));  TEST_EQUAL(s.size(), 20u);  TEST_EQUAL(s, "€😀❤️‍🔥");

    u = U"Aαβγ€😀";
    TRY(i = encoded_utf8_view(u).begin());
    /**/       TEST_EQUAL(*i, '\x41');
    TRY(++i);  TEST_EQUAL(*i, '\xce');
    TRY(++i);  TEST_EQUAL(*i, '\xb1');
    TRY(++i);  TEST_EQUAL(*i, '\xce');
    TRY(++i);  TEST_EQUAL(*i, '\xb2');
    TRY(++i);  TEST_EQUAL(*i, '\xce');
    TRY(++i);  TEST_EQUAL(*i, '\xb3');
    TRY(++i);  TEST_EQUAL(*i, '\xe2');
    TRY(++i);  TEST_EQUAL(*i, '\x82');
    TRY(++i);  TEST_EQUAL(*i, '\xac');
    TRY(++i);  TEST_EQUAL(*i, '\xf0');
    TRY(++i);  TEST_EQUAL(*i, '\x9f');
    TRY(++i);  TEST_EQUAL(*i, '\x98');
    TRY(++i);  TEST_EQUAL(*i, '\x80');
    TRY(++i);  TEST(i == encoded_utf8_view(u).end());

    TRY(i = encoded_utf8_view(u).end());
    TRY(--i);  TEST_EQUAL(*i, '\x80');
    TRY(--i);  TEST_EQUAL(*i, '\x98');
    TRY(--i);  TEST_EQUAL(*i, '\x9f');
    TRY(--i);  TEST_EQUAL(*i, '\xf0');
    TRY(--i);  TEST_EQUAL(*i, '\xac');
    TRY(--i);  TEST_EQUAL(*i, '\x82');
    TRY(--i);  TEST_EQUAL(*i, '\xe2');
    TRY(--i);  TEST_EQUAL(*i, '\xb3');
    TRY(--i);  TEST_EQUAL(*i, '\xce');
    TRY(--i);  TEST_EQUAL(*i, '\xb2');
    TRY(--i);  TEST_EQUAL(*i, '\xce');
    TRY(--i);  TEST_EQUAL(*i, '\xb1');
    TRY(--i);  TEST_EQUAL(*i, '\xce');
    TRY(--i);  TEST_EQUAL(*i, '\x41');
    TEST(i == encoded_utf8_view(u).begin());

}
