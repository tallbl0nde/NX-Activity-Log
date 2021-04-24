#include "ui/overlay/ColourPicker.hpp"

// Defaults
#define HEIGHT 470
#define TITLE_FONT_SIZE 26

// Formats colour as hex string
static std::string RGBtoHex(Aether::Colour c) {
    char str[10];
    snprintf(str, 10, "#%02x%02x%02x%02x", c.r(), c.g(), c.b(), c.a());
    return std::string(str);
}

namespace CustomOvl {
    ColourPicker::ColourPicker(std::string s, Aether::Colour c, std::function<void(Aether::Colour)> f) : Aether::Overlay() {
        this->func = f;

        // Create elements
        this->rect = new Aether::Rectangle(this->x(), this->y() + this->h() - HEIGHT, this->w(), HEIGHT);
        this->addElement(this->rect);
        this->top = new Aether::Rectangle(this->x() + 30, this->rect->y() + 72, this->w() - 60, 1);
        this->addElement(this->top);
        this->bottom = new Aether::Rectangle(this->x() + 30, this->rect->y() + this->rect->h() - 72, this->w() - 60, 1);
        this->addElement(this->bottom);

        this->title = new Aether::Text(this->x() + 72, this->rect->y() + 40, s, TITLE_FONT_SIZE);
        this->title->setY(this->title->y() - this->title->h()/2);
        this->addElement(this->title);

        this->ctrlBar = new Aether::ControlBar();
        this->ctrlBar->addControl(Aether::Button::A, "OK");
        this->ctrlBar->addControl(Aether::Button::B, "Back");
        this->addElement(this->ctrlBar);

        this->tip = new Aether::Text(this->x() + 72, this->bottom->y() + 36, "Tip:", 20);
        this->tip->setY(this->tip->y() - this->tip->h()/2);
        this->addElement(this->tip);

        // Spinners
        int y = this->top->y() + (this->bottom->y() - this->top->y())/2;
        this->red = new Aether::Spinner(this->rect->x() + 220, 0, 120);
        this->red->setMin(0);
        this->red->setMax(255);
        this->red->setLabel("Red");
        this->red->setValue(c.r());
        this->red->setY(y - this->red->h()/2);
        this->addElement(this->red);

        this->green = new Aether::Spinner(this->red->x() + 150, 0, 120);
        this->green->setMin(0);
        this->green->setMax(255);
        this->green->setLabel("Green");
        this->green->setValue(c.g());
        this->green->setY(y - this->green->h()/2);
        this->addElement(this->green);

        this->blue = new Aether::Spinner(this->green->x() + 150, 0, 120);
        this->blue->setMin(0);
        this->blue->setMax(255);
        this->blue->setLabel("Blue");
        this->blue->setValue(c.b());
        this->blue->setY(y - this->blue->h()/2);
        this->addElement(this->blue);

        this->alpha = new Aether::Spinner(this->blue->x() + 150, 0, 120);
        this->alpha->setMin(0);
        this->alpha->setMax(255);
        this->alpha->setLabel("Alpha");
        this->alpha->setValue(c.a());
        this->alpha->setY(y - this->alpha->h()/2);
        this->addElement(this->alpha);

        // OK Button
        this->button = new Aether::BorderButton(this->alpha->x() + 250, 0, 160, 60, 3, "OK", 22, [this](){
            this->callFunc();
            this->close();
        });
        this->button->setY(y - this->button->h()/2 - 26);
        this->addElement(this->button);

        // Preview
        this->colourRect = new Aether::Rectangle(this->rect->x() + this->rect->w() - 192, this->rect->y() + 17, 120, 40);
        this->colourRect->setColour(c);
        this->addElement(this->colourRect);
        this->colourHex = new Aether::Text(0, this->colourRect->y() + this->colourRect->h()/2, RGBtoHex(c), 20);
        this->colourHex->setX(this->colourRect->x() - 24 - this->colourHex->w());
        this->colourHex->setY(this->colourRect->y() + (this->colourRect->h() - this->colourHex->h())/2);
        this->addElement(this->colourHex);

        // Close without updating
        this->onButtonPress(Aether::Button::B, [this](){
            this->close();
        });
        // Close and update
        this->onButtonPress(Aether::Button::A, [this](){
            this->callFunc();
            this->close();
        });

        // Change speed when ZL/ZR pressed
        this->onButtonPress(Aether::Button::ZR, [this]() {
            this->red->setChangeAmount(10);
            this->green->setChangeAmount(10);
            this->blue->setChangeAmount(10);
            this->alpha->setChangeAmount(10);
        });
        this->onButtonRelease(Aether::Button::ZR, [this]() {
            this->red->setChangeAmount(1);
            this->green->setChangeAmount(1);
            this->blue->setChangeAmount(1);
            this->alpha->setChangeAmount(1);
        });
        this->onButtonPress(Aether::Button::ZL, [this]() {
            this->red->setChangeAmount(10);
            this->green->setChangeAmount(10);
            this->blue->setChangeAmount(10);
            this->alpha->setChangeAmount(10);
        });
        this->onButtonRelease(Aether::Button::ZL, [this]() {
            this->red->setChangeAmount(1);
            this->green->setChangeAmount(1);
            this->blue->setChangeAmount(1);
            this->alpha->setChangeAmount(1);
        });
    }

