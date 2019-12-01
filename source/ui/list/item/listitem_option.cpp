#include "listitem_option.hpp"

// Font size
#define TEXT_SIZE 22
// Pixels to indent text
#define INDENT 16
// Height of box
#define BOX_HEIGHT 70

namespace UI::ListItem {
    Option::Option(std::string str, std::string(*f)(bool)) : List_Item() {
        // Create text textures
        this->text = SDLHelper::renderText(str.c_str(), TEXT_SIZE);
        std::string s = (*f)(false);
        this->value = SDLHelper::renderText(s.c_str(), TEXT_SIZE);

        // Store function pointer
        this->selected_func = f;

        // Set height (temporary)
        this->setH(BOX_HEIGHT);

        // Mark selectable
        this->is_selectable = true;
    }

    void Option::draw() {
         // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, this->w, 1);
        SDLHelper::drawRect(this->x, this->y + BOX_HEIGHT - 1, this->w, 1);

        // Draw text
        SDLHelper::drawTexture(this->text, this->theme->getText(), this->x + INDENT, this->y + (BOX_HEIGHT-TEXT_SIZE)/2);
        int tw, th;
        SDLHelper::getDimensions(this->value, &tw, &th);
        SDLHelper::drawTexture(this->value, this->theme->getAccent(), this->x + this->w - tw - INDENT, this->y + (BOX_HEIGHT-TEXT_SIZE)/2);
    }

    void Option::pressed() {
        std::string s = (*this->selected_func)(true);
        this->value = SDLHelper::renderText(s.c_str(), TEXT_SIZE);
    }

    Option::~Option() {
        SDLHelper::destroyTexture(this->text);
        SDLHelper::destroyTexture(this->value);
    }
};