#include "SDLHelper.hpp"
#include "sidemenu.hpp"

// Height of items in list
#define ITEM_HEIGHT 70

namespace UI {
    SideMenu::SideMenu() {
        this->pos = 0;
    }

    void SideMenu::addItem(SideItem * item) {
        this->items.push_back(item);
    }

    void SideMenu::update(uint32_t dt) {

    }

    void SideMenu::draw(int x, int y, int w, int h) {
        // Draw list items
        size_t idx = this->pos;
        if (this->pos > 0) {
            idx--;
        }
        for (size_t i = idx; i < idx + 6; i++) {
            // Break out of loop if going to access outside vector
            if (i >= this->items.size()) {
                break;
            }

            // List items utilize 75% of list width
            this->items[i]->draw(x + w*0.125, y + (ITEM_HEIGHT * (i-pos)), w*0.75, ITEM_HEIGHT);
        }
    }

    size_t SideMenu::getPos() {
        return this->pos;
    }

    void SideMenu::movePos(size_t pos) {
        if (pos < this->items.size()) {
            // Deselect previous item
            this->items[pos]->setSelected(false);
            // Set new pos selected
            this->pos = pos;
            this->items[pos]->setSelected(true);
        }

        if (this->pos > this->items.size()-3) {
            this->pos = this->items.size()-3;
        } else if (this->pos < 0) {
            this->pos = 0;
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