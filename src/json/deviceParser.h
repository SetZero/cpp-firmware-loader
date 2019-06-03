#pragma once
#include <Poco/JSON/Parser.h>
#include <regex>
#include <string>

namespace json = Poco::JSON;

namespace parser {
    class DeviceParser {
    public:
        explicit DeviceParser(const std::string& json);

        template<typename T>
    #ifdef __cpp_concepts
        requires std::is_arithmetic_v<T>
    #endif
        const T getJSONValue(const std::string& value) {
            auto tmpValue = getJsonAsString(value);
            return static_cast<T>(Poco::NumberParser::parse(tmpValue));
        }


        [[nodiscard]] const std::string getJsonAsString(const std::string& value);

        [[nodiscard]] std::byte getJSONByteValue(const std::string& value) {
            auto tmpValue = getJsonAsString(value);
            unsigned int tmp;

            if (Poco::NumberParser::tryParseHex(tmpValue, tmp)) {
                return static_cast<std::byte>(tmp);
            }
            else {
                return static_cast<std::byte>(Poco::NumberParser::parse(tmpValue));
            }
        }
    private:
        [[nodiscard]]  const std::vector<std::string> getPathValue(const std::string& value);

        Poco::Dynamic::Var parsedJSON;
        std::unordered_map<std::string, std::string> jsonValueMap;
    };
}