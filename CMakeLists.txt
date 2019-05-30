cmake_minimum_required(VERSION 3.14)
project(firmware-loader)

set(CMAKE_CXX_STANDARD 20)

find_package(Poco COMPONENTS JSON XML CONFIG REQUIRED)

add_executable(firmware-loader main.cpp src/commandline/parse.h src/utils/enum_constants.h src/utils/EnvironmentChecks.h src/json/deviceParser.h)
target_link_libraries(firmware-loader Poco::JSON Poco::XML)