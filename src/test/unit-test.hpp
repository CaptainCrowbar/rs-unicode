#pragma once

#include <algorithm>
#include <exception>
#include <format>
#include <print>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif

namespace RS::UnitTest {

    const inline auto tty =
        #ifdef _WIN32
            _isatty(1) != 0;
        #else
            isatty(1) != 0;
        #endif

    const inline std::string rule(30, '=');
    const inline std::string xt_reset = tty ? "\x1b[0m" : "";

    inline std::string xt_rgb(int r, int g, int b) {
        if (tty) {
            auto index = 36 * r + 6 * g + b + 16;
            return "\x1b[38;5;" + std::to_string(index) + "m";
        } else {
            return {};
        }
    }

    const inline auto xhead = xt_rgb(5, 5, 3);
    const inline auto xrule = xt_rgb(3, 4, 5);
    const inline auto xtest = xt_rgb(5, 5, 3);
    const inline auto xpass = xt_rgb(2, 5, 2);
    const inline auto xfail = xt_rgb(5, 3, 1);

    template <typename> constexpr auto dependent_false = false;

    inline auto failures = 0;
    inline std::vector<std::string> main_args;

    struct TextMatch {

        std::string text;

        bool operator()(std::string pattern) const {

            std::regex_constants::syntax_option_type flags = {};

            if (pattern.ends_with("/i")) {
                flags |= std::regex_constants::icase;
                pattern.resize(pattern.size() - 2);
            }

            std::regex regex(pattern, flags);
            auto rc = std::regex_search(text, regex);

            return rc;

        }

    };

    inline void call_me_maybe(void (*test)(), const std::string& name) {
        if (main_args.empty() || std::ranges::any_of(main_args, TextMatch{name})) {
            std::println("{}{}{}", xtest, name, xt_reset);
            test();
        }
    }

}

// Explicitly trigger a failure.

#define FAIL(...) do { \
    std::println("{}... Test failed [{}:{}]{}\n\t{}", \
        ::RS::UnitTest::xfail, __FILE__, __LINE__, ::RS::UnitTest::xt_reset, std::format(__VA_ARGS__)); \
    ++::RS::UnitTest::failures; \
} while (false)

// Evaluate an expression. Fails if any exception is thrown.

#define TRY(expr) do { \
    try { \
        (expr); \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Evaluate a boolean expression. Fails if the expression is false, or if any
// exception is thrown.

#define TEST(expr) do { \
    try { \
        auto _test_flag = static_cast<bool>(expr); \
        if (! _test_flag) { \
            FAIL("Expression is false: {}", # expr); \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Same as TEST(), but quit the current test function on failure. This is
// intended for situations where a failure at this point would render the
// rest of the tests meaningless or impossible.

#define REQUIRE(expr) do { \
    try { \
        auto _test_flag = static_cast<bool>(expr); \
        if (! _test_flag) { \
            FAIL("Expression is false: {}", # expr); \
            return; \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
        return; \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
        return; \
    } \
} while (false)

// Compare two expressions for equality. Fails if the expressions are not
// equal, or if any exception is thrown.

#define TEST_EQUAL(lhs, rhs) do { \
    try { \
        auto _test_lhs {lhs}; \
        auto _test_rhs {rhs}; \
        if (_test_lhs != _test_rhs) { \
            FAIL("Expressions are not equal\n\t{} = {}\n\t{} = {}", \
                # lhs, _test_lhs, # rhs, _test_rhs); \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Check an expression against a range. Fails if the expression is outside the
// range, or if any exception is thrown.

#define TEST_IN_RANGE(expr, min, max) do { \
    try { \
        auto _test_expr {expr}; \
        auto _test_min {min}; \
        auto _test_max {max}; \
        if (_test_expr < _test_min || _test_expr > _test_max) \
            FAIL("Expression is out of range\n" \
                "\t{} = {}\n\tmin = {}, max = {}", \
                # expr, _test_expr, _test_min, _test_max); \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Compare an expression (expected to return a string) for a substring match
// against a regular expression. Fails if the result does not match or an
// exception is thrown. The regex pattern is passed as a string literal, and
// can be made case insensitive by appending "/i".

#define TEST_MATCH(expr, pattern) do { \
    try { \
        auto _test_string = static_cast<std::string>(expr); \
        if (! ::RS::UnitTest::TextMatch{_test_string}(pattern)) { \
            FAIL("Expression does not match pattern: {0:?}", _test_string); \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)

// Evaluate an expression that is expected to throw an exception of the given
// type, or a type derived from it. Fails if no exception is thrown, if an
// exception of the wrong type is thrown, or if the exception's type is
// correct but its what() message does not match the regular expression. The
// regex pattern is passed as a string literal, and can be made case
// insensitive by appending "/i".

#define TEST_THROW(expr, exception_type, pattern) do { \
    try { \
        (expr); \
        FAIL("No exception thrown: {}", # expr); \
    } \
    catch (const exception_type& _test_except) { \
        if (! ::RS::UnitTest::TextMatch{_test_except.what()}(pattern)) { \
            FAIL("Unexpected error message: {}", _test_except.what()); \
        } \
    } \
    catch (const std::exception& _test_except) { \
        FAIL("Unexpected exception: {}", _test_except.what()); \
    } \
    catch (...) { \
        FAIL("Unexpected exception"); \
    } \
} while (false)
