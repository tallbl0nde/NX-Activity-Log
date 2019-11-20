#include "loading.hpp"
#include "SDLHelper.hpp"
#include "theme.hpp"

namespace Screen {
    Loading::Loading(bool * b, char * c) : Screen::Screen(b) {
        // Set Loading message
        this->status = c;
    }

    void Loading::event() {

    }

    void Loading::update(uint32_t dt) {
        Screen::update(dt);
    }

    void Loading::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::clearScreen();

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Print loading title
        SDLHelper::drawText("Loading...", this->theme->getText(), 65, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);
    }

    void Loading::setTheme(bool b) {
        if (b) {
            this->theme->setDark();
        }
    }

    Loading::~Loading() {

    }
}