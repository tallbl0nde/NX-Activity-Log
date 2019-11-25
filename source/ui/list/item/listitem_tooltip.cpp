#include "listitem_tooltip.hpp"

// Font size
#define FONT_SIZE 16
// x and y padding
#define PADDING 18

namespace UI::ListItem {
    ToolTip::ToolTip(std::string s) : List_Item() {
        this->str = s;
        this->texture = nullptr;
    }

    void ToolTip::draw() {
        if (this->texture != nullptr) {
            SDLHelper::drawTexture(this->texture, this->theme->getMutedText(), this->x + PADDING, this->y + PADDING);
        }
    }

    void ToolTip::setW(int w) {
        this->w = w;

        // Rerender texture
        if (this->texture != nullptr) {
            SDLHelper::destroyTexture(this->texture);
            this->texture = nullptr;
        }
        if (this->str != "") {
            this->texture = SDLHelper::renderTextWrapped(this->str.c_str(), FONT_SIZE, this->w - (PADDING*2));
        }

        // Set height
        int tw, th;
        SDLHelper::getDimensions(this->texture, &tw, &th);
        this->setH(PADDING*2 + th);
    }

    ToolTip::~ToolTip() {
        if (this->texture != nullptr) {
            SDLHelper::destroyTexture(this->texture);
        }
    }
};