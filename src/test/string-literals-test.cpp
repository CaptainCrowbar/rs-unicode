#include "rs-unicode/string.hpp"
#include "rs-core/unit-test.hpp"
#include <string>

using namespace RS::Unicode;
using namespace RS::Unicode::Literals;

void test_rs_unicode_string_literals() {

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
