#include "settinglistitem.hpp"

// Font sizes
#define TIP_SIZE 16
#define TEXT_SIZE 22

// Pixels to indent text
#define INDENT 16
#define INDENT_TIP 22
// Height of box
#define BOX_HEIGHT 70
// Gap between box and tip
#define TIP_GAP 14

namespace UI {
    SettingListItem::SettingListItem(std::string t, std::string (*f)(bool), std::string hi) : Drawable() {
        // Create text textures
        this->text = SDLHelper::renderText(t.c_str(), TEXT_SIZE);
        this->tip = nullptr;
        this->tip_text = hi;

        std::string s = (*f)(false);
        this->value = SDLHelper::renderText(s.c_str(), TEXT_SIZE);

        // Store function pointer
        this->selected_func = f;

        // Set height (temporary)
        this->setH(BOX_HEIGHT + TIP_GAP);
    }

    void SettingListItem::pressed() {
        std::string s = (*this->selected_func)(true);
        this->value = SDLHelper::renderText(s.c_str(), TEXT_SIZE);
    }

    void SettingListItem::draw() {
        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, this->w, 1);
        SDLHelper::drawRect(this->x, this->y + BOX_HEIGHT - 1, this->w, 1);

        // Draw text
        SDLHelper::drawTexture(this->text, this->theme->getText(), this->x + INDENT, this->y + (BOX_HEIGHT-TEXT_SIZE)/2);
        int tw, th;
        SDLHelper::getDimensions(this->value, &tw, &th);
        SDLHelper::drawTexture(this->value, this->theme->getAccent(), this->x + this->w - tw - INDENT, this->y + (BOX_HEIGHT-TEXT_SIZE)/2);

        // Draw hint
        if (this->tip != nullptr) {
            SDLHelper::drawTexture(this->tip, this->theme->getMutedText(), this->x + INDENT_TIP, this->y + BOX_HEIGHT + TIP_GAP);
        }
    }

    void SettingListItem::renderTip() {
        if (this->tip_text != "") {
            this->tip = SDLHelper::renderTextWrapped(this->tip_text.c_str(), TIP_SIZE, this->w - (INDENT_TIP*2));
        }

        // Set height
        int tw, th;
        SDLHelper::getDimensions(this->tip, &tw, &th);
        this->setH(BOX_HEIGHT + TIP_GAP + th);
    }

    unsigned int SettingListItem::getBoxHeight() {
        return BOX_HEIGHT;
    }

    SettingListItem::~SettingListItem() {
        SDLHelper::destroyTexture(this->text);
        SDLHelper::destroyTexture(this->value);
        if (this->tip != nullptr) {
            SDLHelper::destroyTexture(this->tip);
        }
    }
};