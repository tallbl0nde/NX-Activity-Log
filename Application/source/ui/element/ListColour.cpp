#include "ui/element/ListColour.hpp"
#include "utils/Utils.hpp"

// Height of element
#define HEIGHT 70
// Padding on left/right of text
#define TEXT_PADDING 16

// Formats colour as hex string
static std::string RGBtoHex(Aether::Colour c) {
    char str[10];
    snprintf(str, 10, "#%02x%02x%02x%02x", c.r(), c.g(), c.b(), c.a());
    return std::string(str);
}

namespace CustomElm {
    ListColour::ListColour(std::string s, std::function<void()> f) : Aether::Element() {
        this->setH(HEIGHT);
        this->topR = new Aether::Rectangle(this->x(), this->y(), this->w(), 1);
        this->addElement(topR);
        this->bottomR = new Aether::Rectangle(this->x(), this->y() + this->h(), this->w(), 1);
        this->addElement(bottomR);
        this->text = new Aether::Text(this->x() + TEXT_PADDING, this->y() + this->h()/2, s, 22);
        this->text->setY(this->text->y() - this->text->h()/2);
        this->addElement(this->text);
        this->colour = new Aether::Rectangle(this->x() + this->w() - TEXT_PADDING - 100, this->y() + this->h()/2 - 20, 100, 40);
        this->addElement(this->colour);
        this->hex = new Aether::Text(this->x(), this->y(), "#", 18);
        this->hex->setY(this->y() + (this->h() - this->hex->h())/2);
        this->addElement(this->hex);
        this->onPress(f);
    }

    void ListColour::setColour(Aether::Colour c) {
        this->colour->setColour(c);
        this->hex->setString(RGBtoHex(c));
        this->hex->setX(this->colour->x() - (1.5 * TEXT_PADDING) - this->hex->w());
    }

    void ListColour::setLineColour(Aether::Colour c) {
        this->topR->setColour(c);
        this->bottomR->setColour(c);
    }

    void ListColour::setTextColour(Aether::Colour c) {
        this->hex->setColour(c);
        this->text->setColour(c);
    }

    void ListColour::setW(int w) {
        Element::setW(w);
        this->topR->setRectSize(w, 1);
        this->bottomR->setRectSize(w, 1);
        this->colour->setX(this->x() + this->w() - TEXT_PADDING - 100);
        this->hex->setX(this->colour->x() - (1.5 * TEXT_PADDING) - this->hex->w());
    }
};