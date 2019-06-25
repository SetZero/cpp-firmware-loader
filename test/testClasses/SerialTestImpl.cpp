#include <utility>

//
// Created by sebastian on 17.06.19.
//

#include "SerialTestImpl.h"

SerialTestImpl::SerialTestImpl(std::string device, unsigned int baudrate, serial::utils::SerialConfiguration config)
    : mDevice{std::move(device)}, mBaudrate{baudrate}, mConfig{config} {}

void SerialTestImpl::writeData(std::byte data) {
    mVector.push_back(data);
}

void SerialTestImpl::writeData(const std::vector<std::byte> &data) {
    mVector.insert(std::end(mVector), std::begin(data), std::end(data));
}

std::optional<std::string> SerialTestImpl::reciveByte() {
    return std::nullopt;
}

std::vector<std::byte> SerialTestImpl::reciveBytes() {
    return mVector;
}

bool SerialTestImpl::isOpen() const {
    return true;
}

std::optional<std::string> SerialTestImpl::errorMessage() const {
    return mError;
}

unsigned int SerialTestImpl::baudrate() const noexcept {
    return mBaudrate;
}

const std::vector<std::byte>& SerialTestImpl::getVectorContents() const {
    return mVector;
}


