#include "ui/element/ListAdjust.hpp"

namespace CustomElm {
    ListAdjust::ListAdjust(const std::string & title, const std::string & record, const std::string & adjust) {
        this->setH(ListAdjust::height);

        this->topR = new Aether::Rectangle(this->x(), this->y(), this->w(), 1);
        this->addElement(this->topR);
        this->bottomR = new Aether::Rectangle(this->x(), this->y() + this->h(), this->w(), 1);
        this->addElement(this->bottomR);

        // Create elements
        this->icon       = new Aether::Image(0, 0, "", Aether::Render::Wait);
        this->title      = new Aether::Text(0, 0, title, 20, Aether::Render::Wait);
        this->title->setCanScroll(true);
        this->title->setScrollSpeed(75);
        this->recordTime = new Aether::Text(0, 0, record, 16, Aether::Render::Wait);
        this->adjustTime = new Aether::Text(0, 0, adjust, 16, Aether::Render::Wait);

        this->addElement(this->title);
        this->addTexture(this->title);
        this->addElement(this->recordTime);
        this->addTexture(this->recordTime);
        this->addElement(this->adjustTime);
        this->addTexture(this->adjustTime);
    }

    void ListAdjust::positionElements() {
        this->topR->setRectSize(this->w(), 1);
        this->bottomR->setRectSize(this->w(), 1);

        this->icon->setXY(this->x() + 10, this->y() + 10);
        this->title->setXY(this->icon->x() + this->icon->w() + 15, this->icon->y() + 3);
        this->recordTime->setXY(this->title->x(), this->title->y() + this->title->h() + 10);
        this->adjustTime->setXY(this->recordTime->x(), this->recordTime->y() + this->recordTime->h() + 6);

        int sz = this->w() - this->icon->w() - 40;
        if (this->title->textureWidth() > sz) {
            this->title->setW(sz);
        }
    }

    void ListAdjust::update(uint32_t dt) {
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

    void ListAdjust::setAdjustedTime(const std::string & str) {
        this->adjustTime->setString(str);
        this->positionElements();
    }

    void ListAdjust::setImage(uint8_t * ptr, uint32_t size) {
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

    void ListAdjust::setAdjustColour(const Aether::Colour & c) {
        this->adjustTime->setColour(c);
    }

    void ListAdjust::setLineColour(const Aether::Colour & c) {
        this->topR->setColour(c);
        this->bottomR->setColour(c);
    }

    void ListAdjust::setRecordColour(const Aether::Colour & c) {
        this->recordTime->setColour(c);
    }

    void ListAdjust::setTitleColour(const Aether::Colour & c) {
        this->title->setColour(c);
    }
};
