//
// Created by sebastian on 31.05.19.
//

#pragma once


#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <asio.hpp>

class SerialImpl {
public:
    SerialImpl(std::string device, unsigned int baudrate);

    ~SerialImpl();

    void writeData(std::byte data);

    void writeData(std::vector<std::byte> data);

	std::optional<std::string> reciveByte();

    std::vector<std::byte> reciveBytes();

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] std::optional<std::string> const& errorMessage() const;
private:
    const std::string mDevice;
    const unsigned int mBaudrate;
    asio::io_service mIOService;
    std::unique_ptr<asio::serial_port> mPort = nullptr;
	bool mOpen = false;
	std::optional<std::string> mErrorMessage;
};



