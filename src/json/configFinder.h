//
// Created by sebastian on 31.05.19.
//

#pragma once
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;

class ConfigFinder {
private:
    static constexpr auto CONFIG_FOLDER = "config/";
    static constexpr auto CONFIG_SUFFIX = ".json";

    std::optional<fs::path> fileLocation;
public:
    ConfigFinder(const std::string& deviceName) {
        fileLocation = findFile(deviceName + CONFIG_SUFFIX, CONFIG_FOLDER);
    }
    [[nodiscard]] std::optional<const fs::path> findFile(const std::string& filename, const std::string& folder) noexcept {

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

    [[nodiscard]] const std::optional<const fs::path> getFileLocation() const noexcept {
        return fileLocation;
    }

    [[nodiscard]] const std::optional<const std::string> getFileContents() const noexcept {
        try {
            std::ifstream file{fileLocation->string()};
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        } catch (std::ios_base::failure& fail) {}
        return std::nullopt;
    }
};



