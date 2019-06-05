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

CustomDataTypes::ComputerScience::byte ConfigManager::availableFlash() const noexcept
{
	//TODO
	return CustomDataTypes::ComputerScience::byte(1);
}
