#include <algorithm>
#include "list.hpp"
#include "SDLHelper.hpp"
#include "ui/theme.hpp"

// Height of items in list
#define ITEM_HEIGHT 120
// Font size of sorting text
#define SORT_FONT_SIZE 18

namespace UI {
    List::List() {
        this->pos = 0;
        this->sorting = SortType::LastPlayed;
        this->sort_text = nullptr;
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

    }

    void List::draw(int x, int y, int w, int h) {
        // Draw list items
        int tw, th;
        SDLHelper::getDimensions(this->sort_text, &tw, &th);

        size_t i = this->pos/ITEM_HEIGHT;
        if (i > 0) {
            i--;
        } else {
            // Print sorting type
            SDLHelper::drawTexture(this->sort_text, UI::theme.muted_text, x + (w/2) - (tw/2), y + 10 - this->pos);
        }

        for (; i < (this->pos/ITEM_HEIGHT) + 5; i++) {
            // Break out of loop if going to access outside vector
            if (i >= this->items.size()) {
                break;
            }

            // List items utilize 85% of list width
            this->items[i]->draw(x + w*0.075, y + 25 + th + ((ITEM_HEIGHT * i) - pos), w*0.85, ITEM_HEIGHT);
        }

        // Draw side bar
        if (this->max_pos != 0) {
            SDLHelper::setColour(UI::theme.muted_line);
            int yPos = y + (((float)this->pos / this->max_pos) * (h - 95));
            SDLHelper::drawRect(x + w - 25, yPos, 5, 70);
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