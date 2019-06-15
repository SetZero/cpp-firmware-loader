#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>

namespace test {
    TEST_CASE("Test Config Manager Constructor", "[Test Constructor]") {
        std::string fileName = std::tmpnam(nullptr);
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= fileName;
        std::filesystem::create_directories(path.parent_path());

        std::ofstream stream{ path };
        stream << "Test";
        REQUIRE(1 == 2);
    }
}