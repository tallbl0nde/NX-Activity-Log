#include "ui/element/ListActivity.hpp"

namespace CustomElm {
    ListActivity::ListActivity() : Element() {
        Element::setH(ListActivity::height);

        // Create our 'bounding' rectangles
        this->topR = new Aether::Rectangle(this->x(), this->y(), this->w(), 1);
        this->topR->setHidden(true);
        this->addElement(this->topR);
        this->bottomR = new Aether::Rectangle(this->x(), this->y() + this->h(), this->w(), 1);
        this->bottomR->setHidden(true);
        this->addElement(this->bottomR);

        // Start with blank elements
        this->icon       = new Aether::Image(0, 0, "", Aether::Render::Wait);
        this->title      = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->playtime   = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->mutedLeft  = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->mutedRight = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->rank       = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->rank->setHidden(true);
    }

    void ListActivity::positionItems() {
        this->mutedRight->setX(this->x() + this->w() - this->mutedRight->w() - 15);
        this->rank->setX(this->x() + this->w() - this->rank->w() - 15);

        if (this->title->textureWidth() > this->rank->x() - this->title->x() - 25) {
            this->title->setW(this->rank->x() - this->title->x() - 25);
        }
    }

    bool ListActivity::textReady(Aether::Text * element) {
        return (element->string().empty() ? true : element->ready());
    }

    void ListActivity::setImage(uint8_t * ptr, uint32_t size) {
        // Remove original
        this->removeElement(this->icon);

        // Add new icon and render it asynchronously
        this->icon = new Aether::Image(0, 0, ptr, size, Aether::Render::Wait);
        this->icon->setHidden(true);
        this->icon->setXYWH(this->x() + 10, this->y() + 10, this->h() - 20, this->h() - 20);
        this->icon->setScaleDimensions(this->icon->w(), this->icon->h());
        this->icon->renderAsync();
        this->addElement(this->icon);
    }

    void ListActivity::setTitle(const std::string & title) {
        // Remove original
        this->removeElement(this->title);

        // Add new string and render asynchronously
        this->title = new Aether::Text(this->x() + 130, this->y() + 15, title, 22, Aether::Render::Async);
        this->title->setHidden(true);
        this->title->setCanScroll(true);
        this->title->setScrollSpeed(75);
        this->addElement(this->title);
    }

    void ListActivity::setPlaytime(const std::string & playtime) {
        // Remove original
        this->removeElement(this->playtime);

        // Add new string and render asynchronously
        this->playtime = new Aether::Text(this->x() + 130, this->y() + 51, playtime, 18, Aether::Render::Async);
        this->playtime->setHidden(true);
        this->addElement(this->playtime);
    }

    void ListActivity::setLeftMuted(const std::string & text) {
        // Remove original
        this->removeElement(this->mutedLeft);

        // Add new string and render asynchronously
        this->mutedLeft = new Aether::Text(this->x() + 130, this->y() + 82, text, 18, Aether::Render::Async);
        this->mutedLeft->setHidden(true);
        this->addElement(this->mutedLeft);
    }

    void ListActivity::setRightMuted(const std::string & text) {
        // Remove original
        this->removeElement(this->mutedRight);

        // Add new string and render asynchronously
        this->mutedRight = new Aether::Text(this->x() + this->w(), this->y() + 84, text, 16, Aether::Render::Async);
        this->mutedRight->setHidden(true);
        this->addElement(this->mutedRight);
    }

    void ListActivity::setRank(const std::string & text) {
        // Remove original
        this->removeElement(this->rank);

        // Add new string and render asynchronously
        this->rank = new Aether::Text(this->x() + this->w(), this->y() + 15, text, 18, Aether::Render::Async);
        this->rank->setHidden(true);
        this->rank->setColour(this->mutedRight->colour());
        this->addElement(this->rank);
    }

    void ListActivity::update(unsigned int dt) {
        // Wait until all elements are finished rendering, then show
        if (this->rank->hidden()) {
            if (this->icon->ready() && this->textReady(this->title) && this->textReady(this->playtime) && this->textReady(this->mutedLeft) && this->textReady(this->mutedRight) && this->textReady(this->rank)) {
                this->topR->setHidden(false);
                this->bottomR->setHidden(false);
                this->icon->setHidden(false);
                this->title->setHidden(false);
                this->playtime->setHidden(false);
                this->mutedLeft->setHidden(false);
                this->mutedRight->setHidden(false);
                this->rank->setHidden(false);

                this->positionItems();
            }
        }

        Element::update(dt);
    }

    void ListActivity::setTitleColour(const Aether::Colour & c) {
        this->title->setColour(c);
    }

    void ListActivity::setPlaytimeColour(const Aether::Colour & c) {
        this->playtime->setColour(c);
    }

    void ListActivity::setMutedColour(const Aether::Colour & c) {
        this->mutedLeft->setColour(c);
        this->mutedRight->setColour(c);
        this->rank->setColour(c);
    }

    void ListActivity::setLineColour(const Aether::Colour & c) {
        this->topR->setColour(c);
        this->bottomR->setColour(c);
    }

    void ListActivity::setW(int w) {
        Element::setW(w);

        // Adjusting width must move/adjust children
        this->topR->setRectSize(this->w(), 1);
        this->bottomR->setRectSize(this->w(), 1);
        this->positionItems();
    }
};