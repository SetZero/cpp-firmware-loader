//
// Created by sebastian on 31.05.19.
//

#include "SerialImpl.h"

SerialImpl::SerialImpl(std::string device, int baudrate) : mDevice{device}, mBaudrate{baudrate}, mPort{mIOService, device} {
    asio::serial_port_base::baud_rate baud(9600);
    asio::serial_port_base::character_size bitsize(8);
    asio::serial_port_base::parity parity(asio::serial_port_base::parity::none);
    asio::serial_port_base::stop_bits stopbits(asio::serial_port_base::stop_bits::one);

    mPort.set_option(baud);
    mPort.set_option(bitsize);
    mPort.set_option(parity);
    mPort.set_option(stopbits);
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


