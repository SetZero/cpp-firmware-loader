
#define DEBUG_BUILD true

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

[[nodiscard]] int pgmEnd() {
#if defined(DEBUG_BUILD) && defined(_MSC_VER)
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::hours(std::numeric_limits<int>::max()));
#endif
    return 0;
}

int main(int argc, const char* argv[]) {
    using jsonOpts = firmware::json::config::JsonOptions;

    Parse clParser{argc, argv};
    if(!clParser) {
        std::cout << clParser;
        return pgmEnd();
    }

	using namespace CustomDataTypes::ComputerScience::literals;
    using namespace utils::printable;


    firmware::json::config::ConfigManager configManager{clParser.device()};
    if (!configManager) {
        std::cout << "Error: " << *configManager.errorMessage() << std::endl;
        return pgmEnd();
    }
    std::cout << "Device: " << configManager.getJSONValue<jsonOpts::deviceVendor>()
        << " " << configManager.getJSONValue<jsonOpts::deviceArch>()
        << " [" << configManager.getJSONValue<jsonOpts::deviceSubArch>()
        << "]" << ", " << configManager.getJSONValue<jsonOpts::deviceName>()
        << std::endl;

    if (clParser.baud() < configManager.getJSONValue<jsonOpts::serialMinBaudRate>() &&
        clParser.baud() > configManager.getJSONValue<jsonOpts::serialMaxBaudRate>()) 
    {
        std::cout << "Given Baudrate is not within the allowed min and max\n\r";
        std::cout << "Allowed Minimum: " << configManager.getJSONValue<jsonOpts::serialMinBaudRate>() << "\n\r";
        std::cout << "Allowed Maximum: " << configManager.getJSONValue<jsonOpts::serialMaxBaudRate>() << "\n\r";
        std::cout << "Given Value: " << clParser.baud();
        return pgmEnd();
    }

    firmware::serial::DataSendManager sendManager{configManager, clParser.port(), clParser.baud()};
	if (!sendManager.isOpen()) {
		std::cout << *sendManager.errorMessage() << std::endl;
		return pgmEnd();;
	} else {
        std::cout << "Connection to " << clParser.port() << " successful!" << std::endl;
        if (configManager.getJSONValue<jsonOpts::binaryFormat>() == serial::utils::BinaryFormats::Unknown) {
            std::cout << "Unknown Format!" << std::endl;
            return pgmEnd();
        }
		firmware::reader::HexReader reader{ clParser.binary(), configManager.getJSONValue<jsonOpts::deviceFlashAvailable>() };
		if(!reader) {
            std::cout << *reader.errorMessage();
            return pgmEnd();
		}

		auto maxAvail = configManager.getJSONValue<jsonOpts::deviceFlashAvailable>();
        std::cout << "Used " << reader.getFileSize() << " / " << maxAvail
		        << " (" << (static_cast<long double>(reader.getFileSize()) /
		        static_cast<long double>(static_cast<decltype(reader.getFileSize())>(maxAvail).count())) << "%)" << std::endl;
        std::cout << "Start Address: 0x" << std::hex << reader.getStartAddress() << std::dec << std::endl;
        auto t1 = std::chrono::high_resolution_clock::now();
        sendManager << reader;
		sendManager.flush();
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Transmission took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1) << std::endl;
	}

    return pgmEnd();
}