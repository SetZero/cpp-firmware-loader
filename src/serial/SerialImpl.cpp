//
// Created by sebastian on 31.05.19.
//

#include "SerialImpl.h"

SerialImpl::SerialImpl(std::string device, int baudrate) : mDevice{device}, mBaudrate{baudrate}, mPort{mIOService, device} {
    asio::serial_port_base::baud_rate BAUD(9600);

    mPort.set_option(BAUD);
}

SerialImpl::~SerialImpl() {
    mIOService.stop();
    mPort.close();
}

void SerialImpl::writeData(std::byte data) {
    asio::write(mPort, asio::buffer({data}, 1));
    mIOService.poll();
}

void SerialImpl::writeData(std::vector<std::byte> data) {
    asio::write(mPort, asio::buffer(data, data.size()));
    mIOService.poll();
}

std::string SerialImpl::reciveByte() {
    mIOService.poll();
    asio::streambuf readbuf;
    asio::read(mPort, readbuf, asio::transfer_exactly(1));
    return std::string{asio::buffers_begin(readbuf.data()), asio::buffers_end(readbuf.data())};
}

std::vector<std::byte> SerialImpl::reciveBytes() {
    mIOService.poll();
    return std::vector<std::byte>();
}


