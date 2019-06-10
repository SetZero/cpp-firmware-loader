//
// Created by sebastian on 07.06.19.
//

#pragma once

#include <type_traits>

namespace serial::utils {
    struct SerialConfiguration {
        unsigned int dataBits;
        bool parityBit;
        float stopBits;
    };
    static_assert(std::is_standard_layout_v< SerialConfiguration>, "Serial Config needs to be POD Type");

    enum class BinaryFormats {
        IntelHex,
        Unknown
    };
}



