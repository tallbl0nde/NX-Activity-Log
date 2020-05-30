#include "ui/element/ListSession.hpp"

// Default values
#define FONT_SIZE 20
#define HEIGHT 85

namespace CustomElm {
    ListSession::ListSession() {
        this->setH(HEIGHT);

        this->top = new Aether::Rectangle(0, 0, this->w(), 1);
        this->addElement(this->top);
        this->bottom = new Aether::Rectangle(0, this->h(), this->w(), 1);
        this->addElement(this->bottom);
        this->time = new Aether::Text(this->x() + 16, this->y() + 15, "", FONT_SIZE);
        this->addElement(this->time);
        this->playtime = new Aether::Text(this->x() + 16, this->y() + 50, "", FONT_SIZE);
        this->addElement(this->playtime);
        this->percentage = new Aether::Text(this->x() + 16, this->y() + 50, "", FONT_SIZE);
        this->addElement(this->percentage);
    }

    void ListSession::setLineColour(Aether::Colour c) {
        this->top->setColour(c);
        this->bottom->setColour(c);
    }

    void ListSession::setPercentageColour(Aether::Colour c) {
        this->percentage->setColour(c);
    }

    void ListSession::setPlaytimeColour(Aether::Colour c) {
        this->playtime->setColour(c);
    }

    void ListSession::setTimeColour(Aether::Colour c) {
        this->time->setColour(c);
    }

    void ListSession::setPercentageString(std::string s) {
        this->percentage->setString(s);
        this->percentage->setX(this->x() + this->w() - 16 - this->percentage->w());
    }

    void ListSession::setPlaytimeString(std::string s) {
        this->playtime->setString(s);
    }

    void ListSession::setTimeString(std::string s) {
        this->time->setString(s);
    }

    void ListSession::setW(int w) {
        Element::setW(w);
        this->top->setRectSize(this->w(), 1);
        this->bottom->setRectSize(this->w(), 1);
        this->percentage->setX(this->x() + this->w() - 16 - this->percentage->w());
    }
};
