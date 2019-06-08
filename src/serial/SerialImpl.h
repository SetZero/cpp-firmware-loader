//
// Created by sebastian on 31.05.19.
//

#pragma once


#include <string>
#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <asio.hpp>

class SerialImpl {
public:
    SerialImpl(std::string device, unsigned int baudrate);
    ~SerialImpl();
	SerialImpl(const SerialImpl& a) = delete;
	SerialImpl& operator=(SerialImpl other) = delete;
	SerialImpl(SerialImpl&& o) = delete;
	SerialImpl& operator=(SerialImpl&& other) = delete;

    void writeData(std::byte data);

    void writeData(std::vector<std::byte> data);

	std::optional<std::string> reciveByte();

	std::vector<std::byte> reciveBytes();

	[[nodiscard]] bool isOpen() const;

	[[nodiscard]] std::optional<std::string> const& errorMessage() const;
private:
    const std::string mDevice;
    const unsigned int mBaudrate = 9600;
    asio::io_service mIOService;
    asio::serial_port mPort;
	bool mOpen = false;
	std::optional<std::string> mErrorMessage = std::nullopt;
};



