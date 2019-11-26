#include "screen.hpp"
#include "utils.hpp"

namespace UI {
    // Initalize static variables
    bool Screen::touch_active = true;

    Screen::Screen() : Drawable(0, 0, WIDTH, HEIGHT) {
        this->active_element = 0;
        this->controls = new Controls(65, 670, 1150, 50);
    }

    void Screen::update(uint32_t dt) {
        this->theme->animateHighlight(dt);
    }

    Screen::~Screen() {

    }
}