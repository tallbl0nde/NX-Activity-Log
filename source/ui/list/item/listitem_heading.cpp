#include "listitem_heading.hpp"

// Font size
#define FONT_SIZE 20
// Font size for (?)
#define ICON_FONT_SIZE 30
// Rectangle dimensions
#define RECT_W 5
#define RECT_H 30
// Pixel gap between text and rectangle
#define PADDING 18

namespace UI::ListItem {
    Heading::Heading(std::string s, std::function<void()> c) : List_Item() {
        this->callback = c;

        // Set height
        this->h = RECT_H;

        // If function passed activate help icon
        this->help = nullptr;
        this->is_selectable = false;
        if (this->callback != nullptr) {
            this->help = SDLHelper::renderText("\uE142", ICON_FONT_SIZE, true);
            this->auto_highlight = false;
            this->is_selectable = true;
        }

        this->texture = SDLHelper::renderText(s.c_str(), FONT_SIZE);
    }

    void Heading::draw() {
        // Draw rectangle
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, RECT_W, RECT_H);
        int tw, th;
        SDLHelper::getDimensions(this->texture, &tw, &th);
        SDLHelper::drawTexture(this->texture, this->theme->getText(), this->x + PADDING, this->y + (this->h - th)/2);

        // Draw help (if relevant)
        if (this->help != nullptr) {
            SDLHelper::getDimensions(this->help, &tw, &th);
            if (this->is_selected) {
                SDL_Texture * t = SDLHelper::renderCircle((tw + 10)/2);
                SDLHelper::drawTexture(t, this->theme->getHighlight(), this->x + this->w - PADDING - tw - 5, this->y + (this->h - th)/2 - 5);
                SDLHelper::destroyTexture(t);
                t = SDLHelper::renderCircle(tw/2);
                SDLHelper::drawTexture(t, this->theme->getHighlightBG(), this->x + this->w - PADDING - tw, this->y + (this->h - th)/2);
                SDLHelper::destroyTexture(t);
            }
            SDLHelper::drawTexture(this->help, this->theme->getMutedText(), this->x + this->w - PADDING - tw, this->y + (this->h - th)/2);
        }
    }

    void Heading::pressed() {
        this->callback();
    }

    Heading::~Heading() {
        if (this->help != nullptr) {
            SDLHelper::destroyTexture(this->help);
        }
        SDLHelper::destroyTexture(this->texture);
    }
};