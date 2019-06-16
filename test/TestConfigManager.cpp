#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include "../src/json/ConfigManager.h"

const std::string jsonString = R"({
  "device": {
    "general": {
      "id":   "atmega328p",
      "vendor": "Microchip",
      "arch": "AVR",
      "subarch": "ATMega",
      "name": "Atmega328p"
    },
    "flash": {
      "total": "32KB",
      "available": "30KB"
    },
    "eeprom": {
      "total": "1KB",
      "available": "1023B"
    }
  },
  "serial": {
    "general": {
      "mode": "8N1",
      "bytesPerBurst": 16,
      "minBaudrate": 9600,
      "maxBaudrate": 57600
    },
    "write": {
      "eepromBurstDelay": "100ms",
      "flashBurstDelay": "9ms"
    },
    "sync": {
      "syncByteAmount": 3,
      "syncByte": "0xCC",
      "preamble": "0x55",
      "resyncAfterBurst": "true"
    }
  },
  "binary": {
    "format": "Intel Hex",
    "unusedFlashByte":  "0xFF"
  }
})";

namespace test {
    TEST_CASE("Test Config Manager Constructor", "[Test Constructor]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "FiremwareLoaderTests/ConfigManager.json";
        std::filesystem::create_directories(path.parent_path());

        {
            std::ofstream stream{path};
            stream << jsonString;
            stream.close();
        }

        firmware::json::config::ConfigManager manager{path};
        SECTION("Init") {
            REQUIRE(!static_cast<bool>(manager.errorMessage()));
            REQUIRE(static_cast<bool>(manager));
        }

        using namespace CustomDataTypes::ComputerScience::literals;
        using namespace std::chrono_literals;
        SECTION("All values") {
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceID>() == "atmega328p");
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceVendor>() == "Microchip");
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceArch>() == "AVR");
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceSubArch>() == "ATMega");
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceName>() == "Atmega328p");
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceFlashTotal>() == 32_kB);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceFlashAvailable>() == 30_kB);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceEEPROMTotal>() == 1_kB);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::deviceEEPROMAvailable>() == 1023_B);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialMode>().dataBits == 8);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialMode>().parityBit == serial::utils::Parity::none);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialMode>().stopBits == 1);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialBytesPerBurst>() == 16);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialMinBaudRate>() == 9600);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialMaxBaudRate>() == 57600);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialEEPROMBurstDelay>() == 100ms);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialFlashBurstDelay>() == 9ms);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialSyncByteAmount>() == 3);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialSyncByte>() == std::byte{0xCC});
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialPreamble>() == std::byte{0x55});
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::serialResyncAfterBurst>());
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::binaryFormat>() == serial::utils::BinaryFormats::IntelHex);
            REQUIRE(manager.getJSONValue<firmware::json::config::JsonOptions::unusedFlashByte>() == std::byte{0xFF});
        }
    }

    TEST_CASE("Test Nonexistent File", "[Nonexistent file Test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "FiremwareLoaderTests/NaNaNaNaNBatman.txt";

        firmware::json::config::ConfigManager manager{path};
        REQUIRE(!static_cast<bool>(manager));
    }

    TEST_CASE("Test empty File", "[empty file Test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "FiremwareLoaderTests/empty.txt";
        std::ofstream output{path};

        firmware::json::config::ConfigManager manager{path};
        REQUIRE(!static_cast<bool>(manager));
    }

    TEST_CASE("Test empty JSON", "[empty json Test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "FiremwareLoaderTests/empty.json";
        std::filesystem::create_directories(path.parent_path());
        {
            std::ofstream stream{path};
            stream << "{}";
            stream.close();
        }
        firmware::json::config::ConfigManager manager{path};
        REQUIRE(static_cast<bool>(manager));
#ifdef __cpp_exceptions
        REQUIRE_THROWS_AS(manager.getJSONValue<firmware::json::config::JsonOptions::deviceID>(), std::runtime_error);
#endif
    }
}