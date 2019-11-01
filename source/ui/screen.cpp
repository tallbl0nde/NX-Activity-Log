#include "screen.hpp"

namespace UI {
    Screen::Screen(struct Theme * t, bool * b) {
        this->theme = t;
        this->loop = b;
        this->controls = new Controls();
    }

    Screen::~Screen() {
        delete this->controls;
    }
}