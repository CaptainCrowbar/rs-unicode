#include "rs-unicode/legacy.hpp"
#include "test/unit-test.hpp"
#include <string>

using namespace RS::Unicode;

void test_rs_unicode_legacy_conversion() {

    LegacyEncoding le;
    std::string a, b, c;

    a = "Hello αβγδε \xff";
    TRY(b = le.to_utf8(a));
    TEST_EQUAL(b, a);
    TRY(c = le.from_utf8(b));
    TEST_EQUAL(c, a);

    TRY(le = LegacyEncoding("windows-1252"));
    a = "Hello world";
    TRY(b = le.to_utf8(a));
    TEST_EQUAL(b, "Hello world");
    TRY(c = le.from_utf8(b));
    TEST_EQUAL(c, a);

    a = "\x80\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8e\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9e\x9f";
    TRY(b = le.to_utf8(a));
    TEST_EQUAL(b, "€‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ");
    TRY(c = le.from_utf8(b));
    TEST_EQUAL(c, a);

    a = "\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf";
    TRY(b = le.to_utf8(a));
    TEST_EQUAL(b, "¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿");
    TRY(c = le.from_utf8(b));
    TEST_EQUAL(c, a);

    TRY(le = LegacyEncoding(1252));

    a = "Hello world";
    TRY(b = le.to_utf8(a));
    TEST_EQUAL(b, "Hello world");
    TRY(c = le.from_utf8(b));
    TEST_EQUAL(c, a);

    a = "\x80\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8e\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9e\x9f";
    TRY(b = le.to_utf8(a));
    TEST_EQUAL(b, "€‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ");
    TRY(c = le.from_utf8(b));
    TEST_EQUAL(c, a);

    a = "\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf";
    TRY(b = le.to_utf8(a));
    TEST_EQUAL(b, "¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿");
    TRY(c = le.from_utf8(b));
    TEST_EQUAL(c, a);

}
