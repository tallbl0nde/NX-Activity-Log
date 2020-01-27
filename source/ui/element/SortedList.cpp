#include "SortedList.hpp"
#include "Utils.hpp"

namespace CustomElm {
    SortedList::SortedList(int x, int y, int w, int h) : Aether::List(x, y, w, h) {
        this->heading = new Aether::Text(0, 0, "Text", 20);
        List::addElement(this->heading);
        List::addElement(new Aether::ListSeparator());
    }

    void SortedList::addElement(ListActivity * e, SortInfo * i) {
        List::addElement(e);
        this->sortinfo.push_back(i);
    }

    void SortedList::removeAllElements() {
        List::removeAllElements();
        while (this->sortinfo.size() > 0) {
            delete this->sortinfo[0];
            this->sortinfo.erase(this->sortinfo.begin());
        }
    }

    void SortedList::setSort(SortType t) {
        this->sorting = t;

        // Merge vectors
        std::vector<Element *> items(this->children.begin() + 2, this->children.end());
        std::vector<std::pair<Element *, SortInfo *> > merged;
        Utils::mergeVectors(merged, items, this->sortinfo);

        // Change heading text + reorder items
        switch (t) {
            case AlphaAsc:
                this->heading->setString("Sorting: Alphabetically");
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->name < rhs.second->name;
                });
                break;
            case HoursAsc:
                this->heading->setString("Sorting by: Most Played");
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->playtime > rhs.second->playtime;
                });
                break;
            case HoursDec:
                this->heading->setString("Sorting by: Least Played");
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->playtime < rhs.second->playtime;
                });
                break;
            case LaunchAsc:
                this->heading->setString("Sorting by: Most Launched");
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->launches > rhs.second->launches;
                });
                break;
            case LaunchDec:
                this->heading->setString("Sorting by: Least Launched");
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->launches < rhs.second->launches;
                });
                break;
            case FirstPlayed:
                this->heading->setString("Sorting by: First Played");
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->firstPlayed < rhs.second->firstPlayed;
                });
                // Push never played to bottom
                while (merged[0].second->firstPlayed == 0) {
                    std::rotate(merged.begin(), merged.begin()+1, merged.end());
                }
                break;
            case LastPlayed:
                this->heading->setString("Sorting by: Recently Played");
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->lastPlayed > rhs.second->lastPlayed;
                });
                break;
        }

        // Split vectors and update positions
        Utils::splitVectors(merged, items, this->sortinfo);
        for (size_t i = 0; i < items.size(); i++) {
            this->children[i+2] = items[i];
        }

        // Finally reset scroll pos
        this->setScrollPos(0);
    }

    SortType SortedList::sort() {
        return this->sorting;
    }
};