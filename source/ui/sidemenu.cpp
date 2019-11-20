#include "sidemenu.hpp"

// Height of items in list
#define ITEM_HEIGHT 70

namespace UI {
    SideMenu::SideMenu(int x, int y, int w, int h) : Drawable(x, y, w, h) {
        this->pos = 0;
    }

    void SideMenu::addItem(SideItem * item) {
        item->setX(this->x + this->w*0.125);
        item->setY(this->y + (ITEM_HEIGHT * this->items.size()));
        item->setW(this->w*0.72);
        item->setH(ITEM_HEIGHT);
        this->items.push_back(item);
    }

    void SideMenu::button(uint8_t button, uint8_t state) {
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

        // Button released
        } else if (state == SDL_RELEASED) {

        }
    }

    void SideMenu::draw() {
        // Draw list items
        for (size_t i = 0; i < this->items.size(); i++) {
            // Show highlighting box if currently selected
            if (this->pos == i) {
                SDLHelper::setColour(this->theme->getHighlight());
                SDLHelper::drawRect(this->items[i]->getX() - 5, this->items[i]->getY() - 5, this->items[i]->getW() + 10, this->items[i]->getH() + 10);
                SDLHelper::setColour(this->theme->getHighlightBG());
                SDLHelper::drawRect(this->items[i]->getX(), this->items[i]->getY(), this->items[i]->getW(), this->items[i]->getH());
            }

            // List items utilize 72% of list width
            this->items[i]->draw();
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