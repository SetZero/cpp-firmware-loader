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

class SerialImpl {
public:
    SerialImpl(std::string device, unsigned int baudrate, serial::utils::SerialConfiguration config);
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

    [[nodiscard]] asio::serial_port_base::parity::type convertParity(serial::utils::Parity parity);

    template<typename T>
#ifdef __cpp_concepts
    requires std::is_floating_point_v<T>
#endif
   [[nodiscard]] asio::serial_port_base::stop_bits::type convertStopBit(T parity) {
        if (parity < 1.2) {
            return asio::serial_port_base::stop_bits::one;
        } else if (parity >= 1.2 && parity < 1.7) {
            return asio::serial_port_base::stop_bits::onepointfive;
        } else if (parity >= 1.7) {
            return asio::serial_port_base::stop_bits::two;
        }
   }

private:
    const std::string mDevice;
    const unsigned int mBaudrate = 9600;
    asio::io_service mIOService;
    asio::serial_port mPort;
	bool mOpen = false;
	std::optional<std::string> mErrorMessage = std::nullopt;
};



