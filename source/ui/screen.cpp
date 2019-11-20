#include "screen.hpp"
#include "utils.hpp"

namespace UI {
    Screen::Screen(bool * b) : Drawable(0, 0, WIDTH, HEIGHT) {
        this->loop = b;
        this->controls = new Controls(65, 670, 1150, 50);
        this->touch_active = false;
        this->active_element = 0;
    }

    void Screen::update(uint32_t dt) {
        this->theme->animateHighlight(dt);
    }

    Screen::~Screen() {
        delete this->controls;
    }
}