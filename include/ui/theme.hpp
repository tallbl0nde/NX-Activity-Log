#ifndef THEME_HPP
#define THEME_HPP

#include <SDL2/SDL.h>
#include <cstdint>

// Theme struct declaration
typedef struct {
    SDL_Color background;
    SDL_Color alt_background;
    SDL_Color foreground;
    SDL_Color text;
    SDL_Color muted_text;
    SDL_Color muted_line;
    SDL_Color accent;
} theme_t;

namespace UI {
    // Variable storing current theme (defined in theme.cpp)
    extern theme_t theme;

    // Light theme
    extern theme_t theme_light;

    // Dark theme
    extern theme_t theme_dark;
};

#endif