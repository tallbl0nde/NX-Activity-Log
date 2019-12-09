#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include "utils.hpp"

namespace Screen {
    RecentActivity::RecentActivity(User * u) : Screen::Screen() {
        this->user = u;
        this->total_hours = nullptr;

        // Set controls
        this->controls->add(KEY_A, "OK", 0);
        if (!this->is_mypage) {
            this->controls->add(KEY_PLUS, "Exit", 1);
        }
        this->controls->add(KEY_X, "Popup!", 2);

        // Set active element
        this->active_element = (int)ActiveElement::SideMenu;
        this->menu->setActive(true);
    }

    void RecentActivity::event(SDL_Event e){
        switch (e.type) {
            // Button pressed
            case SDL_JOYBUTTONDOWN:
                if (e.jbutton.which == 0 || e.jbutton.which == 99) {
                    // Plus exits app
                    if (e.jbutton.button == Utils::key_map[KEY_PLUS]) {
                        if (!this->is_mypage) {
                            ScreenManager::getInstance()->stopLoop();
                        }

                    // All other buttons get handled by active element
                    } else {
                        switch (this->active_element) {
                            case (int)ActiveElement::SideMenu:
                                this->menu->handleButton(e.jbutton.button, e.jbutton.state);
                                break;
                        }
                    }
                }
                break;

            case SDL_JOYBUTTONUP:
                if (e.jbutton.which == 0 || e.jbutton.which == 99) {
                    switch (this->active_element) {
                        case (int)ActiveElement::SideMenu:
                            this->menu->handleButton(e.jbutton.button, e.jbutton.state);
                            break;
                    }
                }
                break;

            // Touch (pressed)
            case SDL_FINGERDOWN: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pressed within menu
                if (x >= this->menu->getX() && x <= this->menu->getX() + this->menu->getW() && y >= this->menu->getY() && y <= this->menu->getY() + this->menu->getH()) {
                    this->menu->touched(e.type, x, y);
                    this->active_element = (int)ActiveElement::SideMenu;

                // Pass event to controls object if below bottom line
                } else if (y > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }

            // Touch (moved)
            case SDL_FINGERMOTION: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;
                float dx = WIDTH * e.tfinger.dx;
                float dy = HEIGHT * e.tfinger.dy;

                // Moved after being in menu
                if ((x-dx) >= this->menu->getX() && (x-dx) <= this->menu->getX() + this->menu->getW() && (y-dy) >= this->menu->getY() && (y-dy) <= this->menu->getY() + this->menu->getH()) {
                    this->menu->touched(e.type, x, y);

                } else {
                    // Pass event to controls object if was below or originally below line
                    if (y > 647 || (HEIGHT * (e.tfinger.y - e.tfinger.dy)) > 647) {
                        this->controls->touched(e.type, x, y);
                    }
                }
                break;
            }

            // Touch (released)
            case SDL_FINGERUP: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Released within menu
                if (x >= this->menu->getX() && x <= this->menu->getX() + this->menu->getW() && y >= this->menu->getY() && y <= this->menu->getY() + this->menu->getH()) {
                    this->menu->touched(e.type, x, y);

                } else {
                    // Pass event to controls object if below bottom line
                    if (y > 647) {
                        this->controls->touched(e.type, x, y);
                    }
                }
                break;
            }
        }
    }

    void RecentActivity::update(uint32_t dt) {
        Screen::update(dt);
        this->menu->update(dt);

        if (ScreenManager::getInstance()->touch_active) {
            this->controls->disable(KEY_A);
        } else {
            this->controls->enable(KEY_A);
        }

        // Change screen if menu selected
        switch (this->menu->getSelected()) {
            // Recent Activity (this one so do nothing)
            case 0:
                break;
            // All Activity
            case 1:
                ScreenManager::getInstance()->popScreen();
                break;
            // Settings
            case 2:
                ScreenManager::getInstance()->setScreen(new Settings(this->user));
                break;
        }
    }

    void RecentActivity::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::clearScreen();

        // Draw menu
        this->menu->draw();

        // Different shade behind list
        SDLHelper::setColour(this->theme->getAltBG());
        SDLHelper::drawRect(400, 88, 850, 560);

        // Draw over list to hide scrolling
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::drawRect(430, 0, 780, 87);
        SDLHelper::drawRect(430, 648, 780, 72);

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Draw player icon
        SDLHelper::drawTexture(this->user->getImage(), 65, 14, 60, 60);

        // Print heading
        std::string str = this->user->getUsername() + "'s Activity";
        SDLHelper::drawText(str.c_str(), this->theme->getText(), 150, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);

        // Print total hours
        // int tw, th;
        // SDLHelper::getDimensions(this->total_hours, &tw, &th);
        // SDLHelper::drawTexture(this->total_hours, this->theme->getMutedText(), 1215 - tw, 44 - th/2);

        // Draw controls
        this->controls->draw();
    }

    RecentActivity::~RecentActivity() {

    }
}