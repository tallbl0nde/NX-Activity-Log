#include "loading.hpp"
#include "SDLHelper.hpp"
#include "theme.hpp"

namespace Screen {
    Loading::Loading(bool * b, char * c) : Screen::Screen(b) {
        // Set Loading message
        this->status = c;
        this->fade = 255;
    }

    void Loading::event() {

    }

    void Loading::update(uint32_t dt) {
        Screen::update(dt);
        this->fade -= 600*(dt/1000.0);
        if (this->fade < 0) {
            this->fade = 0;
        }
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

        // Draw fading overlay
        SDLHelper::setColour(SDL_Color{0, 0, 0, (uint8_t)this->fade});
        SDLHelper::drawRect(0, 0, WIDTH, HEIGHT);
    }

    bool Loading::animDone() {
        if (this->fade <= 0) {
            return true;
        }
        return false;
    }

    void Loading::setTheme(bool b) {
        if (b) {
            this->theme->setDark();
        }
    }

    Loading::~Loading() {

    }
}