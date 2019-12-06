#include "listitem_textentry.hpp"

#define FONT_SIZE 22
// Pixels to indent text
#define INDENT 16
// Height of box
#define BOX_HEIGHT 62

namespace UI::ListItem {
    TextEntry::TextEntry(std::string s) {
        this->is_selectable = true;
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
    }

    TextEntry::~TextEntry() {
        SDLHelper::destroyTexture(this->tex);
    }
};