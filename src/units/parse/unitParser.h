#pragma once

#include <string>
#include <regex>
#include <Poco/NumberParser.h>
#include <iostream>
#include "../../utils/RatioLookup.h"
#include "../..//utils/utils.h"

namespace CustomDataTypes {
    template<typename T>
    std::optional<T> parseUnit(const std::string& value) {
        static std::regex unit_regex(
            R"(([-+]?[0-9]*\.?[0-9]+)\s?(a|f|p|n|u|m|c|d|D|h|k|K|M|T|P|E)?([A-Z0-9a-z!"#$%&'()*+,.\/:;<=>?@\[\] ^_`{|}~-]+)?)",
            std::regex_constants::ECMAScript);
        std::smatch unit_match;
        long double unit_val = 0.0;
        std::pair<std::intmax_t, std::intmax_t> prefix;
        std::optional<T> returnValue = std::nullopt;
        std::string unit;
        if (std::regex_search(value, unit_match, unit_regex)) {
            for (size_t i = 1; i < unit_match.size(); ++i) {
                switch (i) {
                case 1:
                    if (unit_match[i].length() > 0) {
                        unit_val = Poco::NumberParser::parse(unit_match[i].str().data());
                    }
                    break;
                case 2:
                    if (unit_match[i].length() > 0) {
                        prefix = utils::getRatio(unit_match[i].str().at(0));
                    }
                    break;
                case 3:
                    if (unit_match[i].length() > 0) {
                        unit = unit_match[i].str();
                        if (unit == utils::periodic_printable<T>::name) {
                            using period_type = utils::periodic_info<T>::period;
                            returnValue = T{ static_cast<utils::periodic_info<T>::rep>((unit_val * prefix.first * period_type::den) / (prefix.second * period_type::num)) };
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }

        return returnValue;
    }

}