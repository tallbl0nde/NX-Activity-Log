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

    }

    void Loading::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(UI::theme.background);
        SDLHelper::clearScreen();

        // Draw top and bottom lines
        SDLHelper::setColour(UI::theme.foreground);
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Print loading title
        SDLHelper::setColour(UI::theme.text);
        SDLHelper::drawText("Loading...", 65, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);
    }

    Loading::~Loading() {

    }
}