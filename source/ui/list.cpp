#include "list.hpp"

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

    void List::draw(SDL_Renderer * renderer, int x, int y, int w, int h) {
        // Draw list items
        for (size_t i = 0; i < this->items.size(); i++) {
            // List items utilize 95% of list width
            this->items[i]->draw(renderer, x + w*0.05, y + (ITEM_HEIGHT * (i-pos)), w*0.95, ITEM_HEIGHT);
        }
    }

    size_t List::getPos() {
        return this->pos;
    }

    void List::movePos(size_t pos) {
        this->pos = pos;
        if (this->pos > this->items.size()) {
            this->pos = this->items.size();
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