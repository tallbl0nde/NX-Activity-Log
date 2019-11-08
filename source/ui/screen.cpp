#include "screen.hpp"

namespace UI {
    Screen::Screen(bool * b) {
        this->loop = b;
        this->controls = new Controls();
    }

    Screen::~Screen() {
        delete this->controls;
    }
}