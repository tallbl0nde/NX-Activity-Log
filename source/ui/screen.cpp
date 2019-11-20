#include "screen.hpp"

namespace UI {
    Screen::Screen(bool * b) {
        this->loop = b;
        this->controls = new Controls(1215, 670);
        this->touch_active = false;
        this->active_element = 0;
    }

    Screen::~Screen() {
        delete this->controls;
    }
}