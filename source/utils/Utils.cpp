#include <fstream>
#include "Utils.hpp"

namespace Utils {
    void copyFile(std::string src, std::string dest) {
        std::ifstream srcF(src, std::ios::binary);
        std::ofstream destF(dest, std::ios::binary);

        destF << srcF.rdbuf();

        srcF.close();
        destF.flush();
        destF.close();
    }

    // Add commas to a number (only does one but shhh)
    std::string formatNumberComma(unsigned int number) {
        std::string s = std::to_string(number);
        if (s.length() > 3) {
            return s.substr(0, s.length() - 3) + "," + s.substr(s.length() - 3, 3);
        }
        return s;
    }
};