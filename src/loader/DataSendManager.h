//
// Created by sebastian on 03.06.19.
//

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <chrono>
#include <thread>
#include "../serial/Serial.h"
#include "../serial/AbstractSerial.h"
#include "../json/ConfigManager.h"
#include "../utils/utils.h"

namespace firmware::serial {
    class DataSendManager {
    public:
        DataSendManager(const json::config::ConfigManager &manager, const std::string &device, const unsigned int baudrate);

        DataSendManager(const json::config::ConfigManager& manager, std::unique_ptr <AbstractSerial> serialImplementation);

        //TODO: rule of 5
        ~DataSendManager();

        [[nodiscard]] bool isOpen() const noexcept;

        [[nodiscard]] std::optional<std::string> const &errorMessage() const noexcept;

        [[nodiscard]] std::size_t const & bytesPerBurst() const noexcept;

        void bufferedWrite(std::vector<std::byte> data);

        void bufferedWrite(std::byte data);

        void flush() noexcept;

        friend DataSendManager &operator<<(DataSendManager &parse, std::byte data);

    private:
        void sync() noexcept;

        void sendBuffer();

        Serial<SerialMode::TXOnly> mSerial;
        std::deque<std::byte> mBuffer;
        bool mSynced = false;
        const std::size_t mBytesPerBurst;
        const json::config::ConfigManager &mManager;
    };
}


