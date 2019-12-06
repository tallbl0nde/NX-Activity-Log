#include "screen.hpp"
#include "screenmanager.hpp"
#include "utils.hpp"

namespace UI {
    // Initalize static variables
    bool Screen::is_mypage = false;
    SideMenu * Screen::menu = new SideMenu(&ScreenManager::getInstance()->touch_active, 30, 130, 400, 500);

    Screen::Screen() : Drawable(0, 0, WIDTH, HEIGHT) {
        this->active_element = 0;
        this->controls = new Controls(65, 670, 1150, 50);
    }

    void Screen::update(uint32_t dt) {
        this->theme->animateHighlight(dt);
    }

    Screen::~Screen() {
        delete this->controls;
    }
}