#include "rs-unicode/enum.hpp"
#include "test/unit-test.hpp"
#include <concepts>
#include <cstdint>
#include <format>
#include <type_traits>

using namespace RS::Unicode;

namespace {

    RS_UNICODE_ENUM(Simple, int,
        alpha,
        bravo,
        charlie,
    )

    RS_UNICODE_ENUM(Complicated, unsigned,
        alpha,
        bravo = 10,
        charlie,
        delta = 0x20,
        echo,
    )

    RS_UNICODE_BITMASK(Mask, std::uint16_t,
        none = 0,
        alpha = 1,
        bravo = 2,
        charlie = 4,
    )

}

void test_rs_unicode_enum() {

    static_assert(std::same_as<std::underlying_type_t<Simple>, int>);
    static_assert(std::same_as<std::underlying_type_t<Complicated>, unsigned>);
    static_assert(std::formattable<Simple, char>);
    static_assert(std::formattable<Complicated, char>);

    TEST_EQUAL(static_cast<int>(Simple::alpha),         0);
    TEST_EQUAL(static_cast<int>(Simple::bravo),         1);
    TEST_EQUAL(static_cast<int>(Simple::charlie),       2);
    TEST_EQUAL(static_cast<int>(Complicated::alpha),    0);
    TEST_EQUAL(static_cast<int>(Complicated::bravo),    10);
    TEST_EQUAL(static_cast<int>(Complicated::charlie),  11);
    TEST_EQUAL(static_cast<int>(Complicated::delta),    32);
    TEST_EQUAL(static_cast<int>(Complicated::echo),     33);

    TEST_EQUAL(to_string(Simple::alpha),                  "alpha");
    TEST_EQUAL(to_string(Simple::bravo),                  "bravo");
    TEST_EQUAL(to_string(Simple::charlie),                "charlie");
    TEST_EQUAL(to_string(static_cast<Simple>(-1)),        "-1");
    TEST_EQUAL(to_string(static_cast<Simple>(3)),         "3");
    TEST_EQUAL(to_string(Complicated::alpha),             "alpha");
    TEST_EQUAL(to_string(Complicated::bravo),             "bravo");
    TEST_EQUAL(to_string(Complicated::charlie),           "charlie");
    TEST_EQUAL(to_string(Complicated::delta),             "delta");
    TEST_EQUAL(to_string(Complicated::echo),              "echo");
    TEST_EQUAL(to_string(static_cast<Complicated>(1)),    "1");
    TEST_EQUAL(to_string(static_cast<Complicated>(999)),  "999");

    TEST_EQUAL(std::format("{}", Simple::alpha),                  "alpha");
    TEST_EQUAL(std::format("{}", Simple::bravo),                  "bravo");
    TEST_EQUAL(std::format("{}", Simple::charlie),                "charlie");
    TEST_EQUAL(std::format("{}", static_cast<Simple>(-1)),        "-1");
    TEST_EQUAL(std::format("{}", static_cast<Simple>(3)),         "3");
    TEST_EQUAL(std::format("{}", Complicated::alpha),             "alpha");
    TEST_EQUAL(std::format("{}", Complicated::bravo),             "bravo");
    TEST_EQUAL(std::format("{}", Complicated::charlie),           "charlie");
    TEST_EQUAL(std::format("{}", Complicated::delta),             "delta");
    TEST_EQUAL(std::format("{}", Complicated::echo),              "echo");
    TEST_EQUAL(std::format("{}", static_cast<Complicated>(1)),    "1");
    TEST_EQUAL(std::format("{}", static_cast<Complicated>(999)),  "999");

    TEST_EQUAL(std::format("{}", enum_values(Simple{})),       "{alpha, bravo, charlie}");
    TEST_EQUAL(std::format("{}", enum_values(Complicated{})),  "{alpha, bravo, charlie, delta, echo}");

}

void test_rs_unicode_enum_bitmask() {

    static_assert(std::same_as<std::underlying_type_t<Mask>, std::uint16_t>);
    static_assert(std::formattable<Mask, char>);

    TEST_EQUAL(static_cast<int>(Mask::none),     0);
    TEST_EQUAL(static_cast<int>(Mask::alpha),    1);
    TEST_EQUAL(static_cast<int>(Mask::bravo),    2);
    TEST_EQUAL(static_cast<int>(Mask::charlie),  4);

    TEST(! Mask::none);
    TEST(!! Mask::alpha);
    TEST(!! Mask::bravo);
    TEST(!! Mask::charlie);

    TEST_EQUAL(static_cast<int>(~ Mask::none),                                                 0xffff);
    TEST_EQUAL(static_cast<int>(~ Mask::alpha),                                                0xfffe);
    TEST_EQUAL(static_cast<int>(~ Mask::bravo),                                                0xfffd);
    TEST_EQUAL(static_cast<int>(~ Mask::charlie),                                              0xfffb);
    TEST_EQUAL(static_cast<int>(Mask::alpha | Mask::bravo),                                    3);
    TEST_EQUAL(static_cast<int>((Mask::alpha | Mask::bravo) & Mask::alpha),                    1);
    TEST_EQUAL(static_cast<int>((Mask::alpha | Mask::bravo) & Mask::charlie),                  0);
    TEST_EQUAL(static_cast<int>((Mask::alpha | Mask::bravo) ^ (Mask::bravo | Mask::charlie)),  5);

    TEST_EQUAL(to_string(Mask::none),                                 "none");
    TEST_EQUAL(to_string(Mask::alpha),                                "alpha");
    TEST_EQUAL(to_string(Mask::bravo),                                "bravo");
    TEST_EQUAL(to_string(Mask::charlie),                              "charlie");
    TEST_EQUAL(to_string(Mask::alpha | Mask::bravo),                  "alpha|bravo");
    TEST_EQUAL(to_string(Mask::alpha | Mask::charlie),                "alpha|charlie");
    TEST_EQUAL(to_string(Mask::bravo | Mask::charlie),                "bravo|charlie");
    TEST_EQUAL(to_string(Mask::alpha | Mask::bravo | Mask::charlie),  "alpha|bravo|charlie");
    TEST_EQUAL(to_string(static_cast<Mask>(0xffff)),                  "0xffff");

    TEST_EQUAL(std::format("{}", Mask::none),                                 "none");
    TEST_EQUAL(std::format("{}", Mask::alpha),                                "alpha");
    TEST_EQUAL(std::format("{}", Mask::bravo),                                "bravo");
    TEST_EQUAL(std::format("{}", Mask::charlie),                              "charlie");
    TEST_EQUAL(std::format("{}", Mask::alpha | Mask::bravo),                  "alpha|bravo");
    TEST_EQUAL(std::format("{}", Mask::alpha | Mask::charlie),                "alpha|charlie");
    TEST_EQUAL(std::format("{}", Mask::bravo | Mask::charlie),                "bravo|charlie");
    TEST_EQUAL(std::format("{}", Mask::alpha | Mask::bravo | Mask::charlie),  "alpha|bravo|charlie");
    TEST_EQUAL(std::format("{}", static_cast<Mask>(0xffff)),                  "0xffff");

    TEST_EQUAL(std::format("{}", enum_values(Mask{})), "{none, alpha, bravo, charlie}");

}
