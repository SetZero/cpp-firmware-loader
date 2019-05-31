//
// Created by sebastian on 31.05.19.
//

#pragma once
#include <memory>
#include <string>
#include <vector>
#include "SerialImpl.h"

class Serial {
public:
    Serial(const std::string& device, const int baudrate) noexcept : pimpl{std::make_unique<SerialImpl>(device, baudrate)} { }

    void writeData(std::byte data) {
        pimpl->writeData(data);
    }

    void writeData(std::vector<std::byte> data) {
        pimpl->writeData(data);
    }

    std::byte reciveByte() {
        return pimpl->reciveByte();
    }

    std::vector<std::byte> reciveBytes() {
        return pimpl->reciveBytes();
    }
private:
    const std::unique_ptr<SerialImpl> pimpl;
};



