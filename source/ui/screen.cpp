#include "screen.hpp"

namespace UI {
    Screen::Screen(SDL_Renderer * r, struct Theme * t, bool * b) {
        this->renderer = r;
        this->theme = t;
        this->loop = b;
        this->controls = new Controls(r);

        // Create fonts
        PlFontData fontData;
        plGetSharedFontByType(&fontData, PlSharedFontType_Standard);
        this->heading = TTF_OpenFontRW(SDL_RWFromMem(fontData.address, fontData.size), 1, HEADING_FONT_SIZE);
        if (!this->heading) {
            // Handle error
        }
        this->body = TTF_OpenFontRW(SDL_RWFromMem(fontData.address, fontData.size), 1, BODY_FONT_SIZE);
        if (!this->body) {
            // Handle error
        }
    }

    Screen::~Screen() {
        delete this->controls;
    }
}