#include "list.hpp"
#include "SDLHelper.hpp"

// Height of items in list
#define ITEM_HEIGHT 140

namespace UI {
    List::List() {
        this->pos = 0;
    }

    void List::addItem(ListItem * item) {
        this->items.push_back(item);
    }

    void List::update(uint32_t dt) {

    }

    void List::draw(int x, int y, int w, int h) {
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

            // List items utilize 95% of list width
            this->items[i]->draw(x + w*0.025, y + (ITEM_HEIGHT * (i-pos)), w*0.95, ITEM_HEIGHT);
        }

        // Draw side bar
        SDLHelper::setColour(120, 120, 120, 255);
        int yPos = y + (((float)this->pos / this->items.size()) * (h - 50));
        SDLHelper::drawRect(x + w - 10, yPos, 10, 50);
    }

    size_t List::getPos() {
        return this->pos;
    }

    void List::movePos(size_t pos) {
        this->pos = pos;
        if (this->pos > this->items.size()-3) {
            this->pos = this->items.size()-3;
        } else if (this->pos < 0) {
            this->pos = 0;
        }
    }

    List::~List() {
        // Delete ListItem objects
        while (this->items.size() > 0) {
            delete this->items[0];
            this->items.erase(this->items.begin());
        }
    }
};