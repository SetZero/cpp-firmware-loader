//
// Created by sebastian on 31.05.19.
//

#pragma once


#include <string>
#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <type_traits>
#include <asio.hpp>
#include "../utils/SerialUtils.h"

class AbstractSerial {
public:
    virtual void writeData(std::byte data) = 0;

    virtual void writeData(const std::vector<std::byte>& data) = 0;

    virtual std::optional<std::string> reciveByte() = 0;

    virtual std::vector<std::byte> reciveBytes() = 0;

    [[nodiscard]] virtual bool isOpen() const = 0;

    [[nodiscard]] virtual std::optional<std::string> const& errorMessage() const = 0;

    [[nodiscard]] virtual  constexpr unsigned int baudrate() const noexcept = 0;
};



