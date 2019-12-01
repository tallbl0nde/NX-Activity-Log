#include "listitem_user.hpp"
#include "utils.hpp"

// Font size
#define FONT_SIZE 24

// Height of items
#define ITEM_HEIGHT 90

namespace UI::ListItem {
    User::User(SDL_Texture * tex, std::string str) : List_Item() {
        this->setH(ITEM_HEIGHT);
        this->icon = tex;
        this->name = SDLHelper::renderText(str.c_str(), FONT_SIZE);
        this->is_selectable = true;
    }

    void User::draw() {
        // Draw outlines
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, this->w, 1);
        SDLHelper::drawRect(this->x, this->y + this->h, this->w, 1);

        // Draw icon
        SDLHelper::setColour(SDL_Color{255, 255, 255, 255});
        SDLHelper::drawTexture(this->icon, this->x + 10, this->y + 10, this->h - 20, this->h - 20);

        // Print text
        int tw, th;
        SDLHelper::getDimensions(this->name, &tw, &th);
        SDLHelper::drawTexture(this->name, this->theme->getText(), this->x + ITEM_HEIGHT + 15, this->y + (this->h - th)/2);
    }

    User::~User() {
        // Destroy created texture
        SDLHelper::destroyTexture(this->name);
    }
};