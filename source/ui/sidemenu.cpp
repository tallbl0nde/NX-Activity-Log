#include "SDLHelper.hpp"
#include "sidemenu.hpp"
#include "ui/theme.hpp"

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

    void SideMenu::draw(int x, int y, int w, int h) {
        // Draw list items
        for (size_t i = 0; i < this->items.size(); i++) {
            // Show highlighting box if currently selected
            if (this->pos == i) {
                SDLHelper::setColour(UI::theme.highlight);
                SDLHelper::drawRect((x + w*0.125) - 5, y + (ITEM_HEIGHT * i) - 5, w*0.72 + 10, ITEM_HEIGHT + 10);
                SDLHelper::setColour(UI::theme.highlight_bg);
                SDLHelper::drawRect(x + w*0.125, y + (ITEM_HEIGHT * i), w*0.72, ITEM_HEIGHT);
            }

            // List items utilize 72% of list width
            this->items[i]->draw(x + w*0.125, y + (ITEM_HEIGHT * i), w*0.72, ITEM_HEIGHT);
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