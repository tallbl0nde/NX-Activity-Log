#include "screen.hpp"

namespace UI {
    Screen::Screen(struct Theme * t, bool * b) {
        this->theme = t;
        this->loop = b;
    }

    Screen::~Screen() {

    }
}