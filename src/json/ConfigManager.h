//
// Created by sebastian on 03.06.19.
//

#pragma once


#include <string>
#include <memory>
#include "configFinder.h"
#include "deviceParser.h"

class ConfigManager {
public:
    ConfigManager(const std::string& deviceName);
    [[nodiscard]] std::byte syncByte() const noexcept;
    [[nodiscard]] std::byte preamble() const noexcept;
    [[nodiscard]] std::size_t syncByteAmount() const noexcept;
    [[nodiscard]] std::size_t bytesPerBurst() const noexcept;
private:
    std::unique_ptr<parser::DeviceParser> mParser;

    static constexpr auto SYNC_BYTE = "/serial/sync/syncByte";
    static constexpr auto PREAMBLE = "/serial/sync/preamble";
    static constexpr auto SYNC_BYTE_AMOUNT =  "/serial/sync/syncByteAmount";
    static constexpr auto BYTES_PER_BURST =  "/serial/general/bytesPerBurst";
};



