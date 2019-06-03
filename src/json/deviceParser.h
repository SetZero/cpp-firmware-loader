#pragma once
#include <Poco/JSON/Parser.h>
#include <regex>
#include <string>

namespace json = Poco::JSON;

class DeviceParser {
public:
	explicit DeviceParser(const std::string& json);

	template<typename T>
#ifdef __cpp_concepts
	requires std::is_arithmetic_v<T>
#endif
	[[nodiscard]] const T getJSONValue(const std::string& value) {
		auto tmpValue = getJsonAsString(value);
		return Poco::NumberParser::parse(tmpValue);
	}

	template<>
	[[nodiscard]] const std::string getJSONValue<std::string>(const std::string& value) {
		return getJsonAsString(value);
	}

	template<>
	[[nodiscard]] const std::byte getJSONValue<std::byte>(const std::string& value) {
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
	[[nodiscard]] const std::string getJsonAsString(const std::string& value);

    Poco::Dynamic::Var parsedJSON;
    std::unordered_map<std::string, std::string> jsonValueMap;
};