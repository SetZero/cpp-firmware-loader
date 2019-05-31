//
// Created by sebastian on 31.05.19.
//

#include "SerialImpl.h"

SerialImpl::SerialImpl(std::string device, int baudrate) : mDevice{std::move(device)}, mBaudrate{baudrate} {

}

void SerialImpl::writeData(std::byte data) {

}

void SerialImpl::writeData(std::vector<std::byte> data) {

}

std::byte SerialImpl::reciveByte() {
    std::byte result;
    return result;
}

std::vector<std::byte> SerialImpl::reciveBytes() {
    return std::vector<std::byte>();
}
