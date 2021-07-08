#include "ui/element/ListHide.hpp"

namespace CustomElm {
    ListHide::ListHide(const std::string & title, const std::string & id) : Aether::AsyncItem() {
        Element::setH(ListHide::height);

        // Create our bounding rectangles
        this->topR = new Aether::Rectangle(this->x(), this->y(), this->w(), 1);
        this->addElement(this->topR);
        this->bottomR = new Aether::Rectangle(this->x(), this->y() + this->h(), this->w(), 1);
        this->addElement(this->bottomR);

        // Create tick
        this->circle = new Aether::Box(0, 0, this->h() - 50, this->h() - 50, 2, (this->h() - 48)/2);
        this->tick = new Aether::Tick(0, 0, this->h() - 50);

        this->addElement(this->circle);
        this->addElement(this->tick);

        // Create elements
        this->icon       = new Aether::Image(0, 0, "", Aether::Render::Wait);
        this->title      = new Aether::Text(0, 0, title, 22, Aether::Render::Wait);
        this->title->setCanScroll(true);
        this->title->setScrollSpeed(75);
        this->titleID    = new Aether::Text(0, 0, id, 16, Aether::Render::Wait);

        this->addElement(this->title);
        this->addTexture(this->title);
        this->addElement(this->titleID);
        this->addTexture(this->titleID);
    }

    void ListHide::positionElements() {
        this->topR->setRectSize(this->w(), 1);
        this->bottomR->setRectSize(this->w(), 1);

        this->icon->setXY(this->x() + 10, this->y() + 10);
        this->title->setXY(this->icon->x() + this->icon->w() + 15, this->icon->y() + 5);
        this->titleID->setXY(this->title->x(), this->title->y() + this->title->h() + 10);
        this->circle->setXY(this->x() + this->w() - 20 - this->circle->w(), this->y() + 25);
        this->tick->setXY(this->circle->x(), this->circle->y());

        int sz = this->w() - this->icon->w() - this->tick->w() - 60;
        if (this->title->textureWidth() > sz) {
            this->title->setW(sz);
        }
    }

    void ListHide::update(uint32_t dt) {
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

        // Same with tick + circle
        if (!this->tick->hidden()) {
            col = this->tick->getCircleColour();
            col.setA(this->icon->colour().a());
            this->tick->setCircleColour(col);

            col = this->tick->getTickColour();
            col.setA(this->icon->colour().a());
            this->tick->setTickColour(col);
        }

        if (!this->circle->hidden()) {
            col = this->circle->colour();
            col.setA(this->icon->colour().a());
            this->circle->setColour(col);
        }
    }

    void ListHide::setImage(uint8_t * ptr, uint32_t size) {
        // Remove original
        this->removeTexture(this->icon);
        this->removeElement(this->icon);

        // Add new icon and render it asynchronously
        this->icon = new Aether::Image(0, 0, ptr, size, Aether::Render::Wait);
        this->icon->setWH(this->h() - 20, this->h() - 20);
        this->icon->setScaleDimensions(this->icon->w(), this->icon->h());
        this->icon->setColour(Aether::Colour(255, 255, 255, 0));
        this->addElement(this->icon);
        this->addTexture(this->icon);
    }

    void ListHide::setIDColour(const Aether::Colour & c) {
        this->titleID->setColour(c);
    }

    void ListHide::setLineColour(const Aether::Colour & c) {
        this->topR->setColour(c);
        this->bottomR->setColour(c);
    }

    void ListHide::setTitleColour(const Aether::Colour & c) {
        this->title->setColour(c);
    }

    void ListHide::setTickBackgroundColour(const Aether::Colour & c) {
        Aether::Colour col = c;
        col.setA(0);
        this->tick->setCircleColour(col);
    }

    void ListHide::setTickForegroundColour(const Aether::Colour & c) {
        Aether::Colour col = c;
        col.setA(0);
        this->tick->setTickColour(col);
        this->circle->setColour(col);
    }

    bool ListHide::isTicked() {
        return (!this->tick->hidden());
    }

    void ListHide::setTicked(const bool b) {
        this->tick->setHidden(!b);
        this->circle->setHidden(b);
    }
};
