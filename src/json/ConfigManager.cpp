//
// Created by sebastian on 03.06.19.
//

#include "ConfigManager.h"
namespace firmware::json::config {
    ConfigManager::ConfigManager(const std::string &deviceName) {
        ConfigFinder config{deviceName};
        if (auto &content = config.getFileContents()) {
            if(!content->empty()) {
                mParser.emplace(*content);
            } else {
                mError = "config should not be empty!";
            }
        } else {
            mError = content.error();
        }
    }
    ConfigManager::ConfigManager(const std::filesystem::path& filePath) {
        auto fileContent = utils::readFile(filePath);
        if (fileContent) {
            if(!fileContent->empty()) {
                mParser.emplace(*fileContent);
            } else {
                mError = "config should not be empty!";
            }
        } else {
            mError = fileContent.error();
        }
    }
}