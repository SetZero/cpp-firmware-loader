//
// Created by sebastian on 05.06.19.
//

#include "HexReader.h"

HexReader::HexReader(const std::string &fileLocation, const HexReader::byte &maxSize) {
    std::ifstream intelHexInput;
    intelHexInput.open(fileLocation, ifstream::in);
	if (intelHexInput.good()) {
		intelHexInput >> hex;
		byte fileSize = HexReader::byte{ static_cast<long>(hex.currentAddress()) };

		std::cout << "Final address is 0x" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << hex.currentAddress() << std::dec << std::endl;
		std::cout << "File size: " << fileSize << std::endl;
		std::cout << "Max size: " << maxSize << std::endl;
		if (fileSize < maxSize) {
			std::cout << "Fit!" << std::endl;
		}
	} else {
		std::cout << "Failed to open: " << fileLocation << std::endl;
	}
}
