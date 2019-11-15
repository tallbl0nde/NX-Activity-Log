#include "screen.hpp"

namespace UI {
    Screen::Screen(bool * b) {
        this->loop = b;
        this->controls = new Controls(1215, 670);
    }

    Screen::~Screen() {
        delete this->controls;
    }
}