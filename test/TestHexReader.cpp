//
// Created by sebastian on 17.06.19.
//

#include <catch2/catch.hpp>
#include "../src/loader/HexReader.h"
namespace test {
    std::string hexStr(":100000000C9434000C943E000C943E000C943E0082\n"
                    ":100010000C943E000C943E000C943E000C943E0068\n"
                    ":100020000C943E000C943E000C943E000C943E0058\n"
                    ":100030000C943E000C943E000C943E000C943E0048\n"
                    ":100040000C943E000C943E000C943E000C943E0038\n"
                    ":100050000C943E000C943E000C943E000C943E0028\n"
                    ":100060000C943E000C943E0011241FBECFEFD8E04C\n"
                    ":10007000DEBFCDBF0E9440000C944F000C940000E6\n"
                    ":10008000209A91E085B1892785B92FEF39E688E17B\n"
                    ":10009000215030408040E1F700C00000F3CFF894D9\n"
                    ":0200A000FFCF90\n"
                    ":00000001FF");

    TEST_CASE("Hex Reader Test file too big", "[Filesize Test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_firmware/firmware.hex";
        std::filesystem::create_directories(path.parent_path());
        {
            std::ofstream stream{path};
            stream << hexStr;
            stream.close();
        }

        firmware::reader::HexReader reader{path.string(), CustomDataTypes::ComputerScience::byte{10}};
        REQUIRE(!static_cast<bool>(reader));
        REQUIRE(reader.errorMessage().has_value());

        std::filesystem::remove_all(path);
    }

    TEST_CASE("Hex Reader Test fit file size", "[Filesize Test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_firmware/firmware.hex";
        std::filesystem::create_directories(path.parent_path());
        {
            std::ofstream stream{path};
            stream << hexStr;
            stream.close();
        }

        firmware::reader::HexReader reader{path.string(), CustomDataTypes::ComputerScience::megabyte{10}};
        REQUIRE(static_cast<bool>(reader));
        REQUIRE(!reader.errorMessage().has_value());

        std::filesystem::remove_all(path);
    }

    TEST_CASE("Hex Reader Test file size", "[Filesize Test]") {
        auto path = std::filesystem::path{ std::filesystem::temp_directory_path() };
        path /= "fileware_loader_firmware/firmware.hex";
        std::filesystem::create_directories(path.parent_path());
        {
            std::ofstream stream{path};
            stream << hexStr;
            stream.close();
        }

        firmware::reader::HexReader reader{path.string(), CustomDataTypes::ComputerScience::megabyte{10}};
        REQUIRE(reader.getFileSize() == CustomDataTypes::ComputerScience::byte{162});

        std::filesystem::remove_all(path);
    }
}