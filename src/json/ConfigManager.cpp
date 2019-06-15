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
            mError = content.error();
        }
    }
    ConfigManager::ConfigManager(const std::filesystem::path& filePath) {
        auto fileContent = utils::readFile(filePath);
        if (fileContent) {
            mParser = std::make_unique<parser::DeviceParser>(*fileContent);
        } else {
            mError = fileContent.error();
        }
    }
}