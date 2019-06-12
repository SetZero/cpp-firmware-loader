//
// Created by sebastian on 07.06.19.
//

#pragma once

#include <type_traits>

namespace serial::utils {
    enum class Parity {
        odd,
        even,
        none,
        unknown
    };

    struct SerialConfiguration {
        unsigned int dataBits;
        Parity parityBit;
        float stopBits;
    };
    static_assert(std::is_standard_layout_v< SerialConfiguration>, "Serial Config needs to be POD Type");

    enum class BinaryFormats {
        IntelHex,
        Unknown
    };
}



