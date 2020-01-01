#include "listitem_statistic.hpp"

// Font size
#define TEXT_SIZE 21
// Pixels to indent text
#define INDENT 16
// Height of box
#define BOX_HEIGHT 60

namespace UI::ListItem {
    Statistic::Statistic(std::string n, std::string v) : List_Item() {
        // Create text textures
        this->name = SDLHelper::renderText(n.c_str(), TEXT_SIZE);
        this->value = SDLHelper::renderText(v.c_str(), TEXT_SIZE);

        // Set height (temporary)
        this->setH(BOX_HEIGHT);

        // Mark unselectable
        this->is_selectable = false;
    }

    void Statistic::draw() {
         // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, this->w, 1);
        SDLHelper::drawRect(this->x, this->y + this->h, this->w, 1);

        // Draw text
        SDLHelper::drawTexture(this->name, this->theme->getText(), this->x + INDENT, this->y + (BOX_HEIGHT-TEXT_SIZE)/2);
        int tw, th;
        SDLHelper::getDimensions(this->value, &tw, &th);
        SDLHelper::drawTexture(this->value, this->theme->getAccent(), this->x + this->w - tw - INDENT, this->y + (BOX_HEIGHT-TEXT_SIZE)/2);
    }

    Statistic::~Statistic() {
        SDLHelper::destroyTexture(this->name);
        SDLHelper::destroyTexture(this->value);
    }
};