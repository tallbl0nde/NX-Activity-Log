#include "sideitem.hpp"
#include "utils.hpp"

// Font sizes
#define FONT_SIZE 22

namespace UI {
    SideItem::SideItem(std::string text) {
        this->text = SDLHelper::renderText(text.c_str(), FONT_SIZE);
        this->selected = false;
    }

    void SideItem::draw() {
        if (this->selected) {
            SDLHelper::setColour(this->theme->getAccent());
            SDLHelper::drawRect(this->x + 8, this->y + 9, 4, 52);
            SDLHelper::drawTexture(this->text, this->theme->getAccent(), this->x + 24, this->y + (this->h/2) - (FONT_SIZE/2));
        } else {
            SDLHelper::drawTexture(this->text, this->theme->getText(), this->x + 24, this->y + (this->h/2) - (FONT_SIZE/2));
        }
    }

    bool SideItem::getSelected() {
        return this->selected;
    }

    void SideItem::setSelected(bool b) {
        this->selected = b;
    }

    SideItem::~SideItem() {
        // Destroy created textures
        SDLHelper::destroyTexture(this->text);
    }
};