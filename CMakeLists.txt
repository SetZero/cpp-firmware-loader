SET(PROJECT_NAME firmware-loader)

cmake_minimum_required(VERSION 3.14)
project(${PROJECT_NAME})

set(CONFIG_FOLDER "config_files")

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
	set(CMAKE_CXX_FLAGS "/O2 /W4 /EHa")
endif(WIN32)

if(UNIX)
	set(CMAKE_CXX_FLAGS "-fconcepts -Wall -Wextra -Wshadow -Wfloat-conversion -Wsign-conversion -Wsign-compare -Wmissing-field-initializers -fno-threadsafe-statics")
endif(UNIX)

find_package(Poco COMPONENTS JSON XML CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp src/commandline/parse.h src/utils/enum_constants.h src/utils/EnvironmentChecks.h src/json/deviceParser.h src/json/configFinder.h src/serial/Serial.h src/serial/SerialImpl.cpp src/serial/SerialImpl.h src/serial/AbstractSerial.h  src/json/configFinder.cpp src/json/deviceParser.cpp src/loader/DataSendManager.cpp src/loader/DataSendManager.h src/json/ConfigManager.cpp src/json/ConfigManager.h includes/intelhexclass.h includes/intelhexclass.cpp src/loader/HexReader.cpp src/loader/HexReader.h src/units/IECprefix.h src/utils/SerialUtils.h src/units/parse/unitParser.h src/utils/fileUtils.cpp src/utils/fileUtils.h )
target_link_libraries(${PROJECT_NAME} Poco::JSON Poco::XML)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/${CONFIG_FOLDER})

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${CONFIG_FOLDER} $<TARGET_FILE_DIR:${PROJECT_NAME}>)


add_executable(test_cases includes/intelhexclass.h includes/intelhexclass.cpp includes/intelhexclass.h includes/intelhexclass.h test/TestIntelHex.cpp test/main.cpp test/TestConfigManager.cpp src/json/ConfigManager.cpp src/json/ConfigManager.h src/json/ConfigManager.cpp src/json/ConfigManager.cpp src/json/configFinder.h src/json/configFinder.cpp src/utils/fileUtils.cpp src/utils/fileUtils.h src/json/deviceParser.h src/json/deviceParser.cpp test/TestConfigFinder.cpp src/serial/AbstractSerial.h test/TestSerial.cpp src/loader/HexReader.cpp src/loader/HexReader.h test/testClasses/SerialTestImpl.cpp test/testClasses/SerialTestImpl.h src/loader/DataSendManager.cpp src/loader/DataSendManager.h src/serial/SerialImpl.cpp src/serial/SerialImpl.h test/TestHexReader.cpp)
target_link_libraries(test_cases Poco::JSON Poco::XML)