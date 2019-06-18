//
// Created by sebastian on 17.06.19.
//

#include <catch2/catch.hpp>
#include "testClasses/SerialTestImpl.h"
#include "../src/loader/DataSendManager.h"

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
      "bytesPerBurst": 2,
      "metadataByteSize": 2,
      "minBaudrate": 9600,
      "maxBaudrate": 57600
    },
    "write": {
      "waitTimeForReset":  "1s",
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

    [[nodiscard]] std::filesystem::path pathSetup() {
        auto path = std::filesystem::path{std::filesystem::temp_directory_path()};
        path /= "FiremwareLoaderTests/ConfigManager2.json";
        std::filesystem::create_directories(path.parent_path());

        {
            std::ofstream stream{path};
            stream << jsonString;
            stream.close();
        }

        return path;
    }

    TEST_CASE("General Serial Test", "[General Serial Test]") {
        std::unique_ptr<AbstractSerial> serialImplPtr = std::make_unique<SerialTestImpl>("/dev/null", 9600,
                                                                                         serial::utils::SerialConfiguration{
                                                                                                 8,
                                                                                                 serial::utils::Parity::none,
                                                                                                 1});

        auto path = pathSetup();
        firmware::json::config::ConfigManager manager{path};
        const auto& val = dynamic_cast<SerialTestImpl*>(serialImplPtr.get())->getVectorContents();

        auto sendManager = firmware::serial::DataSendManager{manager, std::move(serialImplPtr), false};
        sendManager.bufferedWrite({std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}});
        //sendManager.bufferedWrite(std::byte{0});

        REQUIRE(val.at(0) == std::byte{0xCC});
        REQUIRE(val.at(1) == std::byte{0xCC});
        REQUIRE(val.at(2) == std::byte{0xCC});
        REQUIRE(val.at(3) == std::byte{0x55});
        REQUIRE(val.at(4) == std::byte{0});
        REQUIRE(val.at(5) == std::byte{1});
        REQUIRE(val.at(6) == std::byte{0xCC});
        REQUIRE(val.at(7) == std::byte{0xCC});
        REQUIRE(val.at(8) == std::byte{0xCC});
        REQUIRE(val.at(9) == std::byte{0x55});
        REQUIRE(val.at(10) == std::byte{2});
        REQUIRE(val.at(11) == std::byte{3});
        std::filesystem::remove_all(path);
    }

    TEST_CASE("Serial Half Transmission Test", "[Serial Test]") {
        std::unique_ptr<AbstractSerial> serialImplPtr = std::make_unique<SerialTestImpl>("/dev/null", 9600,
                                                                                         serial::utils::SerialConfiguration{
                                                                                                 8,
                                                                                                 serial::utils::Parity::none,
                                                                                                 1});

        auto path = pathSetup();
        firmware::json::config::ConfigManager manager{path};
        const auto& val = dynamic_cast<SerialTestImpl*>(serialImplPtr.get())->getVectorContents();

        auto sendManager = firmware::serial::DataSendManager{manager, std::move(serialImplPtr), false};
        sendManager.bufferedWrite({std::byte{0}, std::byte{1}, std::byte{2}});
        //sendManager.bufferedWrite(std::byte{0});

        REQUIRE(val.at(0) == std::byte{0xCC});
        REQUIRE(val.at(1) == std::byte{0xCC});
        REQUIRE(val.at(2) == std::byte{0xCC});
        REQUIRE(val.at(3) == std::byte{0x55});
        REQUIRE(val.at(4) == std::byte{0});
        REQUIRE(val.at(5) == std::byte{1});
        std::filesystem::remove_all(path);
    }

}