//
// Created by sebastian on 17.06.19.
//

#pragma once

#include <vector>
#include "../../src/serial/AbstractSerial.h"

class SerialTestImpl : public AbstractSerial {
public:
    SerialTestImpl(std::string device, unsigned int baudrate, serial::utils::SerialConfiguration config);

    void writeData(std::byte data) override;

    void writeData(const std::vector<std::byte>& data) override;

    std::optional<std::string> reciveByte() override;

    std::vector<std::byte> reciveBytes() override;

    [[nodiscard]] bool isOpen() const override;

    [[nodiscard]] std::optional<std::string> errorMessage() const override;

    [[nodiscard]]  unsigned int baudrate() const noexcept override;

    [[nodiscard]] const std::vector<std::byte>& getVectorContents() const;
private:
    std::vector<std::byte> mVector;
    std::string mDevice;
    unsigned int mBaudrate;
    serial::utils::SerialConfiguration mConfig;
    std::optional<std::string> mError;
};



