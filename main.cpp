
#define DEBUG_BUILD false

#include <iostream>
#include <chrono>
#include <thread>
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
#include "src/units/Byte.h"
#include "src/json/ConfigManager.h"
#include "src/loader/DataSendManager.h"



int main(int argc, const char* argv[]) {
    Parse clParser{argc, argv};
    if(!clParser) {
        std::cout << clParser;
        return 0;
    }
    ConfigManager configManager{clParser.device()};
    DataSendManager sendManager{configManager, clParser.port(), clParser.baud()};
	if (!sendManager.isOpen()) {
		std::cout << *sendManager.errorMessage() << std::endl;
#ifdef _MSC_VER
		while (true) {}
#endif
		return 0;
	} else {
		std::cout << "Connection to " << clParser.port() << " successful!" << std::endl;
		sendManager.bufferedWrite({
				static_cast<std::byte>(0x41),
				static_cast<std::byte>(0x42),
				static_cast<std::byte>(0x43),
				static_cast<std::byte>(0x44),
				static_cast<std::byte>(0x45),
				static_cast<std::byte>(0x46),
				static_cast<std::byte>(0x47),
				static_cast<std::byte>(0x48),
				static_cast<std::byte>(0x49),
				static_cast<std::byte>(0x4a),
				static_cast<std::byte>(0x4b),
				static_cast<std::byte>(0x4c),
				static_cast<std::byte>(0x4d)
			});
		sendManager << static_cast<std::byte>(0x4e) << static_cast<std::byte>(0x4f);
		sendManager.flush();
	}

    using namespace CustomDataTypes::ComputerScience::literals;

    auto mb = 128_kB;

#if DEBUG_BUILD
	std::cout << "====[ DEBUG ] ====" << std::endl;
	std::cout << "System Information" << std::endl;
	std::cout << "OS: " << Poco::Environment::osDisplayName() << std::endl;
	std::cout << "Core Count: " << Poco::Environment::processorCount() << std::endl;

    std::cout << "Waiting for data... " << std::endl;
	for (size_t i = 0; i < 2; i++) {
		std::cout << *serial.reciveByte() << std::endl;
	}
	std::cout << "==================" << std::endl;
#endif

#ifdef _MSC_VER
	while (true) {}
#endif

    return 0;
}