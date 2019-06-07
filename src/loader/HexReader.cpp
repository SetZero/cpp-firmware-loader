//
// Created by sebastian on 05.06.19.
//

#include "HexReader.h"

namespace firmware::utils {
    HexReader::HexReader(const std::string &fileLocation, const HexReader::byte &maxSize) {
        std::ifstream intelHexInput;
        intelHexInput.open(fileLocation, std::ifstream::in);
        if (intelHexInput.good()) {
            intelHexInput >> hex;
            byte fileSize = HexReader::byte{static_cast<long>(hex.currentAddress())};

            std::cout << "Final address is 0x" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex
                      << hex.currentAddress() << std::dec << std::endl;
            std::cout << "File size: " << fileSize << std::endl;
            if (fileSize > maxSize) {
                std::stringstream ss;
                ss << "Unable to write " << fileSize << " in the available space of " << maxSize;
                mErrorMessage = ss.str();
                return;
            }
            if(hex.getNoErrors() > 0) {
                std::stringstream ss;
                ss << "There were " << hex.getNoErrors() << " errors while parsing the hex file!";
                mErrorMessage = ss.str();
                return;
            }
        } else {
            std::stringstream ss;
            ss << "Failed to open: " << fileLocation;
            mErrorMessage = ss.str();
            return;
        }
        mCanWrite = true;
    }

    void HexReader::writeToStream(serial::DataSendManager &manager) {
        if(!mCanWrite) return;
        for (const auto &v : std::as_const(hex)) {
            std::cout << std::dec << v.address << ": 0x" << std::hex << (int) v.data << std::endl;
        }
    }

    HexReader::operator bool() const noexcept {
        return mCanWrite;
    }

    const std::optional<std::string>& HexReader::errorMessage() const noexcept {
        return mErrorMessage;
    }

    serial::DataSendManager &operator<<(serial::DataSendManager &sender, const HexReader &reader) {
        return sender;
    }
}
