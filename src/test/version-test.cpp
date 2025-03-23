#include "rs-unicode/version.hpp"
#include "test/unit-test.hpp"
#include <print>

using namespace RS::Unicode;

void test_rs_unicode_version() {

    TEST_IN_RANGE(library_version()[0], 0, 100);
    TEST_IN_RANGE(library_version()[1], 0, 1'000);
    TEST_IN_RANGE(library_version()[2], 0, 10'000);
    TEST_MATCH(library_version_string(), R"(\d+\.\d+\.\d+)");

    TEST_IN_RANGE(unicode_version()[0], 16, 100);
    TEST_IN_RANGE(unicode_version()[1], 0, 100);
    TEST_IN_RANGE(unicode_version()[2], 0, 100);
    TEST_MATCH(unicode_version_string(), R"(\d+\.\d+\.\d+)");

    std::println("... Library version {}", library_version_string());
    std::println("... Unicode version {}", unicode_version_string());

}
