#include "loading.hpp"
#include "SDLHelper.hpp"

namespace Screen {
    Loading::Loading(struct Theme * t, bool * b, char * c) : Screen::Screen(t, b) {
        // Set Loading message
        this->status = c;
    }

    void Loading::event() {

    }

    void Loading::update(uint32_t dt) {

    }

    void Loading::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->background, this->theme->background, this->theme->background, 255);
        SDLHelper::clearScreen();

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->line, this->theme->line, this->theme->line, 255);
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Print loading title
        SDLHelper::drawText("Loading...", 65, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);
    }

    Loading::~Loading() {

    }
}