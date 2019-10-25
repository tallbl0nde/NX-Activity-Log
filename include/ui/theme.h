#pragma once

struct Theme{
    uint8_t backgroundColour;
    uint8_t lineColour;
    uint8_t textColour;
};

// Light theme
struct Theme theme_light = {
    235,
    0,
    45
};

// Dark theme
struct Theme theme_dark = {
    45,
    164,
    255
};