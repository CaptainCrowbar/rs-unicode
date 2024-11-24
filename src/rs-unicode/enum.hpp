#pragma once

#include "rs-unicode/format.hpp"
#include <concepts>
#include <format>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#define RS_UNICODE_ENUM(EnumType, IntType, ...) \
    enum class EnumType: IntType { \
        __VA_ARGS__ \
    }; \
    [[maybe_unused]] inline const std::set<EnumType>& enum_values(EnumType) { \
        static const auto set = [] { \
            std::set<EnumType> set; \
            if constexpr (std::signed_integral<IntType>) { \
                auto table = ::RS::Unicode::Detail::signed_enum_table(# __VA_ARGS__); \
                for (auto& [k,v]: table) { \
                    set.insert(static_cast<EnumType>(k)); \
                } \
            } else { \
                auto table = ::RS::Unicode::Detail::unsigned_enum_table(# __VA_ARGS__); \
                for (auto& [k,v]: table) { \
                    set.insert(static_cast<EnumType>(k)); \
                } \
            } \
            return set; \
        }(); \
        return set; \
    } \
    [[maybe_unused]] inline std::string to_string(EnumType t) { \
        static const auto table = [] { \
            if constexpr (std::signed_integral<IntType>) { \
                return ::RS::Unicode::Detail::signed_enum_table(# __VA_ARGS__); \
            } else { \
                return ::RS::Unicode::Detail::unsigned_enum_table(# __VA_ARGS__); \
            } \
        }(); \
        auto index = static_cast<IntType>(t); \
        auto it = table.find(index); \
        if (it == table.end()) { \
            return std::to_string(index); \
        } else { \
            return it->second; \
        } \
    } \
    [[maybe_unused]] void rs_unicode_enum(EnumType); \
    [[maybe_unused]] void rs_unicode_format(EnumType);

#define RS_UNICODE_BITMASK(EnumType, IntType, ...) \
    enum class EnumType: IntType { \
        __VA_ARGS__ \
    }; \
    [[maybe_unused]] inline const std::set<EnumType>& enum_values(EnumType) { \
        static const auto set = [] { \
            std::set<EnumType> set {}; \
            auto table = ::RS::Unicode::Detail::unsigned_enum_table(# __VA_ARGS__); \
            for (auto& [k,v]: table) { \
                set.insert(static_cast<EnumType>(k)); \
            } \
            return set; \
        }(); \
        return set; \
    } \
    [[maybe_unused]] inline std::string to_string(EnumType t) { \
        static_assert(std::unsigned_integral<IntType>); \
        static const auto table = ::RS::Unicode::Detail::linear_enum_table(# __VA_ARGS__); \
        auto index = static_cast<unsigned long long>(t); \
        if (index == 0) { \
            return "none"; \
        } \
        auto residue = index; \
        std::string str; \
        for (const auto& [number,name]: table) { \
            if ((number & residue) != 0) { \
                str += name + '|'; \
                residue &= ~ number; \
            } \
        } \
        if (residue == 0) { \
            str.pop_back(); \
            return str; \
        } else { \
            return std::format("0x{:x}", index); \
        } \
    } \
    [[maybe_unused]] constexpr bool operator!(EnumType t) noexcept { \
        return ! static_cast<bool>(t); \
    } \
    [[maybe_unused]] constexpr EnumType operator~(EnumType t) noexcept { \
        return static_cast<EnumType>(~ static_cast<IntType>(t)); \
    } \
    [[maybe_unused]] constexpr EnumType operator&(EnumType t, EnumType u) noexcept { \
        return static_cast<EnumType>(static_cast<IntType>(t) & static_cast<IntType>(u)); \
    } \
    [[maybe_unused]] constexpr EnumType operator|(EnumType t, EnumType u) noexcept { \
        return static_cast<EnumType>(static_cast<IntType>(t) | static_cast<IntType>(u)); \
    } \
    [[maybe_unused]] constexpr EnumType operator^(EnumType t, EnumType u) noexcept { \
        return static_cast<EnumType>(static_cast<IntType>(t) ^ static_cast<IntType>(u)); \
    } \
    [[maybe_unused]] constexpr EnumType& operator&=(EnumType& t, EnumType u) noexcept { \
        return t = t & u; \
    } \
    [[maybe_unused]] constexpr EnumType& operator|=(EnumType& t, EnumType u) noexcept { \
        return t = t | u; \
    } \
    [[maybe_unused]] constexpr EnumType& operator^=(EnumType& t, EnumType u) noexcept { \
        return t = t ^ u; \
    } \
    [[maybe_unused]] void rs_unicode_bitmask(EnumType); \
    [[maybe_unused]] void rs_unicode_enum(EnumType); \
    [[maybe_unused]] void rs_unicode_format(EnumType);

namespace RS::Unicode::Detail {

    template <typename T>
    concept ExtendedEnum = std::is_enum_v<T>
        && requires(T t) { rs_unicode_enum(t); };

    template <typename T>
    concept ExtendedBitmask = std::is_enum_v<T>
        && requires(T t) { rs_unicode_bitmask(t); };

    template <std::integral T>
    std::vector<std::pair<T, std::string>> make_enum_list(const char* va_args) {

        static constexpr std::string_view whitespace = "\t\n\r ";

        std::string_view va_view = va_args;
        std::vector<std::pair<T, std::string>> list;
        T index = 0;
        auto begin_field = 0uz;

        while (begin_field < va_view.size()) {

            auto comma_pos = va_view.find_first_of(",", begin_field);
            auto field = va_view.substr(begin_field, comma_pos - begin_field);
            auto begin_name = field.find_first_not_of(whitespace);

            if (begin_name != std::string::npos) {

                field = field.substr(begin_name);
                field = field.substr(0, field.find_last_not_of(whitespace) + 1);
                auto eq_pos = field.find('=');

                if (eq_pos != std::string::npos) {

                    auto index_str = field.substr(eq_pos + 1);
                    field = field.substr(0, eq_pos);
                    auto end_field = field.find_last_not_of(whitespace) + 1;

                    if (end_field != 0) {
                        field = field.substr(0, end_field);
                    }

                    if constexpr (std::signed_integral<T>) {
                        index = static_cast<T>(std::strtoll(index_str.data(), nullptr, 0));
                    } else {
                        index = static_cast<T>(std::strtoull(index_str.data(), nullptr, 0));
                    }

                }

                std::string name{field};
                list.push_back({index, name});
                ++index;

            }

            begin_field = comma_pos + 1;

        }

        return list;

    }

    inline std::unordered_map<long long, std::string> signed_enum_table(const char* va_args) {
        auto list = make_enum_list<long long>(va_args);
        std::unordered_map<long long, std::string> table(list.begin(), list.end());
        return table;
    }

    inline std::unordered_map<unsigned long long, std::string> unsigned_enum_table(const char* va_args) {
        auto list = make_enum_list<unsigned long long>(va_args);
        std::unordered_map<unsigned long long, std::string> table(list.begin(), list.end());
        return table;
    }

    inline std::vector<std::pair<unsigned long long, std::string>> linear_enum_table(const char* va_args) {
        return make_enum_list<unsigned long long>(va_args);
    }

}
