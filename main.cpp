
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
#include "src/utils/utils.h"


int main(int argc, const char* argv[]) {
    using jsonOpts = firmware::json::config::JsonOptions;

    Parse clParser{argc, argv};
    if(!clParser) {
        std::cout << clParser;
        return 0;
    }

	using namespace CustomDataTypes::ComputerScience::literals;
    using namespace utils::printable;

    firmware::json::config::ConfigManager configManager{clParser.device()};
    std::cout << "Device: " << configManager.getJSONValue<jsonOpts::deviceVendor>()
        << " " << configManager.getJSONValue<jsonOpts::deviceArch>()
        << " [" << configManager.getJSONValue<jsonOpts::deviceSubArch>()
        << "]" << ", " << configManager.getJSONValue<jsonOpts::deviceName>()
        << std::endl;

    if (clParser.baud() < configManager.getJSONValue<jsonOpts::serialMinBaudRate>() &&
        clParser.baud() > configManager.getJSONValue<jsonOpts::serialMaxBaudRate>()) 
    {
        std::cout << "Given Baudrate is not within the allowed min and/or max\n\r";
        std::cout << "Allowed Minimum: " << configManager.getJSONValue<jsonOpts::serialMinBaudRate>() << "\n\r";
        std::cout << "Allowed Maximum: " << configManager.getJSONValue<jsonOpts::serialMaxBaudRate>() << "\n\r";
        std::cout << "Given Value: " << clParser.baud();
        return 0;
    }

    firmware::serial::DataSendManager sendManager{configManager, clParser.port(), clParser.baud()};
	if (!sendManager.isOpen()) {
		std::cout << *sendManager.errorMessage() << std::endl;
		return 0;
	} else {
        std::cout << "Connection to " << clParser.port() << " successful!" << std::endl;
        if (configManager.getJSONValue<jsonOpts::binaryFormat>() == serial::utils::BinaryFormats::Unknown) {
            std::cout << "Unknown Format!" << std::endl;
            return 0;
        }
		firmware::reader::HexReader reader{ clParser.binary(), configManager.getJSONValue<jsonOpts::deviceFlashAvailable>() };
		if(!reader) {
            std::cout << *reader.errorMessage();
            return 0;
		}

		auto maxAvail = configManager.getJSONValue<jsonOpts::deviceFlashAvailable>();
        std::cout << "Used " << reader.getFileSize() << " / " << maxAvail
		        << " (" << (static_cast<long double>(reader.getFileSize()) /
		        static_cast<long double>(static_cast<decltype(reader.getFileSize())>(maxAvail).count())) << "%)" << std::endl;
        sendManager << reader;
		sendManager.flush();
        std::cout << "Finished Transmission" << std::endl;
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