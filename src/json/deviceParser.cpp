//
// Created by sebastian on 02.06.19.
//

#include "deviceParser.h"
#include "../utils/expected.h"

namespace parser {
    DeviceParser::DeviceParser(const std::string &json) {
        json::Parser parser;
        parsedJSON = parser.parse(json);
    }

    const std::vector<std::string> DeviceParser::getPathValue(const std::string &value) {
        std::regex regex(R"(\/?([A-Za-z0-9]+)\/?)", std::regex_constants::ECMAScript);
        std::string::const_iterator searchStart(value.cbegin());
        std::smatch matches;
        std::vector<std::string> result_set;

        while (regex_search(searchStart, value.cend(), matches, regex)) {
            result_set.push_back(matches[1]);
            searchStart = matches.suffix().first;
        }
        return result_set;
    }

    const utils::expected<std::string, std::string> DeviceParser::getJsonAsString(const std::string &value) {
        try {
            std::string returnValue;
            auto search = jsonValueMap.find(value);

            if (search == jsonValueMap.end()) {
                Poco::Dynamic::Var jValue = parsedJSON.extract<json::Object::Ptr>();
                for (auto &str : getPathValue(value)) {
                    auto jObj = jValue.extract<json::Object::Ptr>();
                    jValue = jObj->get(str);
                }
                jsonValueMap.insert(std::begin(jsonValueMap), {value, jValue.toString()});
                returnValue = jValue.toString();
            } else {
                returnValue = search->second;
            }
            return returnValue;
        } catch (Poco::InvalidAccessException& e) {
            return utils::make_unexpected(e.what());
        }
    }

    utils::expected<std::byte, std::string> DeviceParser::getJSONByteValue(const std::string &value) {
        auto tmpValue = getJsonAsString(value);
        if(!tmpValue) {
            return utils::make_unexpected(tmpValue.error());
        }
        unsigned int tmp;

        if (tmpValue->rfind("0x", 0) == 0 && Poco::NumberParser::tryParseHex(*tmpValue, tmp)) {
            return static_cast<std::byte>(tmp);
        }
        else {
            return static_cast<std::byte>(Poco::NumberParser::parse(*tmpValue));
        }
    }
}