//
// Created by sebastian on 30.05.19.
//

#pragma once
#include <clara.hpp>
#include "../utils/enum_constants.h"

template<OSTypes OS>
class Parse {
private:
    int baudrate = 0;
public:
    Parse(int argc, const char* argv[]) noexcept {
        auto cli = clara::Opt( baudrate, "baudrate" )
                ["-b"]["--baudrate"]
                        ("What baudrate should be used to communicate?");
        auto result = cli.parse( clara::Args( argc, argv ) );
    }
};



