//
// Created by sebastian on 31.05.19.
//

#pragma once


#include <string>
#include <string>
#include <memory>
#include <optional>
#include <asio.hpp>

class SerialImpl {
public:
    SerialImpl(std::string device, unsigned int baudrate);

    ~SerialImpl();

    void writeData(std::byte data);

    void writeData(std::basic_string<std::byte> data);

	std::optional<std::string> reciveByte();

	std::basic_string<std::byte> reciveBytes();

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] std::optional<std::string> const& errorMessage() const;
private:
    const std::string mDevice;
    const unsigned int mBaudrate;
    asio::io_service mIOService;
    asio::serial_port mPort;
	bool mOpen = false;
	std::optional<std::string> mErrorMessage;
};



