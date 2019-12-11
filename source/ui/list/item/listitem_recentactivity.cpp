#include "listitem_recentactivity.hpp"
#include "screenmanager.hpp"
#include "TimeHelper.hpp"
#include "Title.hpp"
#include "utils.hpp"

// Font sizes
#define TITLE_FONT_SIZE 22
#define SUB_FONT_SIZE 18

// Height of items
#define ITEM_HEIGHT 120

// Text scroll speed (in ms)
#define TEXT_SPEED 120
// Text scroll pause (in ms)
#define TEXT_PAUSE 600

namespace UI::ListItem {
    RecentActivity::RecentActivity(RecentPlayStatistics * stats, unsigned int r) : List_Item() {
        this->setH(ITEM_HEIGHT);

        // Create textures using titleID + stats
        // Get name and icon
        NsApplicationControlData data;
        NacpLanguageEntry * lang = nullptr;
        size_t nacp_size;
        Result rc = nsGetApplicationControlData(NsApplicationControlSource_Storage, stats->titleID, &data, sizeof(NsApplicationControlData), &nacp_size);
        if (R_SUCCEEDED(rc)){
            // Get name
            rc = nacpGetLanguageEntry(&data.nacp, &lang);
            if (R_SUCCEEDED(rc)){
                this->title = SDLHelper::renderText(lang->name, TITLE_FONT_SIZE);
            }

            // Get icon
            size_t icon_size = nacp_size - sizeof(data.nacp);
            // Create texture
            this->icon = SDLHelper::renderImage(data.icon, icon_size);
        }

        std::string str = "Played for " + TimeH::playtimeToString(stats->playtime, " and ");
        this->playtime = SDLHelper::renderText(str.c_str(), SUB_FONT_SIZE);
        str = "Played " + std::to_string(stats->launches);
        (stats->launches == 1) ? str += " time" : str += " times";
        this->launches = SDLHelper::renderText(str.c_str(), SUB_FONT_SIZE);
        str = "#" + std::to_string(r);
        this->rank = SDLHelper::renderText(str.c_str(), SUB_FONT_SIZE);

        // Initialize all other variables
        this->text_scroll_x = 0;
        this->text_scroll_pause = 0;
        this->is_selectable = true;
    }

    void RecentActivity::update(uint32_t dt) {
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

    void RecentActivity::draw() {
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
        SDLHelper::drawTexture(this->launches, this->theme->getMutedText(), this->x + this->h + 10, this->y + 83);
        SDLHelper::getDimensions(this->rank, &tw, &th);
        SDLHelper::drawTexture(this->rank, this->theme->getMutedText(), this->x + this->w - tw - 15, this->y + 15);
    }

    void RecentActivity::pressed() {

    }

    void RecentActivity::setSelected(bool b) {
        List_Item::setSelected(b);

        // Reset text scroll if deselected
        if (!b) {
            this->text_scroll_x = 0;
            this->text_scroll_pause = 0;
        }
    }

    RecentActivity::~RecentActivity() {
        // Destroy created textures
        SDLHelper::destroyTexture(this->title);
        SDLHelper::destroyTexture(this->launches);
        SDLHelper::destroyTexture(this->playtime);
        SDLHelper::destroyTexture(this->icon);
    }
};