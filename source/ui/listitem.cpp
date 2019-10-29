#include <algorithm>
#include <ctime>
#include <iomanip>
#include "listitem.hpp"
#include <sstream>

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

    void ListItem::draw(SDL_Renderer * renderer, int x, int y, int w, int h) {
        // Draw background
        SDL_Rect rect = {x, y, w, h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Draw icon
        rect.x = x + 10;
        rect.y = y + 10;
        rect.w = h - 20;
        rect.h = rect.w;
        SDL_RenderCopy(renderer, this->title->getIcon(), NULL, &rect);

        // Draw name

    }

    ListItem::~ListItem() {

    }
};