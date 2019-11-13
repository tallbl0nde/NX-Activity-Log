#include <algorithm>
#include "listitem.hpp"
#include "SDLHelper.hpp"
#include "theme.hpp"
#include "utils.hpp"

// Font sizes
#define TITLE_FONT_SIZE 22
#define SUB_FONT_SIZE 18

namespace UI {
    ListItem::ListItem(Title * t) {
        // Create textures using title object
        this->icon = t->getIcon();
        this->title = SDLHelper::renderText(t->getName().c_str(), TITLE_FONT_SIZE);
        std::string str = "Played for " + Utils::formatPlaytime(t->getPlaytime());
        this->playtime = SDLHelper::renderText(str.c_str(), SUB_FONT_SIZE);
        this->lastplayed = SDLHelper::renderText(Utils::formatLastPlayed(t->getLastPlayed()).c_str(), SUB_FONT_SIZE);

        this->rank = nullptr;
        this->title_obj = t;
        this->selected = false;
    }

    void ListItem::update(uint32_t dt) {

    }

    void ListItem::draw(int x, int y, int w, int h) {
        // Draw background
        if (this->selected) {
            SDLHelper::drawRect(x, y, w, h);
        }

        // Draw outlines
        SDLHelper::setColour(UI::theme.muted_line);
        SDLHelper::drawRect(x, y, w, 1);
        SDLHelper::drawRect(x, y + h, w, 1);

        // Draw icon
        SDLHelper::setColour(SDL_Color{255, 255, 255, 255});
        SDLHelper::drawTexture(this->icon, x + 10, y + 10, h - 20, h - 20);

        // Print text
        SDLHelper::drawTexture(this->title, UI::theme.text, x + h + 10, y + 15);
        SDLHelper::drawTexture(this->playtime, UI::theme.accent, x + h + 10, y + 51);
        SDLHelper::drawTexture(this->lastplayed, UI::theme.muted_text, x + h + 10, y + 82);
        if (this->rank != nullptr) {
            int tw, th;
            SDLHelper::getDimensions(this->rank, &tw, &th);
            SDLHelper::drawTexture(this->rank, UI::theme.muted_text, x + w - tw - 15, y + 15);
        }
    }

    void ListItem::setRank(size_t i) {
        // If set to zero remove texture
        if (i == 0) {
            if (this->rank != nullptr) {
                SDLHelper::destroyTexture(this->rank);
            }
            this->rank = nullptr;
            return;
        }

        // Otherwise create new texture
        std::string str = "#" + std::to_string(i);
        this->rank = SDLHelper::renderText(str.c_str(), SUB_FONT_SIZE);
    }

    Title * ListItem::getTitleObj() {
        return this->title_obj;
    }

    ListItem::~ListItem() {
        // Destroy created textures
        SDLHelper::destroyTexture(this->title);
        SDLHelper::destroyTexture(this->playtime);
        SDLHelper::destroyTexture(this->lastplayed);

        if (this->rank != nullptr) {
            SDLHelper::destroyTexture(this->rank);
        }

        delete this->title_obj;
    }
};