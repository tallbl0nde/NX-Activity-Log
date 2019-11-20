#include "error.hpp"
#include "SDLHelper.hpp"
#include "theme.hpp"

namespace Screen {
    Error::Error(bool * b, std::string msg) : Screen::Screen(b) {
        // Set error message
        this->message = msg;

        // Add buttons
        this->controls->reset();
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

                // Touch (pressed)
                case SDL_FINGERDOWN: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pass event to controls object if below bottom line
                    if (y > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }

                // Touch (moved)
                case SDL_FINGERMOTION: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pass event to controls object if was below or originally below line
                    if (y > 647 || (HEIGHT * (events.tfinger.y - events.tfinger.dy)) > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }

                // Touch (released)
                case SDL_FINGERUP: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pass event to controls object if below bottom line
                    if (y > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }
            }
        }
    }

    void Error::update(uint32_t dt) {
        Screen::update(dt);
    }

    void Error::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::clearScreen();

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Print loading title and message
        SDLHelper::drawText("Error", this->theme->getText(), 65, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);
        SDLHelper::drawText(this->message.c_str(), this->theme->getText(), 65, 128, BODY_FONT_SIZE);

        // Draw controls
        this->controls->draw();
    }

    Error::~Error() {

    }
}