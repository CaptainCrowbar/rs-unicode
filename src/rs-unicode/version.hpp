#pragma once

#include <array>
#include <format>
#include <string>

namespace RS::Unicode {

    namespace Detail {

        inline std::string version_to_string(std::array<int, 3> v) {
            return std::format("{}.{}.{}", v[0], v[1], v[2]);
        }

    }

    inline std::array<int, 3> library_version() noexcept {
        return { 0, 2, 13 }; // Library version
    }

    inline std::array<int, 3> unicode_version() noexcept {
        return { 16, 0, 0 }; // Unicode version
    }

    inline std::string library_version_string() {
        return Detail::version_to_string(library_version());
    }

    inline std::string unicode_version_string() {
        return Detail::version_to_string(unicode_version());
    }

}
