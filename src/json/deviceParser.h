#pragma once
#include <Poco/JSON/Parser.h>
#include <regex>
#include <string>
#include <type_traits>
#include "../utils/expected.h"

namespace json = Poco::JSON;

namespace parser {
    class DeviceParser {
    public:
        explicit DeviceParser(const std::string& json);

        template<typename T>
    #ifdef __cpp_concepts
        requires std::is_arithmetic_v<T> || std::is_same_v<T, std::byte>
    #endif
        const utils::expected<T, std::string> getJSONValue(const std::string& value) {
            auto tmpValue = getJsonAsString(value);
            if(!tmpValue) {
                return utils::make_unexpected(tmpValue.error());
            }


            unsigned int tmp;

            if (tmpValue->rfind("0x", 0) == 0 && Poco::NumberParser::tryParseHex(*tmpValue, tmp)) {
                return static_cast<T>(tmp);
            }
            else {
                return static_cast<T>(Poco::NumberParser::parse(*tmpValue));
            }
            //auto tmpValue = getJsonAsString(value);
            //return T{Poco::NumberParser::parse(tmpValue)};
        }

        [[nodiscard]] const utils::expected<std::string, std::string> getJsonAsString(const std::string& value);

        [[nodiscard]] utils::expected<std::byte, std::string> getJSONByteValue(const std::string& value);
    private:
        [[nodiscard]]  const std::vector<std::string> getPathValue(const std::string& value);

        Poco::Dynamic::Var parsedJSON;
        std::unordered_map<std::string, std::string> jsonValueMap;
    };

}