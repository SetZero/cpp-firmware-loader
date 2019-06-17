//
// Created by sebastian on 05.06.19.
//

#pragma once


#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
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

        [[nodiscard]] constexpr auto getStartAddress() const noexcept { return mStartAddress; }

        void writeToStream(serial::DataSendManager &manager) const;

        friend serial::DataSendManager& operator<<(serial::DataSendManager& sender, const HexReader& reader);
    private:

        void sendMetadata(serial::DataSendManager& manager) const;

        template<typename T>
#ifdef __cpp_concepts
        requires std::is_arithmetic_v<T>
#endif
        void sendNumericValue(firmware::serial::DataSendManager& manager, const T& value) const {
            auto splitValue = utils::splitNumer<std::byte>(value);
            //TODO: if gcc supports it use: std::for_each_n
            /*std::for_each_n(std::begin(splitValue), manager.bytesPerBurst(), [&](auto& element) {
                    manager.bufferedWrite(element);
                });*/
            for(std::size_t i=0; i < std::min(manager.metadataSize(), splitValue.size()); i++) {
                manager.metadataWrite(splitValue[i]);
            }
            for (std::intmax_t i = 0; i < std::max(static_cast < std::intmax_t>(0), static_cast<std::intmax_t>(static_cast<std::intmax_t>(manager.metadataSize()) - static_cast<std::intmax_t>(splitValue.size()))); i++) {
                manager.metadataWrite(std::byte(0x00));
            }
        }

        intelhex hex;
        bool mCanWrite{ false };
        std::optional<std::string> mErrorMessage{ std::nullopt };
        byte mFileSize{ 0 };
        std::size_t mStartAddress{ 0 };
    };

}
