#include <algorithm>
#include "config.hpp"
#include "listitem_activity.hpp"
#include "sortedlist.hpp"
#include "utils.hpp"

// Offset of list
#define LIST_OFFSET 35
// Font size of sorting text
#define SORT_FONT_SIZE 18

namespace UI {
    SortedList::SortedList(bool * b, int x, int y, int w, int h) : List(b, x, y, w, h, LIST_OFFSET) {
        this->sorting = SortType::AlphaAsc;
        this->sort_text = nullptr;
    }

    void SortedList::addItem(List_Item * item) {
        // Sort list using saved config if no item provided
        if (item == nullptr) {
            Config * conf = Config::getConfig();
            this->sort(conf->getGeneralSort());

        // Otherwise add item
        } else {
            List::addItem(item);
        }
    }

    void SortedList::draw() {
        List::draw();

        // Print sorting type
        int tw, th;
        SDLHelper::getDimensions(this->sort_text, &tw, &th);
        SDLHelper::drawTexture(this->sort_text, this->theme->getMutedText(), this->x + (this->w/2) - (tw/2), this->y - this->scroll_pos);
    }

    void SortedList::handleButton(uint8_t button, uint8_t state) {
        List::handleButton(button, state);

        // Perform sort when minus pressed
        if (state == SDL_PRESSED) {
            if (button == Utils::key_map[KEY_MINUS]) {
                switch (this->sorting) {
                    case AlphaAsc:
                        this->sort(HoursAsc);
                        break;
                    case HoursAsc:
                        this->sort(HoursDec);
                        break;
                    case HoursDec:
                        this->sort(LaunchAsc);
                        break;
                    case LaunchAsc:
                        this->sort(LaunchDec);
                        break;
                    case LaunchDec:
                        this->sort(FirstPlayed);
                        break;
                    case FirstPlayed:
                        this->sort(LastPlayed);
                        break;
                    case LastPlayed:
                        this->sort(AlphaAsc);
                        break;
                }

                this->setScrollPos(0);
            }
        }
    }

    void SortedList::sort(SortType type) {
        // Always sort alphabetically first
        std::sort(this->items.begin(), this->items.end(), [](List_Item * lhs, List_Item * rhs){
            return static_cast<ListItem::Activity *>(lhs)->getTitleObj()->getName() < static_cast<ListItem::Activity *>(rhs)->getTitleObj()->getName();
        });

        std::string str;
        switch (type){
            case AlphaAsc:
                str = "Sorting: Alphabetically";
                break;
            case HoursAsc:
                std::sort(this->items.begin(), this->items.end(), [](List_Item * lhs, List_Item * rhs){
                    return static_cast<ListItem::Activity *>(lhs)->getTitleObj()->getPlaytime() > static_cast<ListItem::Activity *>(rhs)->getTitleObj()->getPlaytime();
                });
                str = "Sorting by: Most Played";
                break;
            case HoursDec:
                std::sort(this->items.begin(), this->items.end(), [](List_Item * lhs, List_Item * rhs){
                    return static_cast<ListItem::Activity *>(lhs)->getTitleObj()->getPlaytime() < static_cast<ListItem::Activity *>(rhs)->getTitleObj()->getPlaytime();
                });
                str = "Sorting by: Least Played";
                break;
            case LaunchAsc:
                std::sort(this->items.begin(), this->items.end(), [](List_Item * lhs, List_Item * rhs){
                    return static_cast<ListItem::Activity *>(lhs)->getTitleObj()->getLaunches() > static_cast<ListItem::Activity *>(rhs)->getTitleObj()->getLaunches();
                });
                str = "Sorting by: Most Launched";
                break;
            case LaunchDec:
                std::sort(this->items.begin(), this->items.end(), [](List_Item * lhs, List_Item * rhs){
                    return static_cast<ListItem::Activity *>(lhs)->getTitleObj()->getLaunches() < static_cast<ListItem::Activity *>(rhs)->getTitleObj()->getLaunches();
                });
                str = "Sorting by: Least Launched";
                break;
            case FirstPlayed:
                std::sort(this->items.begin(), this->items.end(), [](List_Item * lhs, List_Item * rhs){
                    return static_cast<ListItem::Activity *>(lhs)->getTitleObj()->getFirstPlayed() < static_cast<ListItem::Activity *>(rhs)->getTitleObj()->getFirstPlayed();
                });
                while (static_cast<ListItem::Activity *>(this->items[0])->getTitleObj()->getFirstPlayed() == 0){
                    std::rotate(this->items.begin(), this->items.begin()+1, this->items.end());
                }
                str = "Sorting by: First Played";
                break;
            case LastPlayed:
                std::sort(this->items.begin(), this->items.end(), [](List_Item * lhs, List_Item * rhs){
                    return static_cast<ListItem::Activity *>(lhs)->getTitleObj()->getLastPlayed() > static_cast<ListItem::Activity *>(rhs)->getTitleObj()->getLastPlayed();
                });
                str = "Sorting by: Recently Played";
                break;
        }

        // Set sorted type
        this->sorting = type;

        // Stop scrolling
        this->is_scrolling = false;
        this->highlight_item = 0;

        // Refresh item rankings
        unsigned int offset = 0;
        for (size_t i = 0; i < this->items.size(); i++) {
            if (this->sorting != AlphaAsc) {
                static_cast<ListItem::Activity *>(this->items[i])->setRank(i + 1);
            } else {
                static_cast<ListItem::Activity *>(this->items[i])->setRank(0);
            }
            this->items[i]->setOffset(offset);
            offset += this->items[i]->getH();
        }

        // Create texture
        if (this->sort_text != nullptr) {
            SDLHelper::destroyTexture(this->sort_text);
        }
        this->sort_text = SDLHelper::renderText(str.c_str(), SORT_FONT_SIZE);
    }

    SortedList::~SortedList() {
        if (this->sort_text != nullptr) {
            SDLHelper::destroyTexture(this->sort_text);
        }
    }
};