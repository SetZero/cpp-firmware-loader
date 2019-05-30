#include <Poco/JSON/Parser.h>

class DeviceParser {
public:
	constexpr DeviceParser(std::string json) {
		Parser parser;
		auto result = parser.parse(json);
	}
private:
};