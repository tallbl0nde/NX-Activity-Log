#ifndef THEME_H
#define THEME_H

#include <cstdint>

struct Theme{
    uint8_t background;
    uint8_t line;
    uint8_t text;
};

// Light theme
static struct Theme theme_light = {
    235,
    0,
    45
};

// Dark theme
static struct Theme theme_dark = {
    45,
    164,
    255
};

#endif