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
	//TODO: rule of 5
	~DataSendManager();
    [[nodiscard]] bool isOpen() const noexcept;
    [[nodiscard]] std::optional<std::string> const& errorMessage() const noexcept;
    void bufferedWrite(std::vector<std::byte> data) noexcept;
    void bufferedWrite(std::byte data) noexcept;
	void flush() noexcept;
    friend DataSendManager& operator<<( DataSendManager& parse,  std::byte data);
private:
	void sync() noexcept;
	void sendBuffer() noexcept;

    Serial<SerialMode::TXOnly> mSerial;
    const ConfigManager& mManager;
    std::deque<std::byte> mBuffer;
    bool mOpen = false;
};



