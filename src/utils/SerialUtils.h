//
// Created by sebastian on 07.06.19.
//

#pragma once


namespace serial::utils {
    struct SerialConfiguration {
        unsigned int dataBits;
        bool parityBit;
        unsigned int stopBit;
    };

    enum class BinaryFormats {
        IntelHex
    };
}



