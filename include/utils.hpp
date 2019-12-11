#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <string>
#include <switch.h>

// Dimensions of renderer/screen size in pixels
#define WIDTH 1280
#define HEIGHT 720

namespace Utils {
    // Map from HidControllerKeys -> int
    #define KEY_MAP_SIZE 28
    extern std::map<HidControllerKeys, int> key_map;

    // Copy file from source path to dest path
    void copyFile(std::string, std::string);

    // Add commas to provided number
    std::string formatNumberComma(u32);
};

#endif