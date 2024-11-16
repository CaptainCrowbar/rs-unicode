#include "rs-unicode/property-map.hpp"
#include "test/unit-test.hpp"

using namespace RS::Unicode;

void test_rs_unicode_binary_property_map() {

    BinaryPropertyMap map {
        { U'0', U'9'},
        { U'A', U'Z'},
        { U'a', U'z'},
    };

    TEST(! map[0]);
    TEST(! map[U' ']);
    TEST(! map[U'/']);
    TEST(map[U'0']);
    TEST(map[U'9']);
    TEST(! map[U':']);
    TEST(! map[U'@']);
    TEST(map[U'A']);
    TEST(map[U'Z']);
    TEST(! map[U'[']);
    TEST(! map[U'`']);
    TEST(map[U'a']);
    TEST(map[U'z']);
    TEST(! map[U'{']);
    TEST(! map[U'~']);

}

void test_rs_unicode_general_property_map() {

    GeneralPropertyMap<int> map {
        { U'0', 1},
        { U':', 0},
        { U'A', 2},
        { U'[', 0},
        { U'a', 3},
        { U'{', 0},
    };

    TEST_EQUAL(map[0], 0);
    TEST_EQUAL(map[U' '], 0);
    TEST_EQUAL(map[U'/'], 0);
    TEST_EQUAL(map[U'0'], 1);
    TEST_EQUAL(map[U'9'], 1);
    TEST_EQUAL(map[U':'], 0);
    TEST_EQUAL(map[U'@'], 0);
    TEST_EQUAL(map[U'A'], 2);
    TEST_EQUAL(map[U'Z'], 2);
    TEST_EQUAL(map[U'['], 0);
    TEST_EQUAL(map[U'`'], 0);
    TEST_EQUAL(map[U'a'], 3);
    TEST_EQUAL(map[U'z'], 3);
    TEST_EQUAL(map[U'{'], 0);
    TEST_EQUAL(map[U'~'], 0);

}
