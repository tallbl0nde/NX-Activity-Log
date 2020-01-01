#include "listitem_heading.hpp"

// Font size
#define FONT_SIZE 20
// Rectangle dimensions
#define RECT_W 5
#define RECT_H 30
// Pixel gap between text and rectangle
#define PADDING 18

namespace UI::ListItem {
    Heading::Heading(std::string s) : List_Item() {
        // Set height
        this->h = RECT_H;

        this->is_selectable = false;
        this->texture = SDLHelper::renderText(s.c_str(), FONT_SIZE);
    }

    void Heading::draw() {
        // Draw rectangle
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, RECT_W, RECT_H);
        int tw, th;
        SDLHelper::getDimensions(this->texture, &tw, &th);
        SDLHelper::drawTexture(this->texture, this->theme->getText(), this->x + PADDING, this->y + (this->h - th)/2);
    }

    Heading::~Heading() {
        SDLHelper::destroyTexture(this->texture);
    }
};