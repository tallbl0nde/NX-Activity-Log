#include "navigable.hpp"

namespace UI {
    Navigable::Navigable(bool * b, int x, int y, int w, int h) : Drawable(x, y, w, h) {
        this->touch_active = b;
        this->setActive(false);
    }

    void Navigable::update(uint32_t dt) {
        // Iterate over array and increment time held if pressed
        for (size_t i = 0; i < KEY_MAP_SIZE; i++) {
            if (this->button[i].is_pressed) {
                this->button[i].time_held += dt;
            }
        }
    }

    void Navigable::handleButton(uint8_t button, uint8_t state) {
        if (state == SDL_PRESSED) {
            this->button[button].is_pressed = true;
            this->button[button].time_held = 0;
        } else if (state == SDL_RELEASED) {
            this->button[button].is_pressed = false;
        }
    }

    void Navigable::setActive(bool b) {
        // Reset/initialize button array
        if (!b) {
            for (size_t i = 0; i < KEY_MAP_SIZE; i++) {
                struct ButtonInfo s;
                s.is_pressed = false;
                s.time_held = 0;
                this->button[i] = s;
            }
        }
        this->is_active = b;
    }

    Navigable::~Navigable() {

    }
};