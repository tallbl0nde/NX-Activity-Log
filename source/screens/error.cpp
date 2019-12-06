#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include "theme.hpp"

namespace Screen {
    Error::Error(std::string msg) : Screen::Screen() {
        // Set error message
        this->message = msg;

        // Add buttons
        this->controls->add(KEY_PLUS, "Exit", 0);
    }

    void Error::event(SDL_Event e) {
        switch (e.type) {
            // Button pressed down
            case SDL_JOYBUTTONDOWN:
                if (e.jbutton.which == 0 || e.jbutton.which == CONTROLS_ID) {
                    // + button
                    if (e.jbutton.button == Utils::key_map[KEY_PLUS]) {
                        ScreenManager::getInstance()->stopLoop();
                    }
                }
                break;

            // Touch (pressed)
            case SDL_FINGERDOWN: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pass event to controls object if below bottom line
                if (y > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }

            // Touch (moved)
            case SDL_FINGERMOTION: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pass event to controls object if was below or originally below line
                if (y > 647 || (HEIGHT * (e.tfinger.y - e.tfinger.dy)) > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }

            // Touch (released)
            case SDL_FINGERUP: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pass event to controls object if below bottom line
                if (y > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }
        }
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