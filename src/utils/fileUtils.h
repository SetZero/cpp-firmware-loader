#pragma once
#include <filesystem>
#include <sstream>
#include <fstream>
#include "expected.h"

namespace utils {
    utils::expected<const std::string, const std::string> readFile(const std::filesystem::path& path) noexcept;
}