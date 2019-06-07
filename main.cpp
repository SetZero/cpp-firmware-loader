
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
#include "src/loader/HexReader.h"


int main(int argc, const char* argv[]) {
    Parse clParser{argc, argv};
    if(!clParser) {
        std::cout << clParser;
        return 0;
    }

	using namespace CustomDataTypes::ComputerScience::literals;

    firmware::json::config::ConfigManager configManager{clParser.device()};
    std::cout << "Device: " << configManager.getJSONValue<firmware::json::config::JsonOptions::deviceVendor>()
        << " " << configManager.getJSONValue<firmware::json::config::JsonOptions::deviceArch>()
        << " [" << configManager.getJSONValue<firmware::json::config::JsonOptions::deviceSubArch>()
        << "]" << ", " << configManager.getJSONValue<firmware::json::config::JsonOptions::deviceName>()
        << std::endl;
    firmware::serial::DataSendManager sendManager{configManager, clParser.port(), clParser.baud()};
	if (!sendManager.isOpen()) {
		std::cout << *sendManager.errorMessage() << std::endl;
#ifdef _MSC_VER
		while (true) {}
#endif
		return 0;
	} else {
        std::cout << "Connection to " << clParser.port() << " successful!" << std::endl;
		firmware::utils::HexReader reader{ clParser.binary(), 32_kB };
		if(!reader) {
            std::cout << *reader.errorMessage();
		}

		reader.writeToStream(sendManager);
		sendManager.bufferedWrite({
				std::byte{0x41},
				std::byte{0x42},
				std::byte{0x43},
				std::byte{0x44},
				std::byte{0x45},
				std::byte{0x46},
				std::byte{0x47},
				std::byte{0x48},
				std::byte{0x49},
				std::byte{0x4a},
				std::byte{0x4b},
				std::byte{0x4c},
				std::byte{0x4d}
		});
		sendManager << std::byte{ 0x4e } << std::byte{ 0x4f };
		sendManager.flush();
	}

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