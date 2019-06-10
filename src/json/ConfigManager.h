//
// Created by sebastian on 03.06.19.
//

#pragma once


#include <string>
#include <memory>
#include <string_view>
#include <chrono>
#include <type_traits>
#include <optional>
#include "../units/Byte.h"
#include "configFinder.h"
#include "deviceParser.h"
#include "../utils/SerialUtils.h"
#include "../units/parse/unitParser.h"

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
        binaryFormat,
        unusedFlashByte
    };

    namespace {

        template<JsonOptions option>
        struct DeviceOptions;

        template<>
        struct DeviceOptions<JsonOptions::deviceID> {
            static constexpr auto jsonKey = "/device/general/id";
            using type = std::string;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceVendor> {
            static constexpr auto jsonKey = "/device/general/vendor";
            using type = std::string;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceArch> {
            static constexpr auto jsonKey = "/device/general/arch";
            using type = std::string;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceSubArch> {
            static constexpr auto jsonKey = "/device/general/subarch";
            using type = std::string;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceName> {
            static constexpr auto jsonKey = "/device/general/name";
            using type = std::string;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceFlashTotal> {
            static constexpr auto jsonKey = "/device/flash/total";
            using type = CustomDataTypes::ComputerScience::byte;
            static constexpr auto converter = [](const std::string& input) noexcept { return *CustomDataTypes::parseUnit<type>(input); };
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceFlashAvailable> {
            static constexpr auto jsonKey = "/device/flash/available";
            using type = CustomDataTypes::ComputerScience::byte;
            static constexpr auto converter = [](const std::string& input) noexcept { return *CustomDataTypes::parseUnit<type>(input); };
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceEEPROMTotal> {
            static constexpr auto jsonKey = "/device/eeprom/total";
            using type = CustomDataTypes::ComputerScience::byte;
            //TODO: return byte...
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::deviceEEPROMAvailable> {
            static constexpr auto jsonKey = "/device/eeprom/available";
            using type = CustomDataTypes::ComputerScience::byte;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialMode> {
            static constexpr auto jsonKey = "/serial/general/mode";
            using type = serial::utils::SerialConfiguration;
            static constexpr auto converter = [](const std::string& input) {
                std::regex regex(R"(([0-9]+)(Y|N)([0-9]+.?(?:[0-9]+)?))", std::regex_constants::ECMAScript);
                std::string::const_iterator searchStart(input.cbegin());
                std::smatch matches;
                while (regex_search(searchStart, input.cend(), matches, regex)) {
                    auto dataBit = matches[1];
                    auto parityBit = matches[2];
                    auto stopBit = matches[3];

                    unsigned int data = static_cast<unsigned int>(Poco::NumberParser::parse(dataBit));
                    bool parity = (parityBit == "Y");
                    float stop = Poco::NumberParser::parse(stopBit);
                    return type{data, parity, stop};
                }
            };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialBytesPerBurst> {
            static constexpr auto jsonKey = "/serial/general/bytesPerBurst";
            using type = std::size_t;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialMinBaudRate> {
            static constexpr auto jsonKey = "/serial/general/minBaudrate";
            using type = std::size_t;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialMaxBaudRate> {
            static constexpr auto jsonKey = "/serial/general/maxBaudrate";
            using type = std::size_t;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialEEPROMBurstDelay> {
            static constexpr auto jsonKey = "/serial/write/eepromBurstDelay";
            using type = std::chrono::nanoseconds;
            static constexpr auto converter = [](const std::string& input) noexcept { return  *CustomDataTypes::parseUnit<type>(input);; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialFlashBurstDelay> {
            static constexpr auto jsonKey = "/serial/write/flashBurstDelay";
            using type = std::chrono::nanoseconds;
            static constexpr auto converter = [](const std::string& input) noexcept { return  *CustomDataTypes::parseUnit<type>(input);; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialSyncByteAmount> {
            static constexpr auto jsonKey = "/serial/sync/syncByteAmount";
            using type = std::size_t;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialSyncByte> {
            static constexpr auto jsonKey = "/serial/sync/syncByte";
            using type = std::byte;
            static constexpr const auto converter = [](const std::string& input) noexcept  { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialPreamble> {
            static constexpr auto jsonKey = "/serial/sync/preamble";
            using type = std::byte;
            static constexpr auto converter = [](const std::string& input) noexcept { return input; };
        };

        template<>
        struct DeviceOptions<JsonOptions::serialResyncAfterBurst> {
            static constexpr auto jsonKey = "/serial/sync/resyncAfterBurst";
            using type = bool;
            static constexpr auto converter = [](std::string input) noexcept {
                std::transform(input.begin(), input.end(), input.begin(), ::tolower);
                return (input == "true");
            };
        };

        template<>
        struct DeviceOptions<JsonOptions::binaryFormat> {
            static constexpr auto jsonKey = "/binary/format";
            using type = serial::utils::BinaryFormats;
            static constexpr auto converter = [](std::string input) noexcept {
                std::transform(input.begin(), input.end(), input.begin(), ::tolower);
                if (input == "intel hex") {
                    return type::IntelHex;
                }
                return type::Unknown;
            };
        };

        template<>
        struct DeviceOptions < JsonOptions::unusedFlashByte> {
            static constexpr auto jsonKey = "/binary/unusedFlashByte";
            using type = std::byte;
            static constexpr const auto converter = [](const std::string& input) noexcept { return input; };
        };
    }

    class ConfigManager {
    public:
        explicit ConfigManager(const std::string &deviceName);

        template<JsonOptions value>
#ifdef __cpp_concepts
        requires requires{
           DeviceOptions<value>::jsonKey;
           DeviceOptions<value>::converter;
        }
#endif
        [[nodiscard]] typename DeviceOptions<value>::type getJSONValue() const noexcept {
            using optionStruct = DeviceOptions<value>;
            if constexpr (std::is_same_v<typename optionStruct::type, std::string>) {
                return mParser->getJsonAsString(optionStruct::jsonKey);
            }  else {
                auto tmpValue = optionStruct::converter(mParser->getJsonAsString(optionStruct::jsonKey));
                if constexpr(!std::is_same_v<std::decay_t<decltype(tmpValue)>, optionStruct::type>) {
                    return mParser->getJSONValue<typename optionStruct::type>(optionStruct::jsonKey);
                } else {
                    return tmpValue;
                }
            }
        }
    private:
        std::unique_ptr<parser::DeviceParser> mParser = nullptr;
    };
}
