#include <cmath>
#include "theme.hpp"

// Animation speed factor for highlighting
#define ANIM_SPEED 1.8

namespace UI {
    Theme::Theme() {
        this->setLight();
        hi_sin_time = 0;
    }

    void Theme::setLight() {
        this->type = T_Light;
        this->accent = SDL_Color{50, 80, 240, 255};
        this->alt_background = SDL_Color{231, 231, 231, 255};
        this->background = SDL_Color{235, 235, 235, 255};
        this->foreground = SDL_Color{45, 45, 45, 255};
        this->highlight = SDL_Color{0, 0, 0, 255};
        this->highlight_bg = SDL_Color{253, 253, 253, 255};
        this->muted_line = SDL_Color{200, 200, 200, 255};
        this->muted_text = SDL_Color{130, 130, 130, 255};
        this->text = SDL_Color{0, 0, 0, 255};
    }

    void Theme::setDark() {
        this->type = T_Dark;
        this->accent = SDL_Color{0, 255, 203, 255};
        this->alt_background = SDL_Color{49, 49, 49, 255};
        this->background = SDL_Color{45, 45, 45, 255};
        this->foreground = SDL_Color{164, 164, 164, 255};
        this->highlight = SDL_Color{0, 0, 255, 255};
        this->highlight_bg = SDL_Color{35, 35, 40, 255};
        this->muted_line = SDL_Color{100, 100, 100, 255};
        this->muted_text = SDL_Color{160, 160, 160, 255};
        this->text = SDL_Color{255, 255, 255, 255};
    }

    void Theme::animateHighlight(uint32_t dt) {
        this->hi_sin_time += (ANIM_SPEED*(dt/1000.0)) * M_PI;
        if (this->hi_sin_time > 2 * M_PI) {
            this->hi_sin_time -= 2 * M_PI;
        }
        if (this->type == T_Light) {
            this->highlight.g = 225 + (25 * sin(this->hi_sin_time));
            this->highlight.b = 203 + (13 * sin(this->hi_sin_time));
        } else {
            this->highlight.g = 200 + (50 * sin(this->hi_sin_time));
            this->highlight.b = 220 + (30 * sin(this->hi_sin_time));
        }
    }

    ThemeType Theme::getType() {
        return this->type;
    }

    SDL_Color Theme::getAccent() {
        return this->accent;
    }

    SDL_Color Theme::getAltBG() {
        return this->alt_background;
    }

    SDL_Color Theme::getBG() {
        return this->background;
    }

    SDL_Color Theme::getFG() {
        return this->foreground;
    }

    SDL_Color Theme::getHighlight() {
        return this->highlight;
    }

    SDL_Color Theme::getHighlightBG() {
        return this->highlight_bg;
    }

    SDL_Color Theme::getMutedLine() {
        return this->muted_line;
    }

    SDL_Color Theme::getMutedText() {
        return this->muted_text;
    }

    SDL_Color Theme::getText() {
        return this->text;
    }

    Theme::~Theme() {

    }
};