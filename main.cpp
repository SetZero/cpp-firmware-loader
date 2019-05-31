#include <iostream>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/Environment.h>
#include <catch.hpp>
#include <clara.hpp>
#include <asio.hpp>

#include "src/commandline/parse.h"
#include "src/utils/enum_constants.h"
#include "src/json//deviceParser.h"
#include "src/json/configFinder.h"
#include "src/serial/Serial.h"

int main(int argc, const char* argv[]) {
    Parse clParser{argc, argv};
    if(!clParser) {
        std::cout << clParser;
        return 0;
    }

    std::cout << clParser.port() << std::endl;
    Serial<SerialMode::Duplex> serial{clParser.port(), clParser.baud()};
    serial.writeData({(std::byte)0xCC, (std::byte)0xCC, (std::byte)0xCC, (std::byte)0x55, (std::byte)0x41, (std::byte)0x42, (std::byte)0x43, (std::byte)0x44});
    std::cout << "Waiting for data... " << std::endl;
    std::cout << serial.reciveByte() << std::endl;

    Poco::JSON::ParseHandler handler;

    ConfigFinder config{clParser.device()};
    if(auto& location = config.getFileLocation()) {
        std::cout << "File Location: " << location->string() << std::endl;
    }
    if(auto& content = config.getFileContents()) {
        DeviceParser parser{*content};
        std::cout << "ID: " << parser.getID() << std::endl;
    }

    std::cout << Poco::Environment::osDisplayName() << std::endl;
    std::cout << Poco::Environment::processorCount() << std::endl;
    return 0;
}