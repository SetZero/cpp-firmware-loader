//
// Created by sebastian on 31.05.19.
//

#pragma once
#include <memory>
#include <string>
#include <string>
#include "SerialImpl.h"

enum class SerialMode {
    RXOnly,
    TXOnly,
    Duplex

};

template<SerialMode mode>
class Serial {
public:
    explicit Serial(const std::string& device, const unsigned int baudrate) : pimpl{std::make_unique<SerialImpl>(device, baudrate)} { }

	[[nodiscard]] std::optional<std::string> const& errorMessage() const {
		return pimpl->errorMessage();
	}

	[[nodiscard]] bool isOpen() const {
		return pimpl->isOpen();
	}
#ifdef __cpp_concepts
    template<SerialMode pMode = mode> requires mode == SerialMode::TXOnly || mode == SerialMode::Duplex
#else
    template<typename U = int, typename = std::enable_if_t<mode == SerialMode::TXOnly || mode == SerialMode::Duplex, int>>
#endif
    void writeData(std::byte data) {
        pimpl->writeData(data);
    }

#ifdef __cpp_concepts
    template<SerialMode pMode = mode> requires mode == SerialMode::TXOnly || mode == SerialMode::Duplex
#else
    template<typename U = int, typename = std::enable_if_t<mode == SerialMode::TXOnly || mode == SerialMode::Duplex, int>>
#endif
    void writeData(std::vector<std::byte> data) {
        pimpl->writeData(data);
    }

#ifdef __cpp_concepts
    template<SerialMode pMode = mode> requires mode == SerialMode::RXOnly || mode == SerialMode::Duplex
#else
    template<typename U = int, typename = std::enable_if_t<mode == SerialMode::RXOnly || mode == SerialMode::Duplex, int>>
#endif
	std::optional<std::string> reciveByte() {
        return pimpl->reciveByte();
    }

#ifdef __cpp_concepts
    template<SerialMode pMode = mode> requires mode == SerialMode::RXOnly || mode == SerialMode::Duplex
#else
    template<typename U = int, typename = std::enable_if_t<mode == SerialMode::RXOnly || mode == SerialMode::Duplex, int>>
#endif
    std::vector<std::byte> reciveBytes() {
        return pimpl->reciveBytes();
    }
private:
    const std::unique_ptr<SerialImpl> pimpl;
};



