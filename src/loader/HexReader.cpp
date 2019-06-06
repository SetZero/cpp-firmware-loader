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
            std::cout << "Max size: " << maxSize << std::endl;
            if (fileSize < maxSize) {
                std::cout << "Fit!" << std::endl;
            }
            std::cout << "Errors: " << hex.getNoErrors() << std::endl;
            std::cout << "Warnings: " << hex.getNoWarnings() << std::endl;
        } else {
            std::cout << "Failed to open: " << fileLocation << std::endl;
        }
    }

    void HexReader::writeToStream(serial::DataSendManager &manager) {
        for (const auto &v : std::as_const(hex)) {
            //manager.bufferedWrite();
            std::cout << std::dec << v.address << ": 0x" << std::hex << (int) v.data << std::endl;
        }
    }

    serial::DataSendManager &operator<<(serial::DataSendManager &sender, const HexReader &reader) {
        return sender;
    }
}
