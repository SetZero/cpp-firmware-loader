//
// Created by sebastian on 03.06.19.
//

#include "DataSendManager.h"

namespace firmware::serial {
    DataSendManager::DataSendManager(const json::config::ConfigManager &manager, const CommunicationData& data) :
            mSerial{data.device, data.baudrate, manager.getJSONValue<json::config::JsonOptions::serialMode>()},
            mBytesPerBurst{ manager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>() },
            mMetadataSize{ manager.getJSONValue<json::config::JsonOptions::serialMetadataSize>() },
            mStartupWaitTime{ manager.getJSONValue<firmware::json::config::JsonOptions::serialWaitTimeForReset>() },
            mManager{ std::move(manager) } {
        // preventing odd serial behaviour. It might be possible that this
        // can be removed later, if hw serial is disabled ?
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        initialSync();
    }

    DataSendManager::DataSendManager(const json::config::ConfigManager& manager, const CommunicationData& data, std::chrono::milliseconds startupWaitTime) :
        mSerial{ data.device, data.baudrate, manager.getJSONValue<json::config::JsonOptions::serialMode>() },
        mBytesPerBurst{ manager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>() },
        mMetadataSize{ manager.getJSONValue<json::config::JsonOptions::serialMetadataSize>() },
        mStartupWaitTime{ startupWaitTime },
        mManager{ std::move(manager) } {
        initialSync();
    }

    DataSendManager::DataSendManager(const json::config::ConfigManager& manager, std::unique_ptr<AbstractSerial> serialImplementation, bool startupSync) :
            mSerial {std::move(serialImplementation)},
            mBytesPerBurst {manager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>()},
            mMetadataSize{ manager.getJSONValue<json::config::JsonOptions::serialMetadataSize>() },
            mStartupWaitTime{ manager.getJSONValue<firmware::json::config::JsonOptions::serialWaitTimeForReset>() },
            mManager{ std::move(manager) } {
        if (startupSync) {
            initialSync();
        }
    }

    DataSendManager::~DataSendManager() {
        flush();
    }

    bool DataSendManager::isOpen() const noexcept {
        return mSerial.isOpen();
    }

    std::optional<std::string> const &DataSendManager::errorMessage() const noexcept {
        return mSerial.errorMessage();
    }

    std::size_t const& DataSendManager::bytesPerBurst() const noexcept
    {
        return mBytesPerBurst;
    }

    std::size_t const& DataSendManager::metadataSize() const noexcept {
        return mMetadataSize;
    }

    void DataSendManager::metadataWrite(std::byte data) {
        mBuffer.push_back(data);
        while (mBuffer.size() >= metadataSize()) {
            sendBuffer(metadataSize());
        }
    }

    void DataSendManager::metadataWrite(const std::vector<std::byte>& data) {
        mBuffer.insert(std::begin(mBuffer), std::begin(data), std::end(data));
        while (mBuffer.size() >= metadataSize()) {
            sendBuffer(metadataSize());
        }
    }

    void DataSendManager::bufferedWrite(const std::vector<decltype(mBuffer)::value_type>& data) {
        mBuffer.insert(std::begin(mBuffer), std::begin(data), std::end(data));
        while (mBuffer.size() >= bytesPerBurst()) {
            sendBuffer();
        }
    }

    void DataSendManager::bufferedWrite(std::byte data) {
        mBuffer.push_back(data);
        if (mBuffer.size() >= mManager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>()) {
            sendBuffer();
        }
    }

    void DataSendManager::flush() noexcept {
        if (mBuffer.empty()) return;
        const auto remainingBit = mManager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>() - mBuffer.size();
        if (remainingBit < mManager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>()) {
            std::fill_n(std::back_inserter(mBuffer), remainingBit, mManager.getJSONValue<json::config::JsonOptions::unusedFlashByte>());
            sendBuffer();
        }
    }

    void DataSendManager::sync() noexcept {
        if (mSerial.isOpen()) {
            const auto syncBytes = mManager.getJSONValue<json::config::JsonOptions::serialSyncByteAmount>();
            std::vector<std::byte> tmpVector;
            tmpVector.assign(syncBytes, mManager.getJSONValue<json::config::JsonOptions::serialSyncByte>());
            tmpVector.push_back(mManager.getJSONValue<json::config::JsonOptions::serialPreamble>());
            mSerial.writeData(tmpVector);
        }
    }

    DataSendManager &operator<<(DataSendManager &parse, std::byte data) {
        parse.bufferedWrite(data);
        return parse;
    }

    void DataSendManager::sendBuffer() {
        sendBuffer(static_cast<long>(mManager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>()));
    }

    void DataSendManager::sendBuffer(std::size_t bufferLength) {
        std::vector<decltype(mBuffer)::value_type> tmp;
        auto it = std::next(std::begin(mBuffer), bufferLength);
        std::move(mBuffer.begin(), it, std::back_inserter(tmp));

        mBuffer.erase(std::begin(mBuffer), it);
        if (mManager.getJSONValue<json::config::JsonOptions::serialResyncAfterBurst>() && !mSynced) {
            sync();
        }
        mSerial.writeData(tmp);
        //Bug: This will not wait for the transmission to be over :-/
        auto baud = mSerial.baudrate();
        auto bitDuration = std::chrono::duration<double, std::ratio<1>>{ 1.0 / baud };

        std::this_thread::sleep_for(bitDuration * tmp.size() * 10);
        std::this_thread::sleep_for(mManager.getJSONValue<json::config::JsonOptions::serialFlashBurstDelay>());
    }
    void DataSendManager::initialSync() {
        using namespace utils::printable;
        std::cout << "Waiting for " << mStartupWaitTime.count() << "ms ..." << std::endl;
        auto start = std::chrono::system_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start) <  mStartupWaitTime) {
            mSerial.writeData(std::byte{ mManager.getJSONValue<json::config::JsonOptions::serialSyncByte>() });
        }
    }
}