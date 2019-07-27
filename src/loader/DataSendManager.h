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
    struct CommunicationData {
        const std::string& device;
        const unsigned int baudrate;
    };

    class DataSendManager {
    public:
        DataSendManager(const json::config::ConfigManager& manager, const CommunicationData& data);

        DataSendManager(const json::config::ConfigManager& manager, const CommunicationData& data, std::chrono::milliseconds startupWaitTime);

        DataSendManager(const json::config::ConfigManager& manager, std::unique_ptr <AbstractSerial> serialImplementation, bool startupSync = true);

        //TODO: rule of 5
        //~DataSendManager();

        [[nodiscard]] bool isOpen() const noexcept;

        [[nodiscard]] std::optional<std::string> errorMessage() const noexcept;

        [[nodiscard]] std::size_t bytesPerBurst() const noexcept;

        [[nodiscard]] std::size_t metadataSize() const noexcept;

        void metadataWrite(std::byte data);

        void metadataWrite(const std::vector<std::byte>& data);

        void bufferedWrite(const std::vector<std::byte>& data);

        void bufferedWrite(std::byte data);

        void flush() noexcept;

        friend DataSendManager &operator<<(const DataSendManager& parse, std::byte data);

    private:
        void sync() noexcept;

        void sendBuffer();

        void sendBuffer(std::size_t bufferLength);

        void initialSync();

        Serial<SerialMode::TXOnly> mSerial;
        std::deque<std::byte> mBuffer;
        bool mSynced = false;
        const std::size_t mBytesPerBurst;
        const std::size_t mMetadataSize;
        const std::chrono::milliseconds mStartupWaitTime;
        const json::config::ConfigManager& mManager;
    };
}


