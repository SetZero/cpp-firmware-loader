//
// Created by sebastian on 15.06.19.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>
#include "../includes/intelhexclass.h"


namespace test {
    std::string str(":100000000C9434000C943E000C943E000C943E0082\n"
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

    TEST_CASE("General Hex Class Test", "[General Hex Test]") {
        intelhex hex;
        std::istringstream buf(str);
        buf >> hex;

        REQUIRE(hex.size() == 162);
        REQUIRE(!hex.empty());
        REQUIRE(hex.getNoErrors() == 0);
        REQUIRE(hex.getNoWarnings() == 0);
    }

    TEST_CASE("Test Hex Iterator", "[Hex Iterator Test]") {
        intelhex hex;
        std::istringstream buf(str);
        buf >> hex;


        std::size_t elements = 0;
        for(const auto& e : hex) {
            elements++;
        }

        REQUIRE((*hex.begin()).data == 0x0C);
        REQUIRE(static_cast<int>((*(++hex.begin())).data) == 0x94);
        REQUIRE(elements == 162);
        REQUIRE((*--hex.end()).data == 0xCF);

        REQUIRE((*hex.begin()).address == 0);
        REQUIRE((*--hex.end()).address == 161);
    }
}

