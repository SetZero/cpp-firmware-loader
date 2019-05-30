//
// Created by sebastian on 30.05.19.
//

#pragma once
#include <clara.hpp>
#include "../utils/enum_constants.h"

template<OSTypes OS>
class Parse {
private:
	std::string configLocation;
	std::string deviceName;
public:
    Parse(int argc, const char* argv[]) noexcept {
        auto cli = clara::Opt(configLocation, "config" )
                ["-c"]["--config"]
                        ("Manually specify the config file location");
			| clara::Opt(deviceName, "device")
				["-d"]["--device"]
						("Set the device name")
			| clara::Opt(deviceName, "binary")
				["-b"]["--binary"]
				("Binary File to Flash to the chip")

        auto result = cli.parse( clara::Args( argc, argv ) );
    }cde 
};



