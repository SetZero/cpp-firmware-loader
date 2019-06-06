//
// Created by sebastian on 03.06.19.
//

#pragma once


#include <string>
#include <memory>
#include <string_view>
#include <chrono>
#include <type_traits>
#include "../units/Byte.h"
#include "../utils/SerialUtils.h"
#include "configFinder.h"
#include "deviceParser.h"

using namespace std::literals::string_view_literals;
namespace firmware::json::config {

    enum class JsonOptions {
        deviceID,
        deviceVendor,
        deviceArch,
        deviceSubArch,
        deviceName,
        deviceFlashTotal,
        deviceFlashAvailable,
        deviceEEPROMTotal,
        deviceEEPROMAvailable,
        serialMode,
        serialBytesPerBurst,
        serialMinBaudRate,
        serialMaxBaudRate,
        serialEEPROMBurstDelay,
        serialFlashBurstDelay,
        serialSyncByteAmount,
        serialPreamble,
        serialResyncAfterBurst,
        serialSyncByte,
        binaryFormat
    };

    namespace {

        template<JsonOptions option>
        struct DeviceOptions;

        template<>
        struct DeviceOptions<JsonOptions::deviceID> {
            static constexpr auto jsonKey = "/device/general/id";
            using type = std::string;
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceVendor> {
            static constexpr auto jsonKey = "/device/general/vendor";
            using type = std::string;
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceArch> {
            static constexpr auto jsonKey = "/device/general/arch";
            using type = std::string;
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceSubArch> {
            static constexpr auto jsonKey = "/device/general/subarch";
            using type = std::string;
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceName> {
            static constexpr auto jsonKey = "/device/general/name";
            using type = std::string;
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceFlashTotal> {
            static constexpr auto jsonKey = "/device/flash/total";
            using type = CustomDataTypes::ComputerScience::byte;
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceFlashAvailable> {
            static constexpr auto jsonKey = "/device/flash/available";
            using type = CustomDataTypes::ComputerScience::byte;
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceEEPROMTotal> {
            static constexpr auto jsonKey = "/device/eeprom/total";
            using type = CustomDataTypes::ComputerScience::byte;
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceEEPROMAvailable> {
            static constexpr auto jsonKey = "/device/eeprom/available";
            using type = CustomDataTypes::ComputerScience::byte;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialMode> {
            static constexpr auto jsonKey = "/serial/general/mode";
            using type = serial::utils::SerialConfiguration;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialBytesPerBurst> {
            static constexpr auto jsonKey = "/serial/general/bytesPerBurst";
            using type = std::size_t;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialMinBaudRate> {
            static constexpr auto jsonKey = "/serial/general/minBaudrate";
            using type = std::size_t;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialMaxBaudRate> {
            static constexpr auto jsonKey = "/serial/general/maxBaudrate";
            using type = std::size_t;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialEEPROMBurstDelay> {
            static constexpr auto jsonKey = "/serial/write/eepromBurstDelay";
            using type = std::chrono::nanoseconds;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialFlashBurstDelay> {
            static constexpr auto jsonKey = "/serial/write/flashBurstDelay";
            using type = std::chrono::nanoseconds;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialSyncByteAmount> {
            static constexpr auto jsonKey = "/serial/sync/syncByteAmount";
            using type = std::size_t;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialSyncByte> {
            static constexpr auto jsonKey = "/serial/sync/syncByte";
            using type = std::byte;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialPreamble> {
            static constexpr auto jsonKey = "/serial/sync/preamble";
            using type = std::byte;
        };

        template<>
        struct DeviceOptions<JsonOptions::serialResyncAfterBurst> {
            static constexpr auto jsonKey = "/serial/sync/resyncAfterBurst";
            using type = bool;
        };

        template<>
        struct DeviceOptions<JsonOptions::binaryFormat> {
            static constexpr auto jsonKey = "/binary/format";
            using type = serial::utils::BinaryFormats;
        };
    }

    class ConfigManager {
    public:
        ConfigManager(const std::string &deviceName);

        template<JsonOptions value>
        [[nodiscard]] DeviceOptions<value>::type getJSONValue() const noexcept {
            using optionStruct = DeviceOptions<value>;
            if constexpr (std::is_same_v<typename optionStruct::type, std::string>) {
                return mParser->getJsonAsString(optionStruct::jsonKey);
            }  else {
                return mParser->getJSONValue<typename optionStruct::type>(optionStruct::jsonKey);
            }
        }
    private:
        std::unique_ptr<parser::DeviceParser> mParser;
    };
}
