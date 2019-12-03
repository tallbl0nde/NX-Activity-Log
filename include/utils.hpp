#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <map>
#include <SDL2/SDL.h>
#include <switch.h>

#define WIDTH 1280
#define HEIGHT 720

namespace Utils {
    // Map from HidControllerKeys -> int
    #define KEY_MAP_SIZE 28
    extern std::map<HidControllerKeys, int> key_map;

    // Copy file from source path to dest path
    void copyFile(std::string, std::string);

    // Returns a string with the time since last played
    std::string formatLastPlayed(u32);

    // Add commas to provided number
    std::string formatNumberComma(u32);

    // Nicely format a time with given separator (from zero)
    std::string formatPlaytime(u32, std::string);

    // Nicely format a timestamp
    std::string formatTimestamp(u32);
};

#endif