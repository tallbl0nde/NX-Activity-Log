#include <algorithm>
#include "listitem.hpp"
#include "utils.hpp"

// Font sizes
#define TITLE_FONT_SIZE 22
#define SUB_FONT_SIZE 18

// Text scroll speed (in ms)
#define TEXT_SPEED 120
// Text scroll pause (in ms)
#define TEXT_PAUSE 600

namespace UI {
    ListItem::ListItem(Title * t) {
        // Create textures using title object
        this->icon = t->getIcon();
        this->title = SDLHelper::renderText(t->getName().c_str(), TITLE_FONT_SIZE);
        std::string str = "Played for " + Utils::formatPlaytime(t->getPlaytime());
        this->playtime = SDLHelper::renderText(str.c_str(), SUB_FONT_SIZE);
        this->lastplayed = SDLHelper::renderText(Utils::formatLastPlayed(t->getLastPlayed()).c_str(), SUB_FONT_SIZE);

        this->text_x = 0;
        this->text_pause = 0;
        this->rank = nullptr;
        this->title_obj = t;
        this->selected = false;
    }

    void ListItem::update(uint32_t dt) {
        if (this->selected) {
            int tw, th;
            SDLHelper::getDimensions(this->title, &tw, &th);
            if (tw > this->w*0.7) {
                if (this->text_x > (tw - (this->w*0.7))) {
                    if (this->text_pause > TEXT_PAUSE) {
                        this->text_x = 0;
                        this->text_pause = 0;
                    } else {
                        this->text_pause += dt;
                    }
                } else {
                    this->text_x += TEXT_SPEED*(dt/1000.0);
                }
            }
        }
    }

    void ListItem::draw() {
        // Draw outlines
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, this->w, 1);
        SDLHelper::drawRect(this->x, this->y + this->h, this->w, 1);

        // Draw icon
        SDLHelper::setColour(SDL_Color{255, 255, 255, 255});
        SDLHelper::drawTexture(this->icon, this->x + 10, this->y + 10, this->h - 20, this->h - 20);

        // Print text
        int tw, th;
        SDLHelper::getDimensions(this->title, &tw, &th);
        if (tw > this->w*0.7) {
            SDLHelper::drawTexture(this->title, this->theme->getText(), this->x + this->h + 10, this->y + 15, this->w*0.7, th, this->text_x, this->w*0.7);
        } else {
            SDLHelper::drawTexture(this->title, this->theme->getText(), this->x + this->h + 10, this->y + 15);
        }
        SDLHelper::drawTexture(this->playtime, this->theme->getAccent(), this->x + this->h + 10, this->y + 51);
        SDLHelper::drawTexture(this->lastplayed, this->theme->getMutedText(), this->x + this->h + 10, this->y + 82);
        if (this->rank != nullptr) {
            SDLHelper::getDimensions(this->rank, &tw, &th);
            SDLHelper::drawTexture(this->rank, this->theme->getMutedText(), this->x + this->w - tw - 15, this->y + 15);
        }
    }

    void ListItem::setSelected(bool b) {
        this->selected = b;
        if (!b) {
            this->text_x = 0;
            this->text_pause = 0;
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