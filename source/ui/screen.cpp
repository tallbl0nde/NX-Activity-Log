#include "screen.hpp"

namespace UI {
    Screen::Screen(SDL_Renderer * r, struct Theme * t, bool * b) {
        this->renderer = r;
        this->theme = t;
        this->loop = b;
        this->controls = new Controls(r);
    }

    Screen::~Screen() {
        delete this->controls;
    }
}