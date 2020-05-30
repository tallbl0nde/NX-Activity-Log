#include "ui/element/ListActivity.hpp"

// Height of element
#define HEIGHT 120

namespace CustomElm {
    ListActivity::ListActivity() : Element() {
        Element::setH(HEIGHT);
        this->topR = new Aether::Rectangle(this->x(), this->y(), this->w(), 1);
        this->addElement(this->topR);
        this->bottomR = new Aether::Rectangle(this->x(), this->y() + HEIGHT, this->w(), 1);
        this->addElement(this->bottomR);

        this->title = new Aether::Text(this->x() + 130, this->y() + 15, "", 22);
        this->title->setScrollSpeed(75);
        this->title->setScroll(true);
        this->addElement(this->title);
        this->playtime = new Aether::Text(this->x() + 130, this->y() + 51, "", 18);
        this->addElement(this->playtime);
        this->mutedLeft = new Aether::Text(this->x() + 130, this->y() + 82, "", 18);
        this->addElement(this->mutedLeft);
        this->mutedRight = new Aether::Text(this->x() + this->w(), this->y() + 84, "", 16);
        this->addElement(this->mutedRight);
        this->rank = new Aether::Text(this->x() + this->w(), this->y() + 15, "", 18);
        this->addElement(this->rank);

        this->icon = nullptr;
    }

    void ListActivity::setImage(Aether::Image * i) {
        this->removeElement(this->icon);
        this->icon = i;
        this->icon->setXYWH(this->x() + 10, this->y() + 10, this->h() - 20, this->h() - 20);
        this->addElement(this->icon);
    }

    void ListActivity::setTitle(std::string s) {
        this->title->setString(s);
    }

    void ListActivity::setPlaytime(std::string s) {
        this->playtime->setString(s);
    }

    void ListActivity::setLeftMuted(std::string s) {
        this->mutedLeft->setString(s);
    }

    void ListActivity::setRightMuted(std::string s) {
        this->mutedRight->setString(s);
        this->mutedRight->setX(this->x() + this->w() - this->mutedRight->w() - 15);
    }

    void ListActivity::setRank(std::string s) {
        this->rank->setString(s);
        this->rank->setX(this->x() + this->w() - this->rank->w() - 15);
        this->setTextWidth();
    }

    void ListActivity::setTitleColour(Aether::Colour c) {
        this->title->setColour(c);
    }

    void ListActivity::setPlaytimeColour(Aether::Colour c) {
        this->playtime->setColour(c);
    }

    void ListActivity::setMutedColour(Aether::Colour c) {
        this->mutedLeft->setColour(c);
        this->mutedRight->setColour(c);
        this->rank->setColour(c);
    }

    void ListActivity::setLineColour(Aether::Colour c) {
        this->topR->setColour(c);
        this->bottomR->setColour(c);
    }

    void ListActivity::setTextWidth() {
        if (this->title->texW() > this->rank->x() - this->title->x() - 25) {
            this->title->setW(this->rank->x() - this->title->x() - 25);
        }
    }

    // Adjusting width must move/adjust children
    void ListActivity::setW(int w) {
        Element::setW(w);
        this->topR->setRectSize(this->w(), 1);
        this->bottomR->setRectSize(this->w(), 1);
        this->mutedRight->setX(this->x() + this->w() - this->mutedRight->w() - 15);
        this->rank->setX(this->x() + this->w() - this->rank->w() - 15);
        this->setTextWidth();
    }
};