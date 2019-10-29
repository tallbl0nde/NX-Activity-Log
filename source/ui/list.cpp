#include "list.hpp"

namespace UI {
    List::List() {

    }

    void List::addItem(ListItem * item) {
        this->items.push_back(item);
    }

    void List::update(uint32_t dt) {

    }

    void List::draw(SDL_Renderer * renderer, int x, int y, int w, int h) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect rr = {x, y, w, h};
        SDL_RenderFillRect(renderer, &rr);

        for (size_t i = 0; i < this->items.size(); i++) {
            // List items utilize 90% of list width
            this->items[i]->draw(renderer, x*0.1, y + (140 * i), w*0.9, 130);
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