#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <switch.h>
#include <vector>
#include "User.hpp"
#include "Title.hpp"

// Dimensions of renderer/screen size in pixels
#define WIDTH 1280
#define HEIGHT 720

// Comparison of AccountUid
bool operator ==(const AccountUid &a, const AccountUid &b);

namespace Utils {
    // Copy file from source path to dest path
    void copyFile(std::string, std::string);

    // Add commas to provided number
    std::string formatNumberComma(u32);

    // Returns vector containing users
    std::vector<User *> getUserObjects();

    // Returns vector containing ALL played titles
    std::vector<Title *> getTitleObjects(std::vector<User *>);

    // Starts services
    void startServices();
    // Stop services
    void stopServices();
};

#endif