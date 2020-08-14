#ifndef UTILS_NX_HPP
#define UTILS_NX_HPP

#include "nx/Title.hpp"
#include "Types.hpp"
#include "nx/User.hpp"
#include <vector>

// Comparison of AccountUid
bool operator ==(const AccountUid &a, const AccountUid &b);

// A whole heap of utility functions for Switch-related objects/things
namespace Utils::NX {
    // Returns ThemeType matching Horizon's
    ThemeType getHorizonTheme();

    // Returns NX::User* if launched via User Page, nullptr otherwise
    ::NX::User * getUserPageUser();

    // Returns Language matching system language, or default if no language found
    Language getSystemLanguage();

    // Returns vector containing users on the console
    std::vector<::NX::User *> getUserObjects();

    // Returns vector containing ALL played titles
    std::vector<::NX::Title *> getTitleObjects(std::vector<::NX::User *>);

    void startServices();
    void stopServices();
};

#endif