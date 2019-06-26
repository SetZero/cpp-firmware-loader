//
// Created by sebastian on 31.05.19.
//

#include "SerialImpl.h"

SerialImpl::SerialImpl(std::string device, unsigned int baudrate, serial::utils::SerialConfiguration config) : 
    mDevice{ device }, mBaudrate{ baudrate }, mPort{ mIOService } {
	try {
		mPort.open(device);
	}
	catch (std::runtime_error& e) {
		mErrorMessage = e.what();
		return;
	}
	mOpen = true;

	using serial = asio::serial_port_base;
	serial::baud_rate baud(baudrate);
	serial::character_size bitsize(config.dataBits);
	serial::parity parity(convertParity(config.parityBit));
	serial::stop_bits stopbits(convertStopBit(config.stopBits));
	serial::flow_control flowcontrol(serial::flow_control::none);

    mPort.set_option(baud);
    mPort.set_option(bitsize);
    mPort.set_option(parity);
    mPort.set_option(stopbits);
	mPort.set_option(flowcontrol);
}

SerialImpl::~SerialImpl() {
    mIOService.stop();
	if (mOpen) {
		mPort.cancel();
		mPort.close();
	}
}

void SerialImpl::writeData(std::byte data) {
	if (mOpen) {
		asio::write(mPort, asio::buffer({ data }, 1));
		mIOService.poll();
	}
}

void SerialImpl::writeData(const std::vector<std::byte>& data) {
	if (mOpen) {
		asio::write(mPort, asio::buffer(data, data.size()));
		mIOService.poll();
	}
}

std::optional<std::string> SerialImpl::reciveByte() {
	if (mOpen) {
		mIOService.poll();
		asio::streambuf readbuf;
		asio::read(mPort, readbuf, asio::transfer_exactly(1));
		return std::string{ asio::buffers_begin(readbuf.data()), asio::buffers_end(readbuf.data()) };
	}
	return std::nullopt;
}

std::vector<std::byte> SerialImpl::reciveBytes() {
	if (mOpen) {
		mIOService.poll();
		return std::vector<std::byte>();
	}
	return {};
}

bool SerialImpl::isOpen() const
{
	return mOpen;
}

std::optional<std::string> SerialImpl::errorMessage() const
{
	return mErrorMessage;
}

unsigned int SerialImpl::baudrate() const noexcept {
    return mBaudrate;
}

asio::serial_port_base::parity::type SerialImpl::convertParity(serial::utils::Parity parity)
{
    switch (parity) {
    case serial::utils::Parity::even:
        return asio::serial_port_base::parity::even;
    case serial::utils::Parity::odd:
        return asio::serial_port_base::parity::odd;
    case serial::utils::Parity::none:
        return asio::serial_port_base::parity::none;
    case serial::utils::Parity::unknown:
    default:
        return asio::serial_port_base::parity::none;
    }
}


