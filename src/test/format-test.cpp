#include "rs-unicode/format.hpp"
#include "test/unit-test.hpp"
#include <cstddef>
#include <format>
#include <string_view>

using namespace RS::Unicode;

namespace MyNamespace {

    struct Alpha {
        int num = 0;
        auto str() const {
            return std::format("A({})", num);
        }
    };

    struct Bravo {
        int num = 0;
        auto str(std::string_view f = {}) const {
            return std::format("B({},{})", num, f);
        }
    };

    struct Charlie {
        int num = 0;
        auto str(std::size_t z = 0) const {
            return std::format("C({},{})", num, z);
        }
    };

    struct Delta {
        int num = 0;
        auto str(std::string_view f = {}, std::size_t z = 0) const {
            return std::format("D({},{},{})", num, f, z);
        }
    };

    struct Echo {
        int num = 0;
    };

    auto to_string(const Echo& t) {
        return std::format("E({})", t.num);
    }

    struct Foxtrot {
        int num = 0;
    };

    auto to_string(const Foxtrot& t, std::string_view f = {}) {
        return std::format("F({},{})", t.num, f);
    }

    struct Golf {
        int num = 0;
    };

    auto to_string(const Golf& t, std::size_t z = 0) {
        return std::format("G({},{})", t.num, z);
    }

    struct Hotel {
        int num = 0;
    };

    auto to_string(const Hotel& t, std::string_view f = {}, std::size_t z = 0) {
        return std::format("H({},{},{})", t.num, f, z);
    }

    void rs_unicode_format(Alpha);
    void rs_unicode_format(Bravo);
    void rs_unicode_format(Charlie);
    void rs_unicode_format(Delta);
    void rs_unicode_format(Echo);
    void rs_unicode_format(Foxtrot);
    void rs_unicode_format(Golf);
    void rs_unicode_format(Hotel);

}

