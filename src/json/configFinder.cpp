//
// Created by sebastian on 02.06.19.
//

#include "configFinder.h"

ConfigFinder::ConfigFinder(const std::string &deviceName) : fileLocation{findFile(deviceName + CONFIG_SUFFIX, CONFIG_FOLDER)} {}

const std::optional<const fs::path>
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
        std::cerr << "Unable to Open: " << folder << std::endl;
        return std::nullopt;
    }
    return std::nullopt;
}

const std::optional<const fs::path> ConfigFinder::getFileLocation() const noexcept {
    return fileLocation;
}

const std::optional<const std::string> ConfigFinder::getFileContents() const noexcept {
        try {
			if (fileLocation) {
				std::ifstream file{ fileLocation->string() };
				std::stringstream buffer;
				buffer << file.rdbuf();
				return buffer.str();
			}
			return std::nullopt;
        } catch (std::ios_base::failure& fail) {}
        return std::nullopt;
    }

