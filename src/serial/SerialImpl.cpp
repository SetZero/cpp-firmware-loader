//
// Created by sebastian on 31.05.19.
//

#include "SerialImpl.h"

SerialImpl::SerialImpl(std::string device, int baudrate) : mDevice{device}, mBaudrate{baudrate}, mPort{mIOService, device} {
}

void SerialImpl::writeData(std::byte data) {
    asio::streambuf b;
    b.commit(
            asio::buffer_copy(
                    b.prepare(sizeof(data)),
                    asio::buffer(&data, sizeof(data))
            )
    );

    asio::write(mPort, b);
    mIOService.poll();
}

void SerialImpl::writeData(std::vector<std::byte> data) {
    asio::streambuf b;
    b.commit(
            asio::buffer_copy(
                    b.prepare(sizeof(data)),
                    asio::buffer(&data, sizeof(data))
            )
    );
    mIOService.poll();
}

std::byte SerialImpl::reciveByte() {
    std::byte result;
    mIOService.poll();
    return result;
}

std::vector<std::byte> SerialImpl::reciveBytes() {
    mIOService.poll();
    return std::vector<std::byte>();
}


