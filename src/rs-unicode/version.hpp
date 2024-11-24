#pragma once

#include <array>
#include <format>
#include <string>

namespace RS::Unicode {

    inline std::array<int, 3> version() noexcept {
        return {{ 0, 1, 3 }};
    }

    inline std::string version_string() {
        auto v = version();
        return std::format("{}.{}.{}", v[0], v[1], v[2]);
    }

}
