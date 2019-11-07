#ifndef THEME_HPP
#define THEME_HPP

#include <SDL2/SDL.h>
#include <cstdint>

// Theme struct declaration
typedef struct {
    SDL_Color background;
    SDL_Color foreground;
    SDL_Color text;
    SDL_Color muted_text;
    SDL_Color accent;
} theme_t;

namespace UI {
    // Variable storing current theme (defined in theme.cpp)
    extern theme_t theme;

    // Light theme
    static theme_t theme_light = {
        SDL_Color{235, 235, 235, 255},
        SDL_Color{45, 45, 45, 255},
        SDL_Color{0, 0, 0, 255},
        SDL_Color{130, 130, 130, 255},
        SDL_Color{50, 80, 240, 255}
    };

    // Dark theme
    static theme_t theme_dark = {
        SDL_Color{45, 45, 45, 255},
        SDL_Color{164, 164, 164, 255},
        SDL_Color{255, 255, 255, 255},
        SDL_Color{160, 160, 160, 255},
        SDL_Color{0, 255, 203, 255}
    };
};

#endif