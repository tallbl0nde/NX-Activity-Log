#include "ui/overlay/ProgressBox.hpp"

#define WIDTH 560

namespace CustomOvl {
    ProgressBox::ProgressBox() : Aether::Overlay() {
        // Create background rectangle
        this->rect = new Aether::Rectangle((this->w() - WIDTH)/2, this->h()/2, WIDTH, 1, 8);
        this->addElement(this->rect);

        // Create heading
        this->heading = new Aether::TextBlock(this->rect->x() + 50, this->rect->y() + 35, "|", 24, this->rect->w() - 100);
        this->addElement(this->heading);

        // Create progress bar + value
        this->bar = new Aether::RoundProgressBar(this->heading->x(), this->heading->y() + this->heading->h() + 25, this->rect->w() - 160);
        this->bar->setValue(0);
        this->addElement(this->bar);

        this->value = new Aether::Text(this->bar->x() + this->bar->w() + 25, 0, "0%", 18);
        this->value->setY(this->bar->y() + (this->bar->h() - this->value->h())/2);
        this->addElement(this->value);

        // Resize rectangle + position elements
        this->rect->setRectSize(this->rect->w(), (this->bar->y() + this->bar->h()) - this->heading->y() + 75);
        this->rect->setY((this->h() - this->rect->h())/2);
        this->heading->setY(this->heading->y() - this->rect->h()/2);
        this->bar->setY(this->bar->y() - this->rect->h()/2);
        this->value->setY(this->value->y() - this->rect->h()/2);

        // Disable closing by button press
        this->onButtonPress(Aether::Button::B, nullptr);
    }

    void ProgressBox::setHeading(const std::string & str) {
        this->heading->setString(str);
    }

    void ProgressBox::setValue(const double value) {
        this->bar->setValue(value);
        this->value->setString(std::to_string(static_cast<int>(value)) + "%");
    }

    void ProgressBox::setBackgroundColour(const Aether::Colour c) {
        this->rect->setColour(c);
    }

    void ProgressBox::setBarBackgroundColour(const Aether::Colour c) {
        this->bar->setBackgroundColour(c);
    }

    void ProgressBox::setBarForegroundColour(const Aether::Colour c) {
        this->bar->setForegroundColour(c);
    }

    void ProgressBox::setTextColour(const Aether::Colour c) {
        this->heading->setColour(c);
        this->value->setColour(c);
    }
};
