//
// Created by sebastian on 03.06.19.
//

#include "ConfigManager.h"

ConfigManager::ConfigManager(const std::string &deviceName) {
    ConfigFinder config{deviceName};
    if(auto& content = config.getFileContents()) {
        mParser = std::make_unique<parser::DeviceParser>(*content);
    } else {
        throw std::runtime_error("Unable to locate Config File!");
    }
}

const std::string& ConfigManager::id() const noexcept
{
	return mParser->getJsonAsString(DEVICE_ID);
}

const std::string& ConfigManager::vendor() const noexcept
{
	return mParser->getJsonAsString(DEVICE_VENDOR);
}

const std::string& ConfigManager::arch() const noexcept
{
	return mParser->getJsonAsString(DEVICE_ARCH);
}

const std::string& ConfigManager::subarch() const noexcept
{
	return mParser->getJsonAsString(DEVICE_SUBARCH);
}

const std::string& ConfigManager::name() const noexcept
{
	return mParser->getJsonAsString(DEVICE_NAME);
}

[[nodiscard]] std::byte ConfigManager::syncByte() const noexcept {
    return mParser->getJSONByteValue(SYNC_BYTE);
}

[[nodiscard]] std::byte ConfigManager::preamble() const noexcept {
    return mParser->getJSONByteValue(PREAMBLE);
}

std::size_t ConfigManager::syncByteAmount() const noexcept {
    return mParser->getJSONValue<std::size_t>(SYNC_BYTE_AMOUNT);
}

std::size_t ConfigManager::bytesPerBurst() const noexcept {
	auto flash = mParser->getJsonAsString(AVAILABLE_FLASH);

    return mParser->getJSONValue<std::size_t>(BYTES_PER_BURST);
}

const CustomDataTypes::ComputerScience::byte& ConfigManager::availableFlash() const noexcept
{
	//TODO
	return CustomDataTypes::ComputerScience::byte(1);
}
