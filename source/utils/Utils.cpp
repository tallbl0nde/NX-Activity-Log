#include <cmath>
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

    // Round a double to given decimal places
    double roundToDecimalPlace(double val, unsigned int p) {
        for (unsigned int i = 0; i < p; i++) {
            val *= 10.0;
        }
        val = std::round(val);
        for (unsigned int i = 0; i < p; i++) {
            val /= 10.0;
        }
        return val;
    }

    std::string truncateToDecimalPlace(std::string str, unsigned int p) {
        size_t dec = str.find(".");
        if (dec == std::string::npos || p >= str.length() - dec) {
            return str;
        }

        // Cut off decimal place if zero
        if (p == 0) {
            dec--;
        }

        return str.substr(0, dec + 1 + p);
    }
};