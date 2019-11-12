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
        // Timestamp stuff
        std::time_t play_stamp = pdmPlayTimestampToPosix(t->getLastPlayed());
        std::time_t now_stamp;
        std::time(&now_stamp);

        // Create textures using title object
        this->icon = t->getIcon();
        this->title = SDLHelper::renderText(t->getName().c_str(), TITLE_FONT_SIZE);
        this->playtime = SDLHelper::renderText(Utils::formatPlaytime(t->getPlaytime()).c_str(), SUB_FONT_SIZE);
        this->lastplayed = SDLHelper::renderText(Utils::formatTimestamps(play_stamp, now_stamp).c_str(), SUB_FONT_SIZE);

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
        SDLHelper::drawTexture(this->playtime, UI::theme.accent, x + h + 10, y + 48);
        SDLHelper::drawTexture(this->lastplayed, UI::theme.muted_text, x + h + 10, y + 77);
    }

    ListItem::~ListItem() {
        // Destroy created textures
        SDLHelper::destroyTexture(this->title);
        SDLHelper::destroyTexture(this->playtime);
        SDLHelper::destroyTexture(this->lastplayed);
    }
};