#ifndef UTILS_UPDATE_HPP
#define UTILS_UPDATE_HPP

#include "Types.hpp"

namespace Utils::Curl {
    void init();
    void exit();

    // Fetch update metadata (version, changelog)
    UpdateData getLatestMetadata();

    // Actually download .nro

};

#endif