#include <iostream>
#include <Poco/JSON/ParseHandler.h>
#include <catch.hpp>
#include <clara.hpp>
#include <asio.hpp>

int main() {
    Poco::JSON::ParseHandler handler;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}