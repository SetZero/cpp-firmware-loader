//
// Created by sebastian on 03.06.19.
//

#include "DataSendManager.h"

namespace firmware::serial {
    DataSendManager::DataSendManager(const json::config::ConfigManager &manager, const std::string &device,
                                     const unsigned int baudrate) :
            mSerial{device, baudrate, manager.getJSONValue<json::config::JsonOptions::serialMode>()},
            mBytesPerBurst{ manager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>() }, mManager{ std::move(manager) } {
        // preventing odd serial behaviour. It might be possible that this
        // can be removed later, if hw serial is disabled ?
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
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


    void DataSendManager::bufferedWrite(std::vector<decltype(mBuffer)::value_type> data) {
        mBuffer.insert(std::begin(mBuffer), std::begin(data), std::end(data));
        while (mBuffer.size() >= mManager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>()) {
            sendBuffer();
        }
    }

    void DataSendManager::flush() noexcept {
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

    void DataSendManager::bufferedWrite(std::byte data) {
        mBuffer.push_back(data);
        if (mBuffer.size() >= mManager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>()) {
            sendBuffer();
        }
    }

    DataSendManager &operator<<(DataSendManager &parse, std::byte data) {
        parse.bufferedWrite(data);
        return parse;
    }

    void DataSendManager::sendBuffer() {
        std::vector<decltype(mBuffer)::value_type> tmp;
        auto it = std::next(std::begin(mBuffer), static_cast<long>(mManager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>()));
        std::move(mBuffer.begin(), it, std::back_inserter(tmp));

        mBuffer.erase(std::begin(mBuffer), it);
        if (mManager.getJSONValue<json::config::JsonOptions::serialResyncAfterBurst>() && !mSynced) {
            sync();
        }
        mSerial.writeData(tmp);
        //Bug: This will not wait for the transmission to be over :-/
        auto baud = mSerial.baudrate();
        auto bitDuration = std::chrono::duration<double, std::ratio<1>>{ 1.0 / baud };

        std::this_thread::sleep_for(bitDuration * tmp.size());
        std::this_thread::sleep_for(mManager.getJSONValue<json::config::JsonOptions::serialFlashBurstDelay>());
    }
}