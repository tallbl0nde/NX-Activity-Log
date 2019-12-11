#include "datedlist.hpp"

// Offset of list
#define LIST_OFFSET 40
// Font size of date text
#define DATE_FONT_SIZE 20

namespace UI {
    DatedList::DatedList(bool * b, int x, int y, int w, int h) : List(b, x, y, w, h, LIST_OFFSET) {
        this->date_text = nullptr;
    }

    void DatedList::addItem(ListItem::RecentActivity * item) {
        List::addItem(item);
    }

    void DatedList::draw() {
        List::draw();

        // Print sorting type
        int tw, th;
        SDLHelper::getDimensions(this->date_text, &tw, &th);
        SDLHelper::drawTexture(this->date_text, this->theme->getText(), this->x + (this->w/2) - (tw/2), this->y - this->scroll_pos);
    }

    void DatedList::setDate(std::string s) {
        s = "Activity for: " + s;

        if (this->date_text != nullptr) {
            SDLHelper::destroyTexture(this->date_text);
        }
        this->date_text = SDLHelper::renderText(s.c_str(), DATE_FONT_SIZE);
    }

    DatedList::~DatedList() {
        if (this->date_text != nullptr) {
            SDLHelper::destroyTexture(this->date_text);
        }
    }
};