//
// Created by sebastian on 16.06.19.
//

#include <catch2/catch.hpp>
#include "../src/json/configFinder.h"

namespace test {
    TEST_CASE("test config finder", "[ConfigFinder test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_config";

        auto configPath = path / "devices" / "stm" / "cortex" / "m4f" / "stm32f104.json";
        std::filesystem::create_directories(configPath.parent_path());
        {
            std::ofstream stream{configPath};
            stream << "Test String";
            stream.close();
        }

        ConfigFinder finder{"stm32f104", path};
        auto contents = finder.getFileContents();

        REQUIRE(static_cast<bool>(contents));
        REQUIRE(*contents == "Test String");

        std::filesystem::remove_all(path);
    }

    TEST_CASE("nested config folder 2", "[ConfigFinder test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_config";

        auto configPath = path / "devices" / "stm" / "cortex" / "m4f" / "stm32f104.json";
        std::filesystem::create_directories(configPath.parent_path());

        auto configPath2 = path / "another_folder_1";
        std::filesystem::create_directories(configPath2.parent_path());

        configPath2 = path / "another_folder_2";
        std::filesystem::create_directories(configPath2.parent_path());

        configPath2 = path / "another_folder_3";
        std::filesystem::create_directories(configPath2.parent_path());

        configPath2 = path / "devices" / "another_folder_1";
        std::filesystem::create_directories(configPath2.parent_path());

        configPath2 = path / "devices" / "another_folder_2";
        std::filesystem::create_directories(configPath2.parent_path());

        configPath2 = path / "devices" / "another_folder_3";
        std::filesystem::create_directories(configPath2.parent_path());


        {
            std::ofstream stream{configPath};
            stream << "Test String";
            stream.close();
        }

        ConfigFinder finder{"stm32f104", path};
        auto contents = finder.getFileContents();

        REQUIRE(static_cast<bool>(contents));
        REQUIRE(*contents == "Test String");

        std::filesystem::remove_all(path);
    }

    TEST_CASE("nested config folder", "[ConfigFinder test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_config";

        auto configPath = path / "somesuperlongfoldernamewhichshouldnotexist" / "this" / "is" / "not" / "over" / "yet" /
                "because" / "we" / "need" / "more" / "folders" / "this" / "needs" / "more" / "stm32f104.json";
        std::filesystem::create_directories(configPath.parent_path());
        {
            std::ofstream stream{configPath};
            stream << "Test String";
            stream.close();
        }

        ConfigFinder finder{"stm32f104", path};
        auto contents = finder.getFileContents();

        REQUIRE(static_cast<bool>(contents));
        REQUIRE(*contents == "Test String");

        std::filesystem::remove_all(path);
    }

    TEST_CASE("test nonexistent folder", "[ConfigFinder test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_SomeFolderString";

        auto configPath = path / "devices" / "stm" / "cortex" / "m4f" / "stm32f104.json";
        {
            std::ofstream stream{configPath};
            stream << "Test String";
            stream.close();
        }

        ConfigFinder finder{"stm32f104", path};
        auto contents = finder.getFileContents();

        REQUIRE(!static_cast<bool>(contents));
        REQUIRE(!contents.error().empty());
    }

    TEST_CASE("test nonexistent file", "[ConfigFinder test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_config";

        auto configPath = path / "devices" / "stm" / "cortex" / "m4f" / "mcu1.json";
        std::filesystem::create_directories(configPath.parent_path());

        ConfigFinder finder{"mcu1", path};
        auto contents = finder.getFileContents();

        REQUIRE(!static_cast<bool>(contents));
        REQUIRE(!contents.error().empty());
        std::filesystem::remove_all(path);
    }

    TEST_CASE("test empty file", "[ConfigFinder test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_config";

        auto configPath = path / "devices" / "stm" / "cortex" / "m4f" / "mcu2.json";
        std::filesystem::create_directories(configPath.parent_path());
        {
            std::ofstream stream{configPath};
            stream.close();
        }

        ConfigFinder finder{"mcu2", path};
        auto contents = finder.getFileContents();

        REQUIRE(static_cast<bool>(contents));
        REQUIRE(contents->empty());
        std::filesystem::remove_all(path);
    }
}