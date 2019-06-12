#include <iostream>

namespace utils {
    template<typename T>
    void printPercent(T percent) noexcept {
        constexpr auto loadingChars = 50;
        const auto stringLength = static_cast<std::size_t>((percent / 100) * loadingChars);

        std::cout << "Flashing |";
        std::cout << std::setprecision(2) << std::fixed;
        std::cout << std::string(stringLength, '#');
        if (loadingChars - stringLength > 0) {
            std::cout << std::string(loadingChars - stringLength, ' ');
        }
        std::cout << "| " << percent << "%\r";
    }
}