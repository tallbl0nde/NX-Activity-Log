#include "listitem_activity.hpp"
#include "screenmanager.hpp"
#include "TimeHelper.hpp"
#include "utils.hpp"

// Font sizes
#define TITLE_FONT_SIZE 22
#define SUB_FONT_SIZE 18
#define LAUNCH_FONT_SIZE 16

// Height of items
#define ITEM_HEIGHT 120

// Text scroll speed (in ms)
#define TEXT_SPEED 120
// Text scroll pause (in ms)
#define TEXT_PAUSE 600

namespace UI::ListItem {
    Activity::Activity(Title * t) : List_Item() {
        this->setH(ITEM_HEIGHT);

        // Create textures using title object
        this->icon = t->getIcon();
        this->title = SDLHelper::renderText(t->getName().c_str(), TITLE_FONT_SIZE);
        std::string str = "Played for " + TimeH::playtimeToString(t->getPlaytime() * 60, " and ");
        this->playtime = SDLHelper::renderText(str.c_str(), SUB_FONT_SIZE);
        this->lastplayed = SDLHelper::renderText(TimeH::lastPlayedTimestampToString(pdmPlayTimestampToPosix(t->getLastPlayed())).c_str(), SUB_FONT_SIZE);
        str = "Played " + std::to_string(t->getLaunches());
        (t->getLaunches() == 1) ? str += " time" : str += " times";
        this->launches = SDLHelper::renderText(str.c_str(), LAUNCH_FONT_SIZE);
        this->rank = nullptr;

        // Initialize all other variables
        this->text_scroll_x = 0;
        this->text_scroll_pause = 0;
        this->title_object = t;
        this->is_selectable = true;
    }

    void Activity::update(uint32_t dt) {
        // Scroll text if selected and text is too large
        if (this->is_selected) {
            int tw, th;
            SDLHelper::getDimensions(this->title, &tw, &th);
            if (tw > this->w*0.7) {
                if (this->text_scroll_x > (tw - (this->w*0.7))) {
                    if (this->text_scroll_pause > TEXT_PAUSE) {
                        this->text_scroll_x = 0;
                        this->text_scroll_pause = 0;
                    } else {
                        this->text_scroll_pause += dt;
                    }
                } else {
                    this->text_scroll_x += TEXT_SPEED*(dt/1000.0);
                }
            }
        }
    }

    void Activity::draw() {
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
            SDLHelper::drawTexture(this->title, this->theme->getText(), this->x + this->h + 10, this->y + 15, this->w*0.7, th, this->text_scroll_x, this->w*0.7);
        } else {
            SDLHelper::drawTexture(this->title, this->theme->getText(), this->x + this->h + 10, this->y + 15);
        }
        SDLHelper::drawTexture(this->playtime, this->theme->getAccent(), this->x + this->h + 10, this->y + 51);
        SDLHelper::drawTexture(this->lastplayed, this->theme->getMutedText(), this->x + this->h + 10, this->y + 82);
        if (this->rank != nullptr) {
            SDLHelper::getDimensions(this->rank, &tw, &th);
            SDLHelper::drawTexture(this->rank, this->theme->getMutedText(), this->x + this->w - tw - 15, this->y + 15);
        }
        SDLHelper::getDimensions(this->launches, &tw, &th);
        SDLHelper::drawTexture(this->launches, this->theme->getMutedText(), this->x + this->w - tw - 15, this->y + 82 + (SUB_FONT_SIZE - LAUNCH_FONT_SIZE));
    }

    void Activity::pressed() {

    }

    void Activity::setSelected(bool b) {
        List_Item::setSelected(b);

        // Reset text scroll if deselected
        if (!b) {
            this->text_scroll_x = 0;
            this->text_scroll_pause = 0;
        }
    }

    void Activity::setRank(unsigned int i) {
        if (this->rank != nullptr) {
            SDLHelper::destroyTexture(this->rank);
        }

        // If set to zero remove texture
        if (i == 0) {
            this->rank = nullptr;
            return;
        }

        // Otherwise create new texture
        std::string str = "#" + std::to_string(i);
        this->rank = SDLHelper::renderText(str.c_str(), SUB_FONT_SIZE);
    }

    Title * Activity::getTitleObj() {
        return this->title_object;
    }

    Activity::~Activity() {
        // Destroy created textures
        SDLHelper::destroyTexture(this->title);
        SDLHelper::destroyTexture(this->playtime);
        SDLHelper::destroyTexture(this->lastplayed);

        if (this->rank != nullptr) {
            SDLHelper::destroyTexture(this->rank);
        }
    }
};