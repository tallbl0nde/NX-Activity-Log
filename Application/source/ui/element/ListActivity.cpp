#include "ui/element/ListActivity.hpp"

namespace CustomElm {
    ListActivity::ListActivity() : AsyncItem() {
        Element::setH(ListActivity::height);

        // Create our 'bounding' rectangles
        this->topR = new Aether::Rectangle(this->x(), this->y(), this->w(), 1);
        this->addElement(this->topR);
        this->bottomR = new Aether::Rectangle(this->x(), this->y() + this->h(), this->w(), 1);
        this->addElement(this->bottomR);

        // Start with blank elements
        this->icon       = new Aether::Image(0, 0, "", Aether::Render::Wait);
        this->title      = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->playtime   = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->mutedLeft  = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->mutedRight = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
        this->rank       = new Aether::Text(0, 0, "", 20, Aether::Render::Wait);
    }

    void ListActivity::positionElements() {
        this->topR->setRectSize(this->w(), 1);
        this->bottomR->setRectSize(this->w(), 1);

        this->mutedRight->setX(this->x() + this->w() - this->mutedRight->w() - 15);
        this->rank->setX(this->x() + this->w() - this->rank->w() - 15);

        if (this->title->textureWidth() > this->rank->x() - this->title->x() - 25) {
            this->title->setW(this->rank->x() - this->title->x() - 25);
        }
    }

    void ListActivity::processText(Aether::Text * & text, std::function<Aether::Text * ()> getNew) {
        // Remove original
        this->removeTexture(text);
        this->removeElement(text);

        // Get (and assign) new text object
        text = getNew();

        // Don't add if empty string
        if (!text->string().empty()) {
            this->addElement(text);
            this->addTexture(text);
        }
    }

    void ListActivity::update(uint32_t dt) {
        // Update normally first
        AsyncItem::update(dt);

        // Match rectangle alphas with the icon's
        Aether::Colour col = this->topR->colour();
        col.setA(this->icon->colour().a());
        this->topR->setColour(col);
        this->bottomR->setColour(col);

        bool hide = (this->icon->hidden());
        this->topR->setHidden(hide);
        this->bottomR->setHidden(hide);
    }

    void ListActivity::setImage(uint8_t * ptr, uint32_t size) {
        // Remove original
        this->removeTexture(this->icon);
        this->removeElement(this->icon);

        // Add new icon and render it asynchronously
        this->icon = new Aether::Image(0, 0, ptr, size, Aether::Render::Wait);
        this->icon->setXYWH(this->x() + 10, this->y() + 10, this->h() - 20, this->h() - 20);
        this->icon->setScaleDimensions(this->icon->w(), this->icon->h());
        this->addElement(this->icon);
        this->addTexture(this->icon);
    }

    void ListActivity::setTitle(const std::string & title) {
        this->processText(this->title, [this, title]() {
            Aether::Text * t = new Aether::Text(this->x() + 130, this->y() + 15, title, 22, Aether::Render::Wait);
            t->setCanScroll(true);
            t->setScrollSpeed(75);
            return t;
        });
    }

    void ListActivity::setPlaytime(const std::string & playtime) {
        this->processText(this->playtime, [this, playtime]() {
            return new Aether::Text(this->x() + 130, this->y() + 51, playtime, 18, Aether::Render::Wait);
        });
    }

    void ListActivity::setLeftMuted(const std::string & text) {
        this->processText(this->mutedLeft, [this, text]() {
            return new Aether::Text(this->x() + 130, this->y() + 82, text, 18, Aether::Render::Wait);
        });
    }

    void ListActivity::setRightMuted(const std::string & text) {
        this->processText(this->mutedRight, [this, text]() {
            return new Aether::Text(this->x() + this->w(), this->y() + 84, text, 16, Aether::Render::Wait);
        });
    }

    void ListActivity::setRank(const std::string & text) {
        this->processText(this->rank, [this, text]() {
            Aether::Text * t = new Aether::Text(this->x() + this->w(), this->y() + 15, text, 18, Aether::Render::Wait);
            t->setColour(this->mutedRight->colour());
            return t;
        });
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
};
