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

#define DEBUG_BUILD true

int main(int argc, const char* argv[]) {
    Parse clParser{argc, argv};
    if(!clParser) {
        std::cout << clParser;
        return 0;
    }

    Serial<SerialMode::Duplex> serial{clParser.port(), clParser.baud()};

    serial.writeData({(std::byte)0xCC, (std::byte)0xCC, (std::byte)0xCC, (std::byte)0x55,
		(std::byte)0x41, (std::byte)0x42, (std::byte)0x43, (std::byte)0x44,
		(std::byte)0x45, (std::byte)0x46, (std::byte)0x47, (std::byte)0x48,
		});

#ifdef DEBUG_BUILD
	std::cout << "====[ DEBUG ] ====" << std::endl;
	std::cout << "System Information" << std::endl;
	std::cout << "OS: " << Poco::Environment::osDisplayName() << std::endl;
	std::cout << "Core Count: " << Poco::Environment::processorCount() << std::endl;

    std::cout << "Waiting for data... " << std::endl;
	for (size_t i = 0; i < 8; i++) {
		std::cout << serial.reciveByte() << std::endl;
	}

	std::cout << "==================" << std::endl;
#endif

    Poco::JSON::ParseHandler handler;

    ConfigFinder config{clParser.device()};
    if(auto& location = config.getFileLocation()) {
        std::cout << "File Location: " << location->string() << std::endl;
    }
    if(auto& content = config.getFileContents()) {
        DeviceParser parser{*content};
        std::cout << "ID: " << parser.getID() << std::endl;
    }


	while(true) {}
    return 0;
}