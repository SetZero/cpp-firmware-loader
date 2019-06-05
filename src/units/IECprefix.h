//
// Created by sebastian on 05.06.19.
//

#pragma once


#include <ratio>

namespace CustomDataTypes::ComputerScience::prefix {
    using kibi = std::ratio<1024, 1>;
    using mebi = std::ratio<1048576, 1>;
    using gibi = std::ratio<1073741824, 1>;
    using tebi = std::ratio<1099511627776, 1>;
    using pebi = std::ratio<1125899906842624, 1>;
}