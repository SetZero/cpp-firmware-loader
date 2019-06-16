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
#include "AbstractSerial.h"

class SerialImpl : public AbstractSerial {
public:
    SerialImpl(std::string device, unsigned int baudrate, serial::utils::SerialConfiguration config);
    ~SerialImpl();
	SerialImpl(const SerialImpl& a) = delete;
	SerialImpl& operator=(SerialImpl other) = delete;
	SerialImpl(SerialImpl&& o) = delete;
	SerialImpl& operator=(SerialImpl&& other) = delete;

    void writeData(std::byte data) override;

    void writeData(const std::vector<std::byte>& data) override;

	std::optional<std::string> reciveByte() override;

	std::vector<std::byte> reciveBytes() override;

	[[nodiscard]] bool isOpen() const override;

	[[nodiscard]] const std::optional<std::string>&  errorMessage() const override;

    [[nodiscard]] unsigned int baudrate() const noexcept override;
private:
    [[nodiscard]] asio::serial_port_base::parity::type convertParity(serial::utils::Parity parity);

    template<typename T>
    [[nodiscard]] asio::serial_port_base::stop_bits::type convertStopBit(T parity)
#ifdef __cpp_concepts
        requires std::is_floating_point_v<T>
#endif
    {
        if (parity < 1.2) {
            return asio::serial_port_base::stop_bits::one;
        } else if (parity >= 1.2 && parity < 1.7) {
            return asio::serial_port_base::stop_bits::onepointfive;
        } else if (parity >= 1.7) {
            return asio::serial_port_base::stop_bits::two;
        } else {
            return asio::serial_port_base::stop_bits::one;
        }
    }


    const std::string mDevice;
    const unsigned int mBaudrate = 9600;
    asio::io_service mIOService;
    asio::serial_port mPort;
	bool mOpen = false;
	std::optional<std::string> mErrorMessage = std::nullopt;
};



