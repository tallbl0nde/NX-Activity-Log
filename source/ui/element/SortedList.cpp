#include "utils/Lang.hpp"
#include "ui/element/SortedList.hpp"
#include "utils/Utils.hpp"

namespace CustomElm {
    SortedList::SortedList(int x, int y, int w, int h) : Aether::List(x, y, w, h) {
        this->heading = new Aether::Text(0, 0, "I", 18);
        List::addElement(this->heading);
        this->heading->setY(this->heading->y() - 15);
        List::addElement(new Aether::ListSeparator(20));
    }

    void SortedList::addElement(ListActivity * e, SortInfo * i) {
        List::addElement(e);
        this->sortinfo.push_back(i);
    }

    void SortedList::removeAllElements() {
        // Remove everything but heading + separator
        while (this->children.size() > 2) {
            delete this->children[2];
            this->children.erase(this->children.begin() + 2);
        }
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
                this->heading->setString("allActivity.sort.name"_lang);
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->name < rhs.second->name;
                });
                break;
            case HoursAsc:
                this->heading->setString("allActivity.sort.mostPlaytime"_lang);
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->playtime > rhs.second->playtime;
                });
                break;
            case HoursDec:
                this->heading->setString("allActivity.sort.leastPlaytime"_lang);
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->playtime < rhs.second->playtime;
                });
                break;
            case LaunchAsc:
                this->heading->setString("allActivity.sort.mostLaunched"_lang);
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->launches > rhs.second->launches;
                });
                break;
            case LaunchDec:
                this->heading->setString("allActivity.sort.leastLaunched"_lang);
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->launches < rhs.second->launches;
                });
                break;
            case FirstPlayed:
                this->heading->setString("allActivity.sort.firstPlayed"_lang);
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->firstPlayed < rhs.second->firstPlayed;
                });
                // Push never played to bottom
                while (merged[0].second->firstPlayed == 0) {
                    std::rotate(merged.begin(), merged.begin()+1, merged.end());
                }
                break;
            case LastPlayed:
                this->heading->setString("allActivity.sort.recentlyPlayed"_lang);
                std::sort(merged.begin(), merged.end(), [](std::pair<Element *, SortInfo *> lhs, std::pair<Element *, SortInfo *> rhs){
                    return lhs.second->lastPlayed > rhs.second->lastPlayed;
                });
                break;

            default:
                break;
        }
        this->heading->setX(this->x() + (this->w() - this->heading->w())/2);

        // Split vectors and update positions
        Utils::splitVectors(merged, items, this->sortinfo);
        for (size_t i = 0; i < items.size(); i++) {
            this->children[i+2] = items[i];
            this->children[i+2]->setY(this->children[i+1]->y() + this->children[i+1]->h());
            if (this->sorting != AlphaAsc) {
                static_cast<ListActivity *>(this->children[i+2])->setRank("#" + std::to_string(i+1));
            } else {
                static_cast<ListActivity *>(this->children[i+2])->setRank("");
            }
        }

        // Finally reset scroll pos
        this->setScrollPos(0);
        if (this->children.size() > 2) {
            this->setFocussed(this->children[2]);
        }
    }

    SortType SortedList::sort() {
        return this->sorting;
    }

    void SortedList::setHeadingColour(Aether::Colour c) {
        this->heading->setColour(c);
    }
};