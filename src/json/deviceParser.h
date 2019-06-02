#include <Poco/JSON/Parser.h>
#include <regex>

namespace json = Poco::JSON;

class DeviceParser {
public:
	explicit DeviceParser(const std::string& json);

    [[nodiscard]]  const std::string getJSONValue(const std::string& value);
private:
    [[nodiscard]]  const std::vector<std::string> getPathValue(const std::string& value);

    Poco::Dynamic::Var parsedJSON;
    std::unordered_map<std::string, std::string> jsonValueMap;
};