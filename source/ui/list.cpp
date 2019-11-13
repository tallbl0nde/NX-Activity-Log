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
    }

    void List::update(uint32_t dt) {

    }

    void List::draw(int x, int y, int w, int h) {
        // Print sorting type
        int tw, th;
        SDLHelper::getDimensions(this->sort_text, &tw, &th);
        SDLHelper::drawTexture(this->sort_text, UI::theme.muted_text, x + (w/2) - (tw/2), y + 10);

        // Draw list items
        size_t idx = this->pos;
        if (this->pos > 0) {
            idx--;
        }
        for (size_t i = idx; i < idx + 6; i++) {
            // Break out of loop if going to access outside vector
            if (i >= this->items.size()) {
                break;
            }

            // List items utilize 85% of list width
            this->items[i]->draw(x + w*0.075, y + 25 + th + (ITEM_HEIGHT * (i-pos)), w*0.85, ITEM_HEIGHT);
        }

        // Draw side bar
        SDLHelper::setColour(SDL_Color{120, 120, 120, 255});
        int yPos = y + (((float)this->pos / this->items.size()) * (h - 50));
        SDLHelper::drawRect(x + w - 10, yPos, 10, 50);
    }

    size_t List::getPos() {
        return this->pos;
    }

    void List::movePos(size_t pos) {
        this->pos = pos;
        if (this->pos > this->items.size()-3) {
            this->pos = this->items.size()-3;
        } else if (this->pos < 0) {
            this->pos = 0;
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