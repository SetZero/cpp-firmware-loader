#pragma once

#include <ratio>
#include <limits>
#include <stdexcept>
#include <iostream>
#include "../utils/RatioLookup.h"
#include "SIUnit.h"
#include "IECprefix.h"

namespace CustomDataTypes::ComputerScience {

	template<typename Rep, typename Period = std::ratio<1> >
#ifdef __cpp_concepts
	requires Period::den >= 1
#endif
	class Byte : public SIUnit<Byte<Rep, Period>, Rep, Period> {
		using parent_type = SIUnit<Byte<Rep, Period>, Rep, Period>;
	public:
		static constexpr std::string_view abr_value = "B";
        constexpr explicit Byte() noexcept = default;
		constexpr explicit Byte(Rep input) noexcept : parent_type{ input } {}
	};

	//SI-Prefix
	using byte      = Byte<std::intmax_t>;
	using kilobyte  = Byte<std::intmax_t, std::kilo>;
	using megabyte  = Byte<std::intmax_t, std::mega>;
	using gigabyte  = Byte<std::intmax_t, std::giga>;
	using terabyte  = Byte<std::intmax_t, std::tera>;
	using petabyte  = Byte<std::intmax_t, std::peta>;

	//IEC-Prefix
    using kibibyte  = Byte<std::intmax_t, prefix::kibi>;
    using mebibyte  = Byte<std::intmax_t, prefix::mebi>;
    using gibibyte  = Byte<std::intmax_t, prefix::gibi>;
    using tebibyte  = Byte<std::intmax_t, prefix::tebi>;
    using pebibyte  = Byte<std::intmax_t, prefix::pebi>;

	namespace literals {
	    //SI-Prefix
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

		//IEC-Prefix
        constexpr kilobyte operator ""_kiB(unsigned long long element) noexcept {
            return kilobyte(static_cast<std::intmax_t>(element));
        }

        constexpr Byte<long double, prefix::kibi> operator ""_kiB(long double element) noexcept {
            return Byte<long double, prefix::kibi>(element);
        }

        constexpr megabyte operator ""_MiB(unsigned long long element) noexcept {
            return megabyte(static_cast<std::intmax_t>(element));
        }

        constexpr Byte<long double, prefix::mebi> operator ""_MiB(long double element) noexcept {
            return Byte<long double, prefix::mebi>(element);
        }

        constexpr gigabyte operator ""_GiB(unsigned long long element) noexcept {
            return gigabyte(static_cast<std::intmax_t>(element));
        }

        constexpr Byte<long double, prefix::gibi> operator ""_GiB(long double element) noexcept {
            return Byte<long double, prefix::gibi>(element);
        }

        constexpr terabyte operator ""_TiB(unsigned long long element) noexcept {
            return terabyte(static_cast<std::intmax_t>(element));
        }

        constexpr Byte<long double, prefix::tebi> operator ""_TiB(long double element) noexcept {
            return Byte<long double, prefix::tebi>(element);
        }
	}
}

