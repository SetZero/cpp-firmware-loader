//
// Created by sebastian on 02.06.19.
//

#include "configFinder.h"

namespace fs = std::filesystem;

ConfigFinder::ConfigFinder(const std::string &deviceName) : fileLocation{findFile(deviceName + CONFIG_SUFFIX, CONFIG_FOLDER)} {}

const utils::expected<const fs::path, const std::string>
ConfigFinder::findFile(const std::string &filename, const std::string &folder) noexcept {

    try {
        for (const auto &entry : fs::directory_iterator(folder)) {
            if(entry.path().filename() == filename) {
                return {entry.path()};
            }
            if(entry.is_directory()) {
                return findFile(filename, entry.path().string());
            }
        }
    } catch(fs::filesystem_error& err) {
        std::stringstream ss;
        ss << "Unable to Open: " << folder;
        return utils::unexpected(ss.str());
    }
    return  utils::make_unexpected("An Unknown Error occured");
}

const utils::expected<const fs::path, const std::string> ConfigFinder::getFileLocation() const noexcept {
    if (fileLocation) {
        return *fileLocation;
    } else {
        return  utils::make_unexpected("File Location is empty! Maybe you didn't load the location?");
    }
}

const utils::expected<const std::string, const std::string> ConfigFinder::getFileContents() const noexcept {
		if (fileLocation) {
            return utils::readFile(*fileLocation);
		}
        std::stringstream ss;
        ss << "Can't load content of an non-existent file! Error: \n" << fileLocation.error();
        return  utils::make_unexpected(ss.str());
}

