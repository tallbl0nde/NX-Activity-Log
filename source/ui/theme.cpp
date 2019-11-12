#include "theme.hpp"

namespace UI {
    // Theme variable
    theme_t theme;

    theme_t theme_light = {
        SDL_Color{235, 235, 235, 255},
        SDL_Color{231, 231, 231, 255},
        SDL_Color{45, 45, 45, 255},
        SDL_Color{0, 0, 0, 255},
        SDL_Color{130, 130, 130, 255},
        SDL_Color{200, 200, 200, 255},
        SDL_Color{50, 80, 240, 255}
    };

    // Dark theme
    theme_t theme_dark = {
        SDL_Color{45, 45, 45, 255},
        SDL_Color{49, 49, 49, 255},
        SDL_Color{164, 164, 164, 255},
        SDL_Color{255, 255, 255, 255},
        SDL_Color{160, 160, 160, 255},
        SDL_Color{100, 100, 100, 255},
        SDL_Color{0, 255, 203, 255}
    };
};