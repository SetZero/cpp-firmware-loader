#include <Poco/JSON/Parser.h>

namespace json = Poco::JSON;

class DeviceParser {
public:
	explicit DeviceParser(const std::string& json);

    [[nodiscard]] const std::string getID() const noexcept;
private:
    Poco::Dynamic::Var parsedJSON;
};