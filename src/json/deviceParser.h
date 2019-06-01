#include <Poco/JSON/Parser.h>

namespace json = Poco::JSON;

class DeviceParser {
public:
	explicit DeviceParser(const std::string& json) {
        json::Parser parser;
        parsedJSON = parser.parse(json);
    }

    [[nodiscard]] const std::string getID() const noexcept {
        auto deviceObj = parsedJSON.extract<json::Object::Ptr>();
        auto device = deviceObj->get("device");

        auto generalObj = device.extract<json::Object::Ptr>();
        auto general = generalObj->get("general");

        auto idObj = general.extract<json::Object::Ptr>();
        auto id = idObj->get("id");

        return id;
	}
private:
    Poco::Dynamic::Var parsedJSON;
};