    void ColourPicker::callFunc() {
        this->func(this->colourRect->colour());
    }

    void ColourPicker::update(uint32_t dt) {
        Aether::Overlay::update(dt);

        // Adjust colour text and rectangle
        Aether::Colour c = Aether::Colour(this->red->value(), this->green->value(), this->blue->value(), this->alpha->value());
        this->colourRect->setColour(c);
        this->colourHex->setString(RGBtoHex(c));
        this->colourHex->setX(this->colourRect->x() - 24 - this->colourHex->w());
    }

    void ColourPicker::setBackLabel(std::string s) {
        this->ctrlBar->updateControl(Aether::Button::B, s);
    }

    void ColourPicker::setOKLabel(std::string s) {
        this->ctrlBar->updateControl(Aether::Button::A, s);
    }

    void ColourPicker::setTipText(std::string s) {
        this->tip->setString(s);
    }

    void ColourPicker::setRedHint(std::string s) {
        this->red->setLabel(s);
    }

    void ColourPicker::setGreenHint(std::string s) {
        this->green->setLabel(s);
    }

    void ColourPicker::setBlueHint(std::string s) {
        this->blue->setLabel(s);
    }

    void ColourPicker::setAlphaHint(std::string s) {
        this->alpha->setLabel(s);
    }

    void ColourPicker::setBackgroundColour(Aether::Colour c) {
        this->rect->setColour(c);
    }

    void ColourPicker::setInactiveColour(Aether::Colour c) {
        this->red->setArrowColour(c);
        this->green->setArrowColour(c);
        this->blue->setArrowColour(c);
        this->alpha->setArrowColour(c);
        this->tip->setColour(c);
    }

    void ColourPicker::setHighlightColour(Aether::Colour c) {
        this->red->setHighlightColour(c);
        this->green->setHighlightColour(c);
        this->blue->setHighlightColour(c);
        this->alpha->setHighlightColour(c);
    }

    void ColourPicker::setTextColour(Aether::Colour c) {
        this->red->setTextColour(c);
        this->green->setTextColour(c);
        this->blue->setTextColour(c);
        this->alpha->setTextColour(c);
        this->button->setBorderColour(c);
        this->button->setTextColour(c);
        this->title->setColour(c);
        this->top->setColour(c);
        this->bottom->setColour(c);
        this->ctrlBar->setDisabledColour(c);
        this->ctrlBar->setEnabledColour(c);
        this->colourHex->setColour(c);
    }
};