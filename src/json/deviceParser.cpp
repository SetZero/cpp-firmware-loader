//
// Created by sebastian on 02.06.19.
//

#include "deviceParser.h"

DeviceParser::DeviceParser(const std::string &json) {
    json::Parser parser;
    parsedJSON = parser.parse(json);
}

const std::string DeviceParser::getID() const noexcept {
        auto deviceObj = parsedJSON.extract<json::Object::Ptr>();
        auto device = deviceObj->get("device");

        auto generalObj = device.extract<json::Object::Ptr>();
        auto general = generalObj->get("general");

        auto idObj = general.extract<json::Object::Ptr>();
        auto id = idObj->get("id");

        return id;
    }