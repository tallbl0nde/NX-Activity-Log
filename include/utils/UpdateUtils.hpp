#ifndef UTILS_UPDATE_HPP
#define UTILS_UPDATE_HPP

#include <functional>
#include "Types.hpp"

namespace Utils::Update {
    // Returns true if an update available
    bool available();

    // Query GitHub for latest release
    UpdateData check();

    // Download latest release to /switch/NX-Activity-Log/download.nro
    // Accepts url to release, callback function for progress and return whether successful
    bool download(std::string, std::function<void(long long int, long long int)>);

    // Install (move) update nro (does nothing if .nro not present)
    void install();

    // Returns true if it is time for an update (i.e. not checked today)
    bool needsCheck();
};

#endif