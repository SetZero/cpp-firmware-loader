#pragma once

#include<type_traits>
#include <limits>
#include <cmath>
#include <chrono>
#include <ratio>
#include <string_view>
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