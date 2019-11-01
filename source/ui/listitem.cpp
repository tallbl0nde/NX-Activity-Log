#include <algorithm>
#include <ctime>
#include <iomanip>
#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <sstream>
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
        this->last_played = ss.str();

        this->title = t;
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
        SDLHelper::setColour(150, 150, 150, 255);
        SDLHelper::drawRect(x, y, w, 1);
        SDLHelper::drawRect(x, y + h, w, 1);

        // Draw icon
        SDLHelper::setColour(255, 255, 255, 255);
        SDLHelper::drawTexture(this->title->getIcon(), x + 10, y + 10, h - 20, h - 20);

        // Print name
        SDLHelper::setColour(0, 0, 0, 255);
        SDLHelper::drawText(this->title->getName().c_str(), x + h, y + 20, TITLE_FONT_SIZE);

        // Print play time
        SDLHelper::setColour(120, 120, 120, 255);
        std::string str = Utils::formatPlaytime(this->title->getPlaytime());
        SDLHelper::drawText(str.c_str(), x + h, y + 40, SUB_FONT_SIZE);

        // Print last played
        SDLHelper::drawText(this->last_played.c_str(), x + h, y + 60, SUB_FONT_SIZE);
    }

    ListItem::~ListItem() {

    }
};