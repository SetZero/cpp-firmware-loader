cmake_minimum_required(VERSION 3.14)
project(firmware-loader)

set(CONFIG_FOLDER "config_files")

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
	set(CMAKE_CXX_FLAGS "/O2 /W4")
endif(WIN32)

if(UNIX)
	set(CMAKE_CXX_FLAGS "-fconcepts -Wall -Wextra -Wshadow -Wfloat-conversion -Wsign-conversion -Wsign-compare -Wmissing-field-initializers -fno-threadsafe-statics")
endif(UNIX)

find_package(Poco COMPONENTS JSON XML CONFIG REQUIRED)

add_executable(firmware-loader main.cpp src/commandline/parse.h src/utils/enum_constants.h src/utils/EnvironmentChecks.h src/json/deviceParser.h src/json/configFinder.h src/serial/Serial.h src/serial/SerialImpl.cpp src/serial/SerialImpl.h src/json/configFinder.cpp src/json/deviceParser.cpp src/loader/DataSendManager.cpp src/loader/DataSendManager.h src/json/ConfigManager.cpp src/json/ConfigManager.h)
target_link_libraries(firmware-loader Poco::JSON Poco::XML)

add_custom_command(TARGET firmware-loader PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${CONFIG_FOLDER} $<TARGET_FILE_DIR:firmware-loader>)