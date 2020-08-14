#ifndef UTILS_THEME_HPP
#define UTILS_THEME_HPP

#include "Aether/Aether.hpp"

namespace Utils::Theme {
    // Reads theme.ini
    void readIni();

    // Fill variable with values under section
    void readValues(std::string, Aether::Colour &);

    // Write variable to ini section
    void writeValues(std::string, Aether::Colour);

    // Destroys and saves ini object
    void deleteIni();
};

#endif