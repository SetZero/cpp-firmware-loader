//
// Created by sebastian on 30.05.19.
//

#pragma once
#include <clara.hpp>
#include "../utils/enum_constants.h"

class Parse {
private:
	std::string configLocation;
	std::string deviceName;
    std::string comPortLocation;
    std::string binaryLocation;
    int baudrate = 9600;
    bool showHelp = false;
    clara::Parser cli;
public:
    Parse(int argc, const char* argv[]) noexcept {
        cli = clara::Help( showHelp )
                   | clara::Opt(configLocation, "config" )
                   ["-c"]["--config"]
                           ("Manually specify the config file location")
                   | clara::Opt(deviceName, "device")
                   ["-d"]["--device"]
                           ("Set the device name (mandatory)")
                   | clara::Opt(binaryLocation, "binary")
                   ["-f"]["--file"]
                           ("Binary File to Flash to the chip (mandatory)")
                   | clara::Opt(comPortLocation, "port")
                   ["-p"]["--port"]
                           ("Specify the port which is connected to the device (mandatory)")
                   | clara::Opt(baudrate, "baud")
                   ["-b"]["--baud"]
                           ("Baudrate for communication with the chip (default: " + std::to_string(baudrate) + ")");

        auto result = cli.parse( clara::Args( argc, argv ) );
        if(!result) {
            std::cout << result.errorMessage();
            showHelp = true;
        } else {
            if(binaryLocation.empty() || comPortLocation.empty() || deviceName.empty()) {
                showHelp = true;
            }
        }
    }

    [[nodiscard]] std::string port() const noexcept {
        return comPortLocation;
    }

    [[nodiscard]] std::string binary() const noexcept {
        return binaryLocation;
    }

    [[nodiscard]] std::string device() const noexcept {
        return deviceName;
    }

    [[nodiscard]] bool help() const noexcept {
        return showHelp;
    }

    [[nodiscard]] bool baud() const noexcept {
        return baudrate;
    }

    explicit operator bool() const {
        return !showHelp;
    }

    friend auto operator<<( std::ostream &os, Parse const &parse ) -> std::ostream& {
        if(parse.showHelp) {
            os << parse.cli;
        }
        return os;
    }
};



