#ifndef UTILS_NX_HPP
#define UTILS_NX_HPP

#include "Title.hpp"
#include "Types.hpp"
#include "User.hpp"
#include <vector>

// Comparison of AccountUid
bool operator ==(const AccountUid &a, const AccountUid &b);

// A whole heap of utility functions for Switch-related objects/things
namespace Utils::NX {
    // Returns ThemeType matching Horizon's
    ThemeType getHorizonTheme();

    // Returns vector containing users on the console
    std::vector<::NX::User *> getUserObjects();

    // Returns vector containing ALL played titles
    std::vector<::NX::Title *> getTitleObjects(std::vector<::NX::User *>);

    void startServices();
    void stopServices();
};

#endif