//
// Created by sebastian on 03.06.19.
//

#include "DataSendManager.h"

DataSendManager::DataSendManager(const ConfigManager& manager, const std::string& device, const unsigned int baudrate) : 
	mSerial{ device, baudrate }, mManager{ std::move(manager) }, mOpen{ mSerial.isOpen() } 
{
    // preventing odd serial behaviour. It might be possible that this
    // can be removed later, if hw serial is disabled ?
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

bool DataSendManager::isOpen() const noexcept {
    return mOpen;
}

std::optional<std::string> const &DataSendManager::errorMessage() const noexcept {
    return mSerial.errorMessage();
}


void DataSendManager::bufferedWrite(std::vector<decltype(mBuffer)::value_type> data) noexcept {
    mBuffer.insert(std::begin(mBuffer), std::begin(data), std::end(data));
    while(mBuffer.size() >= mManager.bytesPerBurst()) {
        std::vector<decltype(mBuffer)::value_type> tmp;
        auto it = std::next(std::begin(mBuffer), static_cast<long>(mManager.bytesPerBurst()));
        std::move(mBuffer.begin(), it, std::back_inserter(tmp));

        mBuffer.erase(std::begin(mBuffer), it);
		sync();
		mSerial.writeData(tmp);
	}
}

void DataSendManager::sync() noexcept {
	if (mSerial.isOpen()) {
		for (size_t i = 0; i < mManager.syncByteAmount(); i++) {
			mSerial.writeData(mManager.syncByte());
		}
		mSerial.writeData(mManager.preamble());
	}
}
