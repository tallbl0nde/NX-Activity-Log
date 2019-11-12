#include "SDLHelper.hpp"
#include "sideitem.hpp"
#include "theme.hpp"
#include "utils.hpp"

// Font sizes
#define FONT_SIZE 22

namespace UI {
    SideItem::SideItem(std::string text) {
        this->text = SDLHelper::renderText(text.c_str(), FONT_SIZE);
        this->selected = false;
    }

    void SideItem::update(uint32_t dt) {

    }

    void SideItem::draw(int x, int y, int w, int h) {
        if (this->selected) {
            SDLHelper::setColour(UI::theme.accent);
            SDLHelper::drawRect(x + 8, y + 9, 4, 52);
            SDLHelper::drawTexture(this->text, UI::theme.accent, x + 24, y + (h/2) - (FONT_SIZE/2));
        } else {
            SDLHelper::drawTexture(this->text, UI::theme.text, x + 24, y + (h/2) - (FONT_SIZE/2));
        }
    }

    void SideItem::setSelected(bool b) {
        this->selected = b;
    }

    SideItem::~SideItem() {
        // Destroy created textures
        SDLHelper::destroyTexture(this->text);
    }
};