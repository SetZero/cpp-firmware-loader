//
// Created by sebastian on 05.06.19.
//

#include "HexReader.h"

namespace firmware::reader {
    HexReader::HexReader(const std::string &fileLocation, const HexReader::byte &maxSize) {
        std::ifstream intelHexInput;
        intelHexInput.open(fileLocation, std::ifstream::in);
        if (intelHexInput.good()) {
            intelHexInput >> hex;
            mFileSize = HexReader::byte{static_cast<long>(hex.currentAddress())};
            if (mFileSize > maxSize) {
                std::stringstream ss;
                ss << "Unable to write " << mFileSize << " in the available space of " << maxSize;
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

    void HexReader::writeToStream(serial::DataSendManager &manager) const {
        if(!mCanWrite) return;
        sendMetadata(manager);

        double counter = 0;
        for (/*double counter = 0;*/const auto & data : std::as_const(hex)) {
            counter++;
            auto percent = (counter / mFileSize.count()) * 100;
            utils::printPercent(percent);

            manager.bufferedWrite(static_cast<std::byte>(data.data));
        }
        std::cout << std::endl;
    }

    void HexReader::sendMetadata(serial::DataSendManager& manager) const
    {
        if (HexReader::byte{ utils::byteMaxValue(manager.bytesPerBurst()) } < mFileSize) {
            std::cout << "Can't write filesize within one buffer length!" << std::endl;
            return;
        }
        auto splitValue = utils::splitNumer<std::byte>(static_cast<std::intmax_t>(mFileSize));
        for (std::size_t i = 0; i < manager.bytesPerBurst(); i++) {
            manager.bufferedWrite(splitValue[i]);
        }
    }

    HexReader::operator bool() const noexcept {
        return mCanWrite;
    }

    const std::optional<std::string>& HexReader::errorMessage() const noexcept {
        return mErrorMessage;
    }

    serial::DataSendManager &operator<<(serial::DataSendManager &sender, const HexReader &reader) {
        reader.writeToStream(sender);
        return sender;
    }
}
