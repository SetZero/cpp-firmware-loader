#pragma once

#include<type_traits>
#include <limits>
#include <cmath>
#include <chrono>
#include <ratio>
#include <string_view>
#include <vector>
#include "RatioLookup.h"

namespace utils {
    template<typename T>
    struct periodic_info;

    template<template<typename, typename> typename T, typename Rep, intmax_t Nom, intmax_t Denom>
    struct periodic_info<T<Rep, std::ratio<Nom, Denom>>> {
        using rep = Rep;
        using period = std::ratio<Nom, Denom>;
    };

    template<typename T>
    struct periodic_printable;

    template<template<typename, typename> typename T, typename Rep, intmax_t Nom, intmax_t Denom>
    struct periodic_printable<T<Rep, std::ratio<Nom, Denom>>> {
        static constexpr std::string_view name = T<Rep, std::ratio<Nom, Denom>>::abr_value;
    };

    template<typename Rep, intmax_t Nom, intmax_t Denom>
    struct periodic_printable<std::chrono::duration<Rep, std::ratio<Nom, Denom>>> {
        static constexpr std::string_view name = "s";
    };

   

    template <typename T>
    constexpr std::intmax_t ipow(T num, unsigned int pow)
    {
        return (pow >= sizeof(unsigned int) * 8) ? 0 :
            pow == 0 ? 1 : num * ipow(num, pow - 1);
    }

    template<typename T>
    [[nodiscard]] constexpr auto byteMaxValue(T byte) {
        return std::pow(2, byte * 8);
    }

    template<typename T>
    [[nodiscard]] constexpr auto getBitCount() {
        constexpr std::size_t bitCount = (std::is_same_v<T, std::byte> ? 8 : std::numeric_limits<T>::digits);
        return bitCount;
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr std::array<T, (getBitCount<U>() / getBitCount<T>())> splitNumer(U number) {
        constexpr auto tBitCount = getBitCount<T>();
        constexpr auto uBitCount = getBitCount<U>();
        constexpr auto turns = (uBitCount / tBitCount);
        constexpr auto bitMask = ipow(2, tBitCount) - 1;
        std::array<T, turns> returnVector{};

        std::size_t i = 0;
        std::generate(returnVector.begin(), returnVector.end(), [&]() mutable {
            return T( (number >> (i++ * tBitCount)) & bitMask );
        });
        return returnVector;
    }

    namespace printable {
        template<typename Rep, typename p>
        std::ostream& operator<< (std::ostream& stream, const std::chrono::duration<Rep, p>& duration) {
            using duration_type =  std::chrono::duration<Rep, p>;
            stream << duration.count()
                << utils::ratio_lookup<typename utils::periodic_info<duration_type>::period>::abr_value
                << utils::periodic_printable<duration_type>::name;
            return stream;
        }
    }
}