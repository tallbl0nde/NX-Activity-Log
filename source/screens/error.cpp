#include "error.hpp"
#include "SDLHelper.hpp"
#include "theme.hpp"

namespace Screen {
    Error::Error(bool * b, std::string msg) : Screen::Screen(b) {
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
        SDLHelper::setColour(UI::theme.background);
        SDLHelper::clearScreen();

        // Draw top and bottom lines
        SDLHelper::setColour(UI::theme.foreground);
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Print loading title and message
        SDLHelper::drawText("Error", UI::theme.text, 65, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);
        SDLHelper::drawText(this->message.c_str(), UI::theme.text, 65, 128, BODY_FONT_SIZE);

        // Draw controls
        this->controls->draw();
    }

    Error::~Error() {

    }
}