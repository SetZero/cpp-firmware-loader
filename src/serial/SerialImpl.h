//
// Created by sebastian on 31.05.19.
//

#pragma once


#include <string>
#include <vector>

class SerialImpl {
public:
    SerialImpl(std::string device, int baudrate);

    void writeData(std::byte data);

    void writeData(std::vector<std::byte> data);

    std::byte reciveByte();

    std::vector<std::byte> reciveBytes();
private:
    const std::string mDevice;
    const int mBaudrate;
};



