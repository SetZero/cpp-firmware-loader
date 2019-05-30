//
// Created by sebastian on 30.05.19.
//

#pragma once


#include "enum_constants.h"

[[nodiscard]] constexpr auto getOS() noexcept -> OSTypes {
#ifdef _WIN32 || _WIN64
    return OSTypes::Windows;
#elif __unix || __unix__
    return OSTypes::Linux;
#elif __APPLE__ || __MACH__
    return OSTypes::MacOS;
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #else
    return "Other";
#endif
}


