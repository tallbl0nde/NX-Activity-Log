#include <algorithm>
#include <ctime>
#include <iomanip>
#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <sstream>
#include "theme.hpp"
#include "utils.h"

// Font sizes
#define TITLE_FONT_SIZE 25
#define SUB_FONT_SIZE 20

namespace UI {
    ListItem::ListItem(Title * t) {
        // Format timestamp for printing
        std::stringstream ss;
        std::time_t timestamp = pdmPlayTimestampToPosix(t->getLastPlayed());
        ss << std::put_time(std::localtime(&timestamp), "%d %B %Y %I:%M %p");

        // Create textures using title object
        this->icon = t->getIcon();
        SDLHelper::setColour(UI::theme.text);
        this->title = SDLHelper::renderText(t->getName().c_str(), TITLE_FONT_SIZE);
        SDLHelper::setColour(UI::theme.accent);
        this->playtime = SDLHelper::renderText(Utils::formatPlaytime(t->getPlaytime()).c_str(), SUB_FONT_SIZE);
        SDLHelper::setColour(UI::theme.muted_text);
        this->lastplayed = SDLHelper::renderText(ss.str().c_str(), SUB_FONT_SIZE);

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
        SDLHelper::setColour(SDL_Color{150, 150, 150, 255});
        SDLHelper::drawRect(x, y, w, 1);
        SDLHelper::drawRect(x, y + h, w, 1);

        // Draw icon
        SDLHelper::setColour(SDL_Color{255, 255, 255, 255});
        SDLHelper::drawTexture(this->icon, x + 10, y + 10, h - 20, h - 20);

        // Print text
        SDLHelper::drawTexture(this->title, x + h + 10, y + 20);
        SDLHelper::drawTexture(this->playtime, x + h + 10, y + 55);
        SDLHelper::drawTexture(this->lastplayed, x + h + 10, y + 80);
    }

    ListItem::~ListItem() {
        // Destroy created textures
        SDLHelper::destroyTexture(this->title);
        SDLHelper::destroyTexture(this->playtime);
        SDLHelper::destroyTexture(this->lastplayed);
    }
};