#include "error.hpp"
#include "SDLHelper.hpp"

namespace Screen {
    Error::Error(struct Theme * t, bool * b, std::string msg) : Screen::Screen(t, b) {
        // Set error message
        this->message = msg;

        // Add buttons
        this->controls->add(KEY_PLUS, "Exit", 0);
    }

    void Error::event() {
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed down
                case SDL_JOYBUTTONDOWN:
                    if (events.jbutton.which == 0) {
                        // + button
                        if (events.jbutton.button == Utils::key_map[KEY_PLUS]) {
                            *(this->loop) = false;
                        }
                    }
                    break;
            }
        }
    }

    void Error::update(uint32_t dt) {

    }

    void Error::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->background, this->theme->background, this->theme->background, 255);
        SDLHelper::clearScreen();

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->line, this->theme->line, this->theme->line, 255);
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Print loading title and message
        SDLHelper::setColour(0, 0, 0, 255);
        SDLHelper::drawText("Error", 65, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);
        SDLHelper::drawText(this->message.c_str(), 65, 128, BODY_FONT_SIZE);

        // Draw controls
        this->controls->draw(1215, 670);
    }

    Error::~Error() {

    }
}