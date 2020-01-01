#include <cmath>
#include "listitem_session.hpp"
#include "TimeHelper.hpp"

// Font size
#define TEXT_SIZE 20
// Pixels to indent text
#define INDENT 16
// Height of box
#define BOX_HEIGHT 85

namespace UI::ListItem {
    Session::Session(u64 pt, u64 tpt, u64 s, u64 e) : List_Item() {
        // Calculate percentage (if total playtime is zero just make it 100%)
        double percent = 100 * ((double)pt / ((tpt == 0) ? pt : tpt));
        percent = roundf(percent * 100) / 100;
        std::string str;
        if (percent < 0.01) {
            str = "< 0.01%";
        } else {
            str = std::to_string(percent);
            while (str[str.length() - 3] != '.') {
                str = str.substr(0, str.length() - 1);
            }
            str += "%";
        }
        this->percentage = SDLHelper::renderText(str.c_str(), TEXT_SIZE);

        this->playtime = SDLHelper::renderText(TimeH::playtimeToString(pt, ", ").c_str(), TEXT_SIZE);

        // Determine time range
        struct tm sTm = TimeH::getTm(s);
        struct tm nTm = TimeH::getTmForCurrentTime();
        bool isAM = ((sTm.tm_hour < 12) ? true : false);
        if (sTm.tm_hour == 0) {
            sTm.tm_hour = 12;
        }
        str = TimeH::tmToString(sTm, true, true, !(sTm.tm_year == nTm.tm_year)) + " - " \
            + std::to_string(((isAM) ? sTm.tm_hour : sTm.tm_hour - 12)) + ":" \
            + ((sTm.tm_min < 10) ? "0" : "" ) + std::to_string(sTm.tm_min) \
            + ((isAM) ? "am" : "pm");
        this->time = SDLHelper::renderText(str.c_str(), TEXT_SIZE);

        // Set height (temporary)
        this->setH(BOX_HEIGHT);

        // Mark unselectable
        this->is_selectable = false;
    }

    void Session::draw() {
         // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getMutedLine());
        SDLHelper::drawRect(this->x, this->y, this->w, 1);
        SDLHelper::drawRect(this->x, this->y + this->h, this->w, 1);

        // Draw text
        SDLHelper::drawTexture(this->time, this->theme->getText(), this->x + INDENT, this->y + 15);
        SDLHelper::drawTexture(this->playtime, this->theme->getAccent(), this->x + INDENT, this->y + 50);
        int tw, th;
        SDLHelper::getDimensions(this->percentage, &tw, &th);
        SDLHelper::drawTexture(this->percentage, this->theme->getMutedText(), this->x + this->w - tw - INDENT, this->y + 50);
    }

    Session::~Session() {
        SDLHelper::destroyTexture(this->time);
        SDLHelper::destroyTexture(this->playtime);
        SDLHelper::destroyTexture(this->percentage);
    }
};