void test_rs_unicode_format_concepts() {

    using namespace Detail;

    static_assert(std::formattable<MyNamespace::Alpha, char>);
    static_assert(std::formattable<MyNamespace::Bravo, char>);
    static_assert(std::formattable<MyNamespace::Charlie, char>);
    static_assert(std::formattable<MyNamespace::Delta, char>);
    static_assert(std::formattable<MyNamespace::Echo, char>);
    static_assert(std::formattable<MyNamespace::Foxtrot, char>);
    static_assert(std::formattable<MyNamespace::Golf, char>);
    static_assert(std::formattable<MyNamespace::Hotel, char>);
    static_assert(FormatByStrMethod<MyNamespace::Alpha>);
    static_assert(FormatByStrMethod<MyNamespace::Bravo>);
    static_assert(FormatByStrMethod<MyNamespace::Charlie>);
    static_assert(FormatByStrMethod<MyNamespace::Delta>);
    static_assert(! FormatByStrMethod<MyNamespace::Echo>);
    static_assert(! FormatByStrMethod<MyNamespace::Foxtrot>);
    static_assert(! FormatByStrMethod<MyNamespace::Golf>);
    static_assert(! FormatByStrMethod<MyNamespace::Hotel>);
    static_assert(! FormatByStrMethodWithFlags<MyNamespace::Alpha>);
    static_assert(FormatByStrMethodWithFlags<MyNamespace::Bravo>);
    static_assert(! FormatByStrMethodWithFlags<MyNamespace::Charlie>);
    static_assert(FormatByStrMethodWithFlags<MyNamespace::Delta>);
    static_assert(! FormatByStrMethodWithFlags<MyNamespace::Echo>);
    static_assert(! FormatByStrMethodWithFlags<MyNamespace::Foxtrot>);
    static_assert(! FormatByStrMethodWithFlags<MyNamespace::Golf>);
    static_assert(! FormatByStrMethodWithFlags<MyNamespace::Hotel>);
    static_assert(! FormatByStrMethodWithSize<MyNamespace::Alpha>);
    static_assert(! FormatByStrMethodWithSize<MyNamespace::Bravo>);
    static_assert(FormatByStrMethodWithSize<MyNamespace::Charlie>);
    static_assert(! FormatByStrMethodWithSize<MyNamespace::Delta>);
    static_assert(! FormatByStrMethodWithSize<MyNamespace::Echo>);
    static_assert(! FormatByStrMethodWithSize<MyNamespace::Foxtrot>);
    static_assert(! FormatByStrMethodWithSize<MyNamespace::Golf>);
    static_assert(! FormatByStrMethodWithSize<MyNamespace::Hotel>);
    static_assert(! FormatByStrMethodWithBoth<MyNamespace::Alpha>);
    static_assert(! FormatByStrMethodWithBoth<MyNamespace::Bravo>);
    static_assert(! FormatByStrMethodWithBoth<MyNamespace::Charlie>);
    static_assert(FormatByStrMethodWithBoth<MyNamespace::Delta>);
    static_assert(! FormatByStrMethodWithBoth<MyNamespace::Echo>);
    static_assert(! FormatByStrMethodWithBoth<MyNamespace::Foxtrot>);
    static_assert(! FormatByStrMethodWithBoth<MyNamespace::Golf>);
    static_assert(! FormatByStrMethodWithBoth<MyNamespace::Hotel>);
    static_assert(! FormatByToString<MyNamespace::Alpha>);
    static_assert(! FormatByToString<MyNamespace::Bravo>);
    static_assert(! FormatByToString<MyNamespace::Charlie>);
    static_assert(! FormatByToString<MyNamespace::Delta>);
    static_assert(FormatByToString<MyNamespace::Echo>);
    static_assert(FormatByToString<MyNamespace::Foxtrot>);
    static_assert(FormatByToString<MyNamespace::Golf>);
    static_assert(FormatByToString<MyNamespace::Hotel>);
    static_assert(! FormatByToStringWithFlags<MyNamespace::Alpha>);
    static_assert(! FormatByToStringWithFlags<MyNamespace::Bravo>);
    static_assert(! FormatByToStringWithFlags<MyNamespace::Charlie>);
    static_assert(! FormatByToStringWithFlags<MyNamespace::Delta>);
    static_assert(! FormatByToStringWithFlags<MyNamespace::Echo>);
    static_assert(FormatByToStringWithFlags<MyNamespace::Foxtrot>);
    static_assert(! FormatByToStringWithFlags<MyNamespace::Golf>);
    static_assert(FormatByToStringWithFlags<MyNamespace::Hotel>);
    static_assert(! FormatByToStringWithSize<MyNamespace::Alpha>);
    static_assert(! FormatByToStringWithSize<MyNamespace::Bravo>);
    static_assert(! FormatByToStringWithSize<MyNamespace::Charlie>);
    static_assert(! FormatByToStringWithSize<MyNamespace::Delta>);
    static_assert(! FormatByToStringWithSize<MyNamespace::Echo>);
    static_assert(! FormatByToStringWithSize<MyNamespace::Foxtrot>);
    static_assert(FormatByToStringWithSize<MyNamespace::Golf>);
    static_assert(! FormatByToStringWithSize<MyNamespace::Hotel>);
    static_assert(! FormatByToStringWithBoth<MyNamespace::Alpha>);
    static_assert(! FormatByToStringWithBoth<MyNamespace::Bravo>);
    static_assert(! FormatByToStringWithBoth<MyNamespace::Charlie>);
    static_assert(! FormatByToStringWithBoth<MyNamespace::Delta>);
    static_assert(! FormatByToStringWithBoth<MyNamespace::Echo>);
    static_assert(! FormatByToStringWithBoth<MyNamespace::Foxtrot>);
    static_assert(! FormatByToStringWithBoth<MyNamespace::Golf>);
    static_assert(FormatByToStringWithBoth<MyNamespace::Hotel>);
    static_assert(AutoFormat<MyNamespace::Alpha>);
    static_assert(AutoFormat<MyNamespace::Bravo>);
    static_assert(AutoFormat<MyNamespace::Charlie>);
    static_assert(AutoFormat<MyNamespace::Delta>);
    static_assert(AutoFormat<MyNamespace::Echo>);
    static_assert(AutoFormat<MyNamespace::Foxtrot>);
    static_assert(AutoFormat<MyNamespace::Golf>);
    static_assert(AutoFormat<MyNamespace::Hotel>);

}

void test_rs_unicode_format_automation() {

    MyNamespace::Alpha a{10};
    MyNamespace::Bravo b{20};
    MyNamespace::Charlie c{30};
    MyNamespace::Delta d{40};
    MyNamespace::Echo e{50};
    MyNamespace::Foxtrot f{60};
    MyNamespace::Golf g{70};
    MyNamespace::Hotel h{80};

    TEST_EQUAL(std::format("{}", a),          "A(10)");
    TEST_EQUAL(std::format("{:beta}", b),     "B(20,beta)");
    TEST_EQUAL(std::format("{:86}", c),       "C(30,86)");
    TEST_EQUAL(std::format("{:delta99}", d),  "D(40,delta,99)");
    TEST_EQUAL(std::format("{}", e),          "E(50)");
    TEST_EQUAL(std::format("{:zeta}", f),     "F(60,zeta)");
    TEST_EQUAL(std::format("{:86}", g),       "G(70,86)");
    TEST_EQUAL(std::format("{:theta99}", h),  "H(80,theta,99)");

}
