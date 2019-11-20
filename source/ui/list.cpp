#include <algorithm>
#include "list.hpp"

// Height of items in list
#define ITEM_HEIGHT 120
// Font size of sorting text
#define SORT_FONT_SIZE 18
// Decrement scroll velocity bu this amount
#define SCROLL_DEC 20

namespace UI {
    List::List(int x, int y, int w, int h) : Drawable(x, y, w, h) {
        this->pos = 0;
        this->sorting = SortType::LastPlayed;
        this->sort_text = nullptr;

        this->is_touched = false;
        this->is_scrolling = false;
        this->scroll_v = 0;
    }

    void List::addItem(ListItem * item) {
        this->items.push_back(item);
        // Update maximum scroll pos
        if (this->items.size() > 4) {
            this->max_pos = ITEM_HEIGHT * (this->items.size() - 4);
        } else {
            this->max_pos = 0;
        }
    }

    void List::update(uint32_t dt) {
        if (this->is_scrolling) {
            this->setPos(this->getPos() - this->scroll_v);
            if (this->scroll_v < 0) {
                this->scroll_v += SCROLL_DEC * (dt/1000.0);
            } else {
                this->scroll_v -= SCROLL_DEC * (dt/1000.0);
            }

            if (this->scroll_v > -1 && this->scroll_v < 1) {
                this->scroll_v = 0;
                this->is_scrolling = false;
            }
        }
    }

    void List::draw() {
        // Draw list items
        int tw, th;
        SDLHelper::getDimensions(this->sort_text, &tw, &th);

        size_t i = this->pos/ITEM_HEIGHT;
        if (i > 0) {
            i--;
        } else {
            // Print sorting type
            SDLHelper::drawTexture(this->sort_text, this->theme->getMutedText(), this->x + (this->w/2) - (tw/2), this->y + 10 - this->pos);
        }

        for (; i < (this->pos/ITEM_HEIGHT) + 5; i++) {
            // Break out of loop if going to access outside vector
            if (i >= this->items.size()) {
                break;
            }

            // List items utilize 85% of list width
            this->items[i]->setX(this->x + this->w*0.075);
            this->items[i]->setY(this->y + 25 + th + ((ITEM_HEIGHT * i) - this->pos));
            this->items[i]->setW(this->w*0.85);
            this->items[i]->setH(ITEM_HEIGHT);
            this->items[i]->draw();
        }

        // Draw side bar
        if (this->max_pos != 0) {
            SDLHelper::setColour(this->theme->getMutedLine());
            int yPos = this->y + (((float)this->pos / this->max_pos) * (this->h - 95));
            SDLHelper::drawRect(this->x + this->w - 25, yPos, 5, 70);
        }
    }

    void List::touched(uint32_t type, float tx, float ty, float dx, float dy) {
        switch (type) {
            // Pressed
            case SDL_FINGERDOWN:
                this->is_touched = true;
                break;

            // Moved
            case SDL_FINGERMOTION:
                this->setPos(this->getPos() - dy);
                this->scroll_v = dy;
                if (this->scroll_v > 30) {
                    this->scroll_v = 30;
                } else if (this->scroll_v < -30) {
                    this->scroll_v = -30;
                }
                break;

            // Released
            case SDL_FINGERUP:
                this->is_scrolling = true;
                this->is_touched = false;
                break;
        }
    }

    unsigned int List::getPos() {
        return this->pos;
    }

    void List::setPos(unsigned int pos) {
        // Avoid going from 0 to max uint val
        if (this->pos == 0 && pos > this->max_pos) {
            return;
        }
        this->pos = pos;
        if (this->pos > this->max_pos) {
            this->pos = this->max_pos;
        }
    }

    SortType List::getSorting() {
        return this->sorting;
    }

    void List::sort(SortType type) {
        // Always sort alphabetically first
        std::sort(this->items.begin(), this->items.end(), [](ListItem * lhs, ListItem * rhs){
            return lhs->getTitleObj()->getName() < rhs->getTitleObj()->getName();
        });

        std::string str;
        switch (type){
            case AlphaAsc:
                str = "Sorting: Alphabetically";
                break;
            case HoursAsc:
                std::sort(this->items.begin(), this->items.end(), [](ListItem * lhs, ListItem * rhs){
                    return lhs->getTitleObj()->getPlaytime() > rhs->getTitleObj()->getPlaytime();
                });
                str = "Sorting by: Most Played";
                break;
            case HoursDec:
                std::sort(this->items.begin(), this->items.end(), [](ListItem * lhs, ListItem * rhs){
                    return lhs->getTitleObj()->getPlaytime() < rhs->getTitleObj()->getPlaytime();
                });
                str = "Sorting by: Least Played";
                break;
            case LaunchAsc:
                std::sort(this->items.begin(), this->items.end(), [](ListItem * lhs, ListItem * rhs){
                    return lhs->getTitleObj()->getLaunches() > rhs->getTitleObj()->getLaunches();
                });
                str = "Sorting by: Most Launched";
                break;
            case LaunchDec:
                std::sort(this->items.begin(), this->items.end(), [](ListItem * lhs, ListItem * rhs){
                    return lhs->getTitleObj()->getLaunches() < rhs->getTitleObj()->getLaunches();
                });
                str = "Sorting by: Least Launched";
                break;
            case FirstPlayed:
                std::sort(this->items.begin(), this->items.end(), [](ListItem * lhs, ListItem * rhs){
                    return lhs->getTitleObj()->getFirstPlayed() < rhs->getTitleObj()->getFirstPlayed();
                });
                while (this->items[0]->getTitleObj()->getFirstPlayed() == 0){
                    std::rotate(this->items.begin(), this->items.begin()+1, this->items.end());
                }
                str = "Sorting by: First Played";
                break;
            case LastPlayed:
                std::sort(this->items.begin(), this->items.end(), [](ListItem * lhs, ListItem * rhs){
                    return lhs->getTitleObj()->getLastPlayed() > rhs->getTitleObj()->getLastPlayed();
                });
                str = "Sorting by: Recently Played";
                break;
        }

        // Set sorted type
        this->sorting = type;

        // Refresh item rankings
        for (size_t i = 0; i < this->items.size(); i++) {
            if (this->sorting != AlphaAsc) {
                this->items[i]->setRank(i + 1);
            } else {
                this->items[i]->setRank(0);
            }
        }

        // Create texture
        if (this->sort_text != nullptr) {
            SDLHelper::destroyTexture(this->sort_text);
        }
        this->sort_text = SDLHelper::renderText(str.c_str(), SORT_FONT_SIZE);
    }

    bool List::isTouched() {
        return this->is_touched;
    }

    List::~List() {
        // Delete ListItem objects
        while (this->items.size() > 0) {
            delete this->items[0];
            this->items.erase(this->items.begin());
        }

        if (this->sort_text != nullptr) {
            SDLHelper::destroyTexture(this->sort_text);
        }
    }
};