//
// Created by sebastian on 31.05.19.
//

#pragma once
#include <filesystem>
#include <fstream>
#include <optional>
#include <iostream>


namespace fs = std::filesystem;

class ConfigFinder {
private:
    static constexpr auto CONFIG_FOLDER = "config/";
    static constexpr auto CONFIG_SUFFIX = ".json";

    std::optional<fs::path> fileLocation;
public:
    explicit ConfigFinder(const std::string& deviceName);

    [[nodiscard]] const std::optional<const fs::path> findFile(const std::string& filename, const std::string& folder) noexcept;

    [[nodiscard]] const std::optional<const fs::path> getFileLocation() const noexcept;

    [[nodiscard]] const std::optional<const std::string> getFileContents() const noexcept;
};



