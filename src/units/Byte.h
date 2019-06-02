#pragma once

#include <ratio>
#include <limits>
#include <stdexcept>
#include <iostream>
#include "../utils/RatioLookup.h"
#include "SIUnit.h"

namespace CustomDataTypes::ComputerScience {

	template<typename Rep, typename Period = std::ratio<1> >
#ifdef __cpp_concepts
	requires Period::den >= 1
#endif
	class Byte : public SIUnit<Byte<Rep, Period>, Rep, Period> {
		using parent_type = SIUnit<Byte<Rep, Period>, Rep, Period>;
	public:
		static constexpr std::string_view abr_value = "B";
		constexpr explicit Byte(Rep input) noexcept : parent_type{ input } {}
	};

	using byte      = Byte<std::intmax_t>;
	using kilobyte  = Byte<std::intmax_t, std::kilo>;
	using megabyte  = Byte<std::intmax_t, std::mega>;
	using gigabyte  = Byte<std::intmax_t, std::giga>;
	using terabyte  = Byte<std::intmax_t, std::tera>;
	using petabyte  = Byte<std::intmax_t, std::peta>;

	namespace literals {
		constexpr byte operator ""_B(unsigned long long element) noexcept {
			return byte(static_cast<std::intmax_t>(element));
		}

		constexpr Byte<long double> operator ""_B(long double element) noexcept {
			return Byte<long double>(element);
		}

		constexpr kilobyte operator ""_kB(unsigned long long element) noexcept {
			return kilobyte(static_cast<std::intmax_t>(element));
		}

		constexpr Byte<long double, std::kilo> operator ""_kB(long double element) noexcept {
			return Byte<long double, std::kilo>(element);
		}

		constexpr megabyte operator ""_MB(unsigned long long element) noexcept {
			return megabyte(static_cast<std::intmax_t>(element));
		}

		constexpr Byte<long double, std::mega> operator ""_MB(long double element) noexcept {
			return Byte<long double, std::mega>(element);
		}

		constexpr gigabyte operator ""_GB(unsigned long long element) noexcept {
			return gigabyte(static_cast<std::intmax_t>(element));
		}

		constexpr Byte<long double, std::giga> operator ""_GB(long double element) noexcept {
			return Byte<long double, std::giga>(element);
		}

		constexpr terabyte operator ""_TB(unsigned long long element) noexcept {
			return terabyte(static_cast<std::intmax_t>(element));
		}

		constexpr Byte<long double, std::tera> operator ""_TB(long double element) noexcept {
			return Byte<long double, std::tera>(element);
		}
	}
}

