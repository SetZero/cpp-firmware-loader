//
// Created by sebastian on 03.06.19.
//

#include "DataSendManager.h"

DataSendManager::DataSendManager(const ConfigManager& manager, const std::string &device, const unsigned int baudrate) : mSerial{device, baudrate}, mManager{std::move(manager)}{
    if(mSerial.isOpen()) {
        for(size_t i=0; i < manager.syncByteAmount(); i++) {
            mSerial.writeData(manager.syncByte());
        }
        mSerial.writeData(manager.preamble());
        mOpen = true;
    }
}

bool DataSendManager::isOpen() const noexcept {
    return mOpen;
}

std::optional<std::string> const &DataSendManager::errorMessage() const noexcept {
    return mSerial.errorMessage();
}


void DataSendManager::bufferedWrite(std::vector<decltype(mBuffer)::value_type> data) noexcept {
    mBuffer.insert(std::begin(mBuffer), std::begin(data), std::end(data));
    if(mBuffer.size() < mManager.bytesPerBurst()) {
        std::vector<decltype(mBuffer)::value_type> tmp;
        auto it = std::next(std::begin(mBuffer), static_cast<ssize_t>(mManager.bytesPerBurst()));
        std::move(mBuffer.begin(), it, std::back_inserter(tmp));

        mBuffer.erase(std::begin(mBuffer), it);

        for(auto& val : tmp) {
            std::cout << std::hex << static_cast<unsigned int>(val) << std::endl;
        }
    }
}