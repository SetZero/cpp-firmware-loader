//
// Created by sebastian on 05.06.19.
//

#pragma once


#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "../../includes/intelhexclass.h"
#include "../json/ConfigManager.h"
#include "../units/Byte.h"

class HexReader {
public:
    using byte = CustomDataTypes::ComputerScience::byte;
    HexReader(const std::string& fileLocation, const byte& maxSize);
private:
    intelhex hex;
};



