#include "fileUtils.h"

namespace utils {
    utils::expected<const std::string, const std::string> readFile(const std::filesystem::path& path) noexcept {
        try {
            std::ifstream file{ path.string() };
            if(!file.good()) {
                return utils::make_unexpected("Couldn't open the config file!");
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        } catch (std::ios_base::failure& fail) {
            std::stringstream ss;
            ss << "The following error occured during file read operation:\n" << fail.what();
            return utils::make_unexpected(ss.str());
        }
    }
}
