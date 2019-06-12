//
// Created by sebastian on 05.06.19.
//

#pragma once


#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "../../includes/intelhexclass.h"
#include "../json/ConfigManager.h"
#include "../units/Byte.h"
#include "../utils/utils.h"
#include "../utils/printUtils.h"
#include "DataSendManager.h"

namespace firmware::reader {
    class HexReader {
    public:
        using byte = CustomDataTypes::ComputerScience::byte;

        HexReader(const std::string &fileLocation, const byte &maxSize);

        explicit operator bool() const noexcept;

        [[nodiscard]] const std::optional<std::string>& errorMessage() const noexcept;

        [[nodiscard]] constexpr byte getFileSize() const noexcept { return mFileSize; }

        void writeToStream(serial::DataSendManager &manager) const;

        friend serial::DataSendManager& operator<<(serial::DataSendManager& sender, const HexReader& reader);
    private:

        void sendMetadata(serial::DataSendManager& manager) const;

        intelhex hex;
        bool mCanWrite{ false };
        std::optional<std::string> mErrorMessage{ std::nullopt };
        byte mFileSize{ 0 };
    };

}
