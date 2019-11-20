#ifndef THEME_HPP
#define THEME_HPP

#include <cmath>
#include <cstdint>
#include <SDL2/SDL.h>

// Theme struct declaration
#define ANIM_SPEED 1.8
typedef struct {
    bool is_light;
    SDL_Color background;
    SDL_Color alt_background;
    SDL_Color foreground;
    SDL_Color text;
    SDL_Color muted_text;
    SDL_Color muted_line;
    SDL_Color accent;
    SDL_Color highlight_bg;
    SDL_Color highlight;

    // Highlight animation stuff
    float hi_time = 0;
    void animateHighlight(uint32_t dt) {
        hi_time += (ANIM_SPEED*(dt/1000.0)) * M_PI;
        if (hi_time > 2 * M_PI) {
            hi_time -= 2 * M_PI;
        }
        if (is_light) {
            highlight.g = 225 + (25 * sin(hi_time));
            highlight.b = 203 + (13 * sin(hi_time));
        } else {
            highlight.g = 200 + (50 * sin(hi_time));
            highlight.b = 220 + (30 * sin(hi_time));
        }
    }
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