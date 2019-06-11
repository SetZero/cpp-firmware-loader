//
// Created by sebastian on 03.06.19.
//

#include "DataSendManager.h"

namespace firmware::serial {
    DataSendManager::DataSendManager(const json::config::ConfigManager &manager, const std::string &device,
                                     const unsigned int baudrate) :
            mSerial{device, baudrate, manager.getJSONValue<json::config::JsonOptions::serialMode>()}, mManager{std::move(manager)}, 
            mOpen{ mSerial.isOpen() }, mBytesPerBurst{ mManager.getJSONValue<json::config::JsonOptions::serialBytesPerBurst>() } {
        // preventing odd serial behaviour. It might be possible that this
        // can be removed later, if hw serial is disabled ?
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    DataSendManager::~DataSendManager() {
        flush();
    }

    bool DataSendManager::isOpen() const noexcept {
        return mOpen;
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
            for (size_t i = 0; i < mManager.getJSONValue<json::config::JsonOptions::serialSyncByteAmount>(); i++) {
                mSerial.writeData(mManager.getJSONValue<json::config::JsonOptions::serialSyncByte>());
            }
            mSerial.writeData(mManager.getJSONValue<json::config::JsonOptions::serialPreamble>());
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
        sync();
        mSerial.writeData(tmp);
        std::this_thread::sleep_for(mManager.getJSONValue<json::config::JsonOptions::serialFlashBurstDelay>());
    }
}