#include "rs-unicode/string.hpp"
#include "rs-core/unit-test.hpp"
#include <format>
#include <string>
#include <vector>

using namespace RS::Unicode;
using namespace RS::Unicode::Literals;

void test_rs_unicode_string_literals_doc() {

    std::string s;

    TRY(s = ""_doc);
    TEST_EQUAL(s, "");
    TRY(s = "Hello world"_doc);
    TEST_EQUAL(s, "Hello world\n");
    TRY(s = "Hello world\n"_doc);
    TEST_EQUAL(s, "Hello world\n");

    TRY(s = R"(
        )"_doc);
    TEST_EQUAL(s, "");

    TRY(s = R"(

        )"_doc);
    TEST_EQUAL(s, "\n");

    TRY(s = R"(
        Alpha
        )"_doc);
    TEST_EQUAL(s,
        "Alpha\n"
    );

    TRY(s = R"(
        Alpha
    )"_doc);
    TEST_EQUAL(s,
        "    Alpha\n"
    );

    TRY(s = R"(
        Alpha
        Bravo
        )"_doc);
    TEST_EQUAL(s,
        "Alpha\n"
        "Bravo\n"
    );

    TRY(s = R"(
        Alpha
        Bravo
    )"_doc);
    TEST_EQUAL(s,
        "    Alpha\n"
        "    Bravo\n"
    );

    TRY(s = R"(
        Alpha
        Bravo
        Charlie
        )"_doc);
    TEST_EQUAL(s,
        "Alpha\n"
        "Bravo\n"
        "Charlie\n"
    );

    TRY(s = R"(
        Alpha
        Bravo
        Charlie
    )"_doc);
    TEST_EQUAL(s,
        "    Alpha\n"
        "    Bravo\n"
        "    Charlie\n"
    );

    TRY(s = R"(

        Alpha
        Bravo

        Charlie
        Delta

        )"_doc);
    TEST_EQUAL(s,
        "\n"
        "Alpha\n"
        "Bravo\n"
        "\n"
        "Charlie\n"
        "Delta\n"
        "\n"
    );

    TRY(s = R"(

        Alpha
        Bravo

        Charlie
        Delta

    )"_doc);
    TEST_EQUAL(s,
        "\n"
        "    Alpha\n"
        "    Bravo\n"
        "\n"
        "    Charlie\n"
        "    Delta\n"
        "\n"
    );

    TRY(s = R"(
        Alpha
            Bravo
        Charlie
        )"_doc);
    TEST_EQUAL(s,
        "Alpha\n"
        "    Bravo\n"
        "Charlie\n"
    );

    TRY(s = R"(
        Alpha
            Bravo
        Charlie
    )"_doc);
    TEST_EQUAL(s,
        "    Alpha\n"
        "        Bravo\n"
        "    Charlie\n"
    );

    TRY(s = R"(
        Alpha
            Bravo
                Charlie
        )"_doc);
    TEST_EQUAL(s,
        "Alpha\n"
        "    Bravo\n"
        "        Charlie\n"
    );

    TRY(s = R"(
        Alpha
            Bravo
                Charlie
    )"_doc);
    TEST_EQUAL(s,
        "    Alpha\n"
        "        Bravo\n"
        "            Charlie\n"
    );

    TRY(s = R"(
                Alpha
            Bravo
        Charlie
    )"_doc);
    TEST_EQUAL(s,
        "            Alpha\n"
        "        Bravo\n"
        "    Charlie\n"
    );

}

void test_rs_unicode_string_literals_qw() {

    std::vector<std::string> v;

    TRY(v = ""_qw);
    TEST_EQUAL(v.size(), 0u);
    TEST_EQUAL(std::format("{}", v), R"([])");

    TRY(v = " \r\n"_qw);
    TEST_EQUAL(v.size(), 0u);
    TEST_EQUAL(std::format("{}", v), R"([])");

    TRY(v = "Alpha"_qw);
    TEST_EQUAL(v.size(), 1u);
    TEST_EQUAL(std::format("{}", v), R"(["Alpha"])");

    TRY(v = "Alpha Bravo"_qw);
    TEST_EQUAL(v.size(), 2u);
    TEST_EQUAL(std::format("{}", v), R"(["Alpha", "Bravo"])");

    TRY(v = "Alpha Bravo Charlie"_qw);
    TEST_EQUAL(v.size(), 3u);
    TEST_EQUAL(std::format("{}", v), R"(["Alpha", "Bravo", "Charlie"])");

    TRY(v = "\r\nAlpha\r\nBravo\r\nCharlie\r\n"_qw);
    TEST_EQUAL(v.size(), 3u);
    TEST_EQUAL(std::format("{}", v), R"(["Alpha", "Bravo", "Charlie"])");

}
