//
// Created by sebastian on 03.06.19.
//

#include "ConfigManager.h"
namespace firmware::json::config {
    ConfigManager::ConfigManager(const std::string &deviceName) {
        ConfigFinder config{deviceName};
        if (auto &content = config.getFileContents()) {
            mParser = std::make_unique<parser::DeviceParser>(*content);
        } else {
            throw std::runtime_error("Unable to locate Config File!");
        }
    }
}