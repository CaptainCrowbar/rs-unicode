#pragma once

#include <type_traits>

#define RS_UNICODE_DEFINE_BITMASK_OPERATORS(T) \
    [[maybe_unused]] constexpr bool operator!(T t) noexcept { \
        return ! static_cast<bool>(t); \
    } \
    [[maybe_unused]] constexpr T operator~(T t) noexcept { \
        using U = std::make_unsigned_t<T>; \
        return static_cast<T>(~ static_cast<U>(t)); \
    } \
    [[maybe_unused]] constexpr T operator&(T t, T u) noexcept { \
        using U = std::make_unsigned_t<T>; \
        return static_cast<T>(static_cast<U>(t) & static_cast<U>(u)); \
    } \
    [[maybe_unused]] constexpr T operator|(T t, T u) noexcept { \
        using U = std::make_unsigned_t<T>; \
        return static_cast<T>(static_cast<U>(t) | static_cast<U>(u)); \
    } \
    [[maybe_unused]] constexpr T operator^(T t, T u) noexcept { \
        using U = std::make_unsigned_t<T>; \
        return static_cast<T>(static_cast<U>(t) ^ static_cast<U>(u)); \
    } \
    [[maybe_unused]] constexpr T& operator&=(T& t, T u) noexcept { \
        return t = t & u; \
    } \
    [[maybe_unused]] constexpr T& operator|=(T& t, T u) noexcept { \
        return t = t | u; \
    } \
    [[maybe_unused]] constexpr T& operator^=(T& t, T u) noexcept { \
        return t = t ^ u; \
    }
