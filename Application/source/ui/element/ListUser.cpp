#include "ui/element/ListUser.hpp"

#define HEIGHT 90

namespace CustomElm {
    ListUser::ListUser(std::string n, uint8_t * p, uint32_t s) : Aether::Element() {
        this->setH(HEIGHT);
        this->topR = new Aether::Rectangle(this->x(), this->y(), this->w(), 1);
        this->addElement(topR);
        this->bottomR = new Aether::Rectangle(this->x(), this->y() + this->h(), this->w(), 1);
        this->addElement(bottomR);
        Aether::Image * im = new Aether::Image(this->x() + 10, this->y() + 10, p, s, Aether::Render::Wait);
        im->setWH(70, 70);
        im->setScaleDimensions(70, 70);
        im->renderSync();
        this->addElement(im);
        this->name = new Aether::Text(this->x() + 105, this->y() + this->h()/2, n, 24);
        this->name->setY(this->name->y() - this->name->h()/2);
        this->addElement(this->name);
    }

    void ListUser::setLineColour(Aether::Colour c) {
        this->topR->setColour(c);
        this->bottomR->setColour(c);
    }

    void ListUser::setTextColour(Aether::Colour c) {
        this->name->setColour(c);
    }

    void ListUser::setW(int w) {
        Element::setW(w);
        this->topR->setRectSize(w, 1);
        this->bottomR->setRectSize(w, 1);
    }
};
