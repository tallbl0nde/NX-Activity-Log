#include "screen.hpp"
#include "utils.hpp"

namespace UI {
    // Initalize static variables
    Controls * Screen::controls = new Controls(65, 670, 1150, 50);
    bool Screen::touch_active = true;

    Screen::Screen(bool * b) : Drawable(0, 0, WIDTH, HEIGHT) {
        this->loop = b;
        this->active_element = 0;
    }

    void Screen::update(uint32_t dt) {
        this->theme->animateHighlight(dt);
    }

    ScreenID Screen::change() {
        return S_Nothing;
    }

    Screen::~Screen() {

    }
}