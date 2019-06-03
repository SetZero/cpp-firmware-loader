//
// Created by sebastian on 03.06.19.
//

#pragma once
#include <string>
#include <vector>
#include <iterator>
#include "../serial/Serial.h"
#include "../json/ConfigManager.h"

class DataSendManager {
public:
    DataSendManager(const ConfigManager& manager, const std::string& device, const unsigned int baudrate);
    [[nodiscard]] bool isOpen() const noexcept;
    [[nodiscard]] std::optional<std::string> const& errorMessage() const noexcept;
    void bufferedWrite(std::vector<std::byte> data) noexcept;
	void flush() noexcept;
private:
	void sync() noexcept;

    Serial<SerialMode::TXOnly> mSerial;
    const ConfigManager& mManager;
    std::deque<std::byte> mBuffer;
    bool mOpen = false;
};



