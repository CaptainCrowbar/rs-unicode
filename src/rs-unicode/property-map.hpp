#pragma once

#include <algorithm>
#include <compare>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <vector>

namespace RS::Unicode {

    class BinaryPropertyMap {

    private:

        struct entry_type {
            char32_t min;
            char32_t max;
            bool operator==(const entry_type& t) const noexcept { return min == t.min; }
            auto operator<=>(const entry_type& t) const noexcept { return min <=> t.min; }
        };

        std::vector<entry_type> table_;

    public:

        BinaryPropertyMap(std::initializer_list<entry_type> list): table_(list) {}

        bool operator[](char32_t c) const noexcept {
            if (c > 0x10'ffff) {
                return false;
            }
            auto it = std::ranges::upper_bound(table_, entry_type{c, {}});
            return it != table_.begin() && c <= std::prev(it)->max;
        }

    };

    template <typename T>
    class GeneralPropertyMap {

    private:

        struct entry_type {
            char32_t code;
            T prop;
            bool operator==(const entry_type& t) const noexcept { return code == t.code; }
            auto operator<=>(const entry_type& t) const noexcept { return code <=> t.code; }
        };

        std::vector<entry_type> table_;

    public:

        GeneralPropertyMap(std::initializer_list<entry_type> list): table_(list) {}

        T operator[](char32_t c) const noexcept {
            if (c > 0x10'ffff) {
                return {};
            }
            auto it = std::ranges::upper_bound(table_, entry_type{c, {}});
            if (it == table_.begin()) {
                return {};
            } else {
                return std::prev(it)->prop;
            }
        }

    };

}
