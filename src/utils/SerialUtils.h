//
// Created by sebastian on 07.06.19.
//

#pragma once


namespace serial::utils {
    struct SerialConfiguration {
        unsigned int dataBits;
        bool parityBit;
        float stopBits;
    };

    enum class BinaryFormats {
        IntelHex,
        Unknown
    };
}



