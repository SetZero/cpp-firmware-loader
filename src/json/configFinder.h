//
// Created by sebastian on 31.05.19.
//

#pragma once
#include <filesystem>
#include <fstream>
#include <optional>
#include <iostream>
#include  <sstream>
#include "../utils/expected.h"
#include "../utils/fileUtils.h"



class ConfigFinder {
private:
    static constexpr auto CONFIG_FOLDER = "config/";
    static constexpr auto CONFIG_SUFFIX = ".json";

    utils::expected<const std::filesystem::path, const std::string> fileLocation;
public:
    explicit ConfigFinder(const std::string& deviceName);

    [[nodiscard]] const utils::expected<const std::filesystem::path, const std::string> findFile(const std::string& filename, const std::string& folder) noexcept;

    [[nodiscard]] const utils::expected<const std::filesystem::path, const std::string> getFileLocation() const noexcept;

    [[nodiscard]] const utils::expected<const std::string, const std::string> getFileContents() const noexcept;
};



