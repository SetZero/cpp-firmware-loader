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


#define DEBUG_BUILD false

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
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(100)); // preventing odd serial behaviour
																 // It might be possible that this
																 // can be removed later, if hw serial
																 // is disabled ?

    //serial.writeData({(std::byte)0xCC, (std::byte)0xCC, (std::byte)0xCC, (std::byte)0x55,
	//	(std::byte)0x41, (std::byte)0x42
	//	});

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