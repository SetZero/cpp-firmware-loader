//
// Created by sebastian on 03.06.19.
//

#pragma once


#include <string>
#include <memory>
#include <string_view>
#include "../units/Byte.h"
#include "configFinder.h"
#include "deviceParser.h"


class ConfigManager {
public:
    ConfigManager(const std::string& deviceName);
	[[nodiscard]] const std::string& id() const noexcept;
	[[nodiscard]] const std::string& vendor() const noexcept;
	[[nodiscard]] const std::string& arch() const noexcept;
	[[nodiscard]] const std::string& subarch() const noexcept;
	[[nodiscard]] const std::string& name() const noexcept;
	[[nodiscard]] const CustomDataTypes::ComputerScience::byte& availableFlash() const noexcept;

    [[nodiscard]] std::byte syncByte() const noexcept;
    [[nodiscard]] std::byte preamble() const noexcept;
    [[nodiscard]] std::size_t syncByteAmount() const noexcept;
	[[nodiscard]] std::size_t bytesPerBurst() const noexcept;
private:
    std::unique_ptr<parser::DeviceParser> mParser;

	// device
	static constexpr auto AVAILABLE_FLASH = "/device/flash/available";
	static constexpr auto DEVICE_ID = "/device/general/id";
	static constexpr auto DEVICE_VENDOR = "/device/general/vendor";
	static constexpr auto DEVICE_ARCH = "/device/general/arch";
	static constexpr auto DEVICE_SUBARCH = "/device/general/subarch";
	static constexpr auto DEVICE_NAME = "/device/general/name";

	static constexpr auto SYNC_BYTE = "/serial/sync/syncByte";
	static constexpr auto PREAMBLE = "/serial/sync/preamble";
	static constexpr auto SYNC_BYTE_AMOUNT = "/serial/sync/syncByteAmount";
	static constexpr auto BYTES_PER_BURST = "/serial/general/bytesPerBurst";
};



