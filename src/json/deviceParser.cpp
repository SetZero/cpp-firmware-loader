//
// Created by sebastian on 02.06.19.
//

#include "deviceParser.h"

DeviceParser::DeviceParser(const std::string &json) {
    json::Parser parser;
    parsedJSON = parser.parse(json);
}

const std::vector<std::string> DeviceParser::getPathValue(const std::string& value) {
    std::regex regex(R"(\/?([A-Za-z0-9]+)\/?)", std::regex_constants::ECMAScript);
    std::string::const_iterator searchStart( value.cbegin() );
    std::smatch matches;
    std::vector<std::string> result_set;

    while ( regex_search( searchStart, value.cend(), matches, regex ) ) {
        result_set.push_back(matches[1]);
        searchStart = matches.suffix().first;
    }
    return result_set;
}