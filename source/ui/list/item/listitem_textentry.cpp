#include "listitem_textentry.hpp"

#define FONT_SIZE 22
// Pixels to indent text
#define INDENT 16
// Height of box
#define BOX_HEIGHT 62

namespace UI::ListItem {
    TextEntry::TextEntry(std::string s, bool tick) {
        this->is_selectable = true;
        this->ticked = tick;
        if (tick) {
            this->tick_tex = SDLHelper::renderCircle((BOX_HEIGHT/2) - INDENT);
        } else {
            this->tick_tex = nullptr;
        }
        this->tex = SDLHelper::renderText(s.c_str(), FONT_SIZE);
        this->setH(BOX_HEIGHT);
    }

    void TextEntry::draw() {
        // Draw lines
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, this->w, 1);
        SDLHelper::drawRect(this->x, this->y + this->h, this->w, 1);

        // Draw text
        int tw, th;
        SDLHelper::getDimensions(this->tex, &tw, &th);
        SDLHelper::drawTexture(this->tex, this->theme->getText(), this->x + INDENT, this->y + (this->h - th)/2);

        // Draw tick
        if (this->ticked) {
            SDLHelper::getDimensions(this->tick_tex, &tw, &th);
            SDLHelper::drawTexture(this->tick_tex, this->theme->getAccent(), this->x + this->w - INDENT - tw, this->y + (this->h - th)/2 + 2);
            std::string str = "\uE14B";
            SDLHelper::drawText(str.c_str(), this->theme->getBG(), this->x + this->w - INDENT - tw + 5, this->y + INDENT/2 + 12, 22, true);
        }
    }

    TextEntry::~TextEntry() {
        SDLHelper::destroyTexture(this->tex);
        if (this->ticked) {
            SDLHelper::destroyTexture(this->tick_tex);
        }
    }
};