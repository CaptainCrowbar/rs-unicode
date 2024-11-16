#pragma once

#include "rs-unicode/character.hpp"
#include "rs-unicode/encoding.hpp"
#include <cstdint>
#include <string>
#include <string_view>

namespace RS::Unicode {

    class LegacyEncoding {

    public:

        LegacyEncoding() = default;
        explicit LegacyEncoding(const std::string& name);
        explicit LegacyEncoding(std::uint32_t key);

        std::string from_utf8(std::string_view input) const;
        std::string to_utf8(std::string_view input) const;

    private:

        #ifdef _WIN32
            std::uint32_t page_ = 0;
        #else
            std::string name_;
        #endif

    };

}
