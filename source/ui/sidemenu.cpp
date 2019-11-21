#include "sidemenu.hpp"

#define NO_BUTTON 200
// Height of items in list
#define ITEM_HEIGHT 70
// Delay (in ms) between moving selection when held
#define MOVE_DELAY 150
// Initial held down delay (in ms)
#define HOLD_DELAY 500

namespace UI {
    SideMenu::SideMenu(bool * b, int x, int y, int w, int h) : Navigable(b, x, y, w, h) {
        this->t_pos = -1;
        this->pos = 0;
    }

    void SideMenu::addItem(SideItem * item) {
        item->setX(this->x + this->w*0.125);
        item->setY(this->y + (ITEM_HEIGHT * this->items.size()));
        item->setW(this->w*0.72);
        item->setH(ITEM_HEIGHT);
        this->items.push_back(item);

        if (this->items.size() == 1) {
            this->items[0]->setSelected(true);
        }
    }

    void SideMenu::handleButton(uint8_t button, uint8_t state) {
        Navigable::handleButton(button, state);

        // Button pressed
        if (state == SDL_PRESSED) {
            // Move selector down one
            if (button == Utils::key_map[KEY_DDOWN]) {
                this->setPos(this->pos + 1);

            // Move selector up one
            } else if (button == Utils::key_map[KEY_DUP]) {
                this->setPos(this->pos - 1);

            // Mark current pos as selected (will be changed)
            } else if (button == Utils::key_map[KEY_A]) {
                for (size_t i = 0; i < this->items.size(); i++) {
                    this->items[i]->setSelected(false);
                }
                this->items[this->pos]->setSelected(true);
            }
        }
    }

    void SideMenu::touched(uint32_t type, float tx, float ty) {
        switch (type) {
            case SDL_FINGERDOWN:
                // Loop over shown controls and set touched if so
                for (size_t i = 0; i < this->items.size(); i++) {
                    if (tx >= this->items[i]->getX() && tx <= this->items[i]->getX() + this->items[i]->getW() && ty >= this->items[i]->getY() && ty <= this->items[i]->getY() + this->items[i]->getH()) {
                        this->t_pos = i;
                        break;
                    }
                }
                break;

            case SDL_FINGERMOTION:
                if (this->t_pos == -1){
                    return;
                }

                // If moved outside of selected button treat as lifted finger
                if (tx < this->items[this->t_pos]->getX() || tx > this->items[this->t_pos]->getX() + this->items[this->t_pos]->getW() || ty < this->items[this->t_pos]->getY() || ty > this->items[this->t_pos]->getY() + this->items[this->t_pos]->getH()) {
                    this->touched(SDL_FINGERUP, tx, ty);
                }
                break;

            case SDL_FINGERUP:
                if (this->t_pos == -1){
                    return;
                }

                // If inside of same button set selected
                if (tx >= this->items[this->t_pos]->getX() || tx <= this->items[this->t_pos]->getX() + this->items[this->t_pos]->getW() || ty >= this->items[this->t_pos]->getY() || ty <= this->items[this->t_pos]->getY() + this->items[this->t_pos]->getH()) {
                    for (size_t i = 0; i < this->items.size(); i++) {
                        this->items[i]->setSelected(false);
                    }
                    this->pos = this->t_pos;
                    this->items[this->pos]->setSelected(true);
                }
                this->t_pos = -1;
                break;
        }
    }

    void SideMenu::update(uint32_t dt) {
        Navigable::update(dt);

        if (this->button[Utils::key_map[KEY_DDOWN]].is_pressed && this->button[Utils::key_map[KEY_DDOWN]].time_held > HOLD_DELAY) {
            this->setPos(this->pos + 1);
            this->button[Utils::key_map[KEY_DDOWN]].time_held -= MOVE_DELAY;
        } else if (this->button[Utils::key_map[KEY_DUP]].is_pressed && this->button[Utils::key_map[KEY_DUP]].time_held > HOLD_DELAY) {
            this->setPos(this->pos - 1);
            this->button[Utils::key_map[KEY_DUP]].time_held -= MOVE_DELAY;
        }
    }

    void SideMenu::draw() {
        // Draw list items
        for (size_t i = 0; i < this->items.size(); i++) {
            // Show highlighting box if currently selected
            if (!(*this->touch_active) && this->pos == i && this->is_active) {
                SDLHelper::setColour(this->theme->getHighlight());
                SDLHelper::drawRect(this->items[i]->getX() - 5, this->items[i]->getY() - 5, this->items[i]->getW() + 10, this->items[i]->getH() + 10);
                SDLHelper::setColour(this->theme->getHighlightBG());
                SDLHelper::drawRect(this->items[i]->getX(), this->items[i]->getY(), this->items[i]->getW(), this->items[i]->getH());
            }

            // List items utilize 72% of list width
            this->items[i]->draw();

            // Draw touch overlay if touched
            if (this->t_pos == (int)i) {
                SDLHelper::setColour(this->theme->getTouchOverlay());
                SDLHelper::drawRect(this->items[i]->getX(), this->items[i]->getY(), this->items[i]->getW(), this->items[i]->getH());
            }
        }
    }

    void SideMenu::setPos(size_t pos) {
        if (pos < this->items.size() && pos >= 0) {
            this->pos = pos;
        }
    }

    SideMenu::~SideMenu() {
        // Delete ListItem objects
        while (this->items.size() > 0) {
            delete this->items[0];
            this->items.erase(this->items.begin());
        }
    }
};