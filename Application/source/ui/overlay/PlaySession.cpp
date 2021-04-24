#include "utils/Lang.hpp"
#include "ui/overlay/PlaySession.hpp"

#define WIDTH 800

namespace CustomOvl {
    PlaySession::PlaySession() : Overlay() {
        // Create elements
        this->rect = new Aether::Rectangle(this->x() + (this->w() - WIDTH)/2, this->y(), WIDTH, this->h());
        this->addElement(this->rect);
        this->title = new Aether::Text(this->rect->x() + 50, this->rect->y() + 45, "details.break.heading"_lang, 28);
        this->title->setY(this->title->y() - this->title->h()/2);
        this->addElement(this->title);
        this->top = new Aether::Rectangle(this->rect->x() + 30, this->rect->y() + 87, this->rect->w() - 60, 1);
        this->addElement(this->top);
        this->bottom = new Aether::Rectangle(this->rect->x() + 30, this->rect->y() + 647, this->rect->w() - 60, 1);
        this->addElement(this->bottom);
        this->sep = new Aether::Rectangle(this->rect->x() + 30, this->bottom->y() - 110, this->bottom->w(), 1);
        this->addElement(this->sep);
        this->ctrlBar = new Aether::ControlBar();
        this->ctrlBar->setXYWH(this->rect->x() + 45, 647, this->rect->w() - 90, this->ctrlBar->h());
        this->ctrlBar->addControl(Aether::Button::B, "common.close"_lang);
        this->addElement(this->ctrlBar);

        this->list = nullptr;
        this->col = Aether::Colour{255, 255, 255, 255};

        this->length = new Aether::Text(this->rect->x() + 3*this->rect->w()/4, this->bottom->y() - 80, "details.break.length"_lang, 20);
        this->length->setX(this->length->x() - this->length->w()/2);
        this->addElement(this->length);
        this->lengthSub = new Aether::Text(this->rect->x() + 3*this->rect->w()/4, this->bottom->y() - 50, "", 18);
        this->addElement(this->lengthSub);
        this->playtime = new Aether::Text(this->rect->x() + this->rect->w()/4, this->bottom->y() - 80, "details.break.playtime"_lang, 20);
        this->playtime->setX(this->playtime->x() - this->playtime->w()/2);
        this->addElement(this->playtime);
        this->playtimeSub = new Aether::Text(this->rect->x() + this->rect->w()/4, this->bottom->y() - 50, "", 18);
        this->addElement(this->playtimeSub);

        // Close on B
        this->onButtonPress(Aether::Button::B, [this](){
            this->close();
            this->removeElement(this->list);
            this->list = nullptr;
        });
    }

    void PlaySession::addListItem(Aether::Element * e) {
        if (this->list == nullptr) {
            this->list = new Aether::List(this->rect->x(), this->top->y() + 1, this->rect->w() - 50, this->sep->y() - this->top->y() - 1);
            this->list->setScrollBarColour(this->col);
            this->addElement(this->list);
            this->setFocussed(this->list);
        }
        this->list->addElement(e);
    }

    void PlaySession::setLength(std::string s) {
        this->lengthSub->setString(s);
        this->lengthSub->setX(this->rect->x() + 3*this->rect->w()/4 - this->lengthSub->w()/2);
    }

    void PlaySession::setPlaytime(std::string s) {
        this->playtimeSub->setString(s);
        this->playtimeSub->setX(this->rect->x() + this->rect->w()/4 - this->playtimeSub->w()/2);
    }

    void PlaySession::setAccentColour(Aether::Colour c) {
        this->lengthSub->setColour(c);
        this->playtimeSub->setColour(c);
    }

    void PlaySession::setBackgroundColour(Aether::Colour c) {
        this->rect->setColour(c);
    }

    void PlaySession::setLineColour(Aether::Colour c) {
        this->top->setColour(c);
        this->bottom->setColour(c);
        this->sep->setColour(c);
    }

    void PlaySession::setMutedLineColour(Aether::Colour c) {
        this->col = c;
    }

    void PlaySession::setTextColour(Aether::Colour c) {
        this->length->setColour(c);
        this->playtime->setColour(c);
        this->title->setColour(c);
        this->ctrlBar->setDisabledColour(c);
        this->ctrlBar->setEnabledColour(c);
    }
};