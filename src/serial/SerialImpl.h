//
// Created by sebastian on 31.05.19.
//

#pragma once


#include <string>
#include <vector>
#include <asio.hpp>

class SerialImpl {
public:
    SerialImpl(std::string device, unsigned int baudrate);

    ~SerialImpl();

    void writeData(std::byte data);

    void writeData(std::vector<std::byte> data);

    std::string reciveByte();

    std::vector<std::byte> reciveBytes();
private:
    const std::string mDevice;
    const unsigned int mBaudrate;
    asio::io_service mIOService;
    asio::serial_port mPort;
};



