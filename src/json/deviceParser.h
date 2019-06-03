#pragma once
#include <Poco/JSON/Parser.h>
#include <regex>

namespace json = Poco::JSON;

class DeviceParser {
public:
	explicit DeviceParser(const std::string& json);

	template<typename T>
    [[nodiscard]] const T getJSONValue(const std::string& value) {
        T returnValue;
        //auto search = jsonValueMap.find(value);

        //if(search == jsonValueMap.end()) {
            Poco::Dynamic::Var jValue = parsedJSON.extract<json::Object::Ptr>();
            for (auto &str : getPathValue(value)) {
                auto jObj = jValue.extract<json::Object::Ptr>();
                jValue = jObj->get(str);
            }
        //    jsonValueMap.insert({value, jValue.toString()});
            returnValue = jValue;
        //} else {
        //    returnValue = search->second;
        //}
        return returnValue;
    }
private:
    [[nodiscard]]  const std::vector<std::string> getPathValue(const std::string& value);

    Poco::Dynamic::Var parsedJSON;
//    std::unordered_map<std::string, std::string> jsonValueMap;
};