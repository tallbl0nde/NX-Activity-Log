#include "ui/overlay/PlaytimePicker.hpp"

// Defaults
#define HEIGHT 470
#define TITLE_FONT_SIZE 26

namespace CustomOvl {
    PlaytimePicker::PlaytimePicker(const std::string & s, const int val, std::function<void(int)> f) : Aether::Overlay() {
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
        this->hour = new Aether::Spinner(this->rect->x() + 280, 0, 120);
        this->hour->setMin(-999);
        this->hour->setMax(999);
        this->hour->setLabel("Hour");
        this->hour->setValue(val / 3600);
        this->hour->setY(y - this->hour->h()/2);
        this->addElement(this->hour);

        this->minute = new Aether::Spinner(this->hour->x() + 150, 0, 120);
        this->minute->setDigits(2);
        this->minute->setMin(-59);
        this->minute->setMax(59);
        this->minute->setLabel("Minute");
        this->minute->setValue((val / 60) % 60);
        this->minute->setY(y - this->minute->h()/2);
        this->addElement(this->minute);

        this->second = new Aether::Spinner(this->minute->x() + 150, 0, 120);
        this->second->setDigits(2);
        this->second->setMin(-59);
        this->second->setMax(59);
        this->second->setLabel("Second");
        this->second->setValue(val % 60);
        this->second->setY(y - this->second->h()/2);
        this->addElement(this->second);

        // OK Button
        this->button = new Aether::BorderButton(this->second->x() + 250, 0, 160, 60, 3, "OK", 22, [this](){
            this->callFunc();
            this->close();
        });
        this->button->setY(y - this->button->h()/2 - 26);
        this->addElement(this->button);

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
            this->hour->setChangeAmount(10);
            this->minute->setChangeAmount(10);
            this->second->setChangeAmount(10);
        });
        this->onButtonRelease(Aether::Button::ZR, [this]() {
            this->hour->setChangeAmount(1);
            this->minute->setChangeAmount(1);
            this->second->setChangeAmount(1);
        });
        this->onButtonPress(Aether::Button::ZL, [this]() {
            this->hour->setChangeAmount(10);
            this->minute->setChangeAmount(10);
            this->second->setChangeAmount(10);
        });
        this->onButtonRelease(Aether::Button::ZL, [this]() {
            this->hour->setChangeAmount(1);
            this->minute->setChangeAmount(1);
            this->second->setChangeAmount(1);
        });
    }

    void PlaytimePicker::callFunc() {
        this->func((this->hour->value() * 3600) + (this->minute->value() * 60) + this->second->value());
    }

    void PlaytimePicker::setBackLabel(const std::string & s) {
        this->ctrlBar->updateControl(Aether::Button::B, s);
    }

    void PlaytimePicker::setOKLabel(const std::string & s) {
        this->ctrlBar->updateControl(Aether::Button::A, s);
    }

    void PlaytimePicker::setTipText(const std::string & s) {
        this->tip->setString(s);
    }

    void PlaytimePicker::setHourHint(const std::string & s) {
        this->hour->setLabel(s);
    }

    void PlaytimePicker::setMinuteHint(const std::string & s) {
        this->minute->setLabel(s);
    }

    void PlaytimePicker::setSecondHint(const std::string & s) {
        this->second->setLabel(s);
    }

    void PlaytimePicker::setBackgroundColour(const Aether::Colour & c) {
        this->rect->setColour(c);
    }

    void PlaytimePicker::setInactiveColour(const Aether::Colour & c) {
        this->hour->setArrowColour(c);
        this->minute->setArrowColour(c);
        this->second->setArrowColour(c);
        this->tip->setColour(c);
    }

    void PlaytimePicker::setHighlightColour(const Aether::Colour & c) {
        this->hour->setHighlightColour(c);
        this->minute->setHighlightColour(c);
        this->second->setHighlightColour(c);
    }

    void PlaytimePicker::setTextColour(const Aether::Colour & c) {
        this->hour->setTextColour(c);
        this->minute->setTextColour(c);
        this->second->setTextColour(c);
        this->button->setBorderColour(c);
        this->button->setTextColour(c);
        this->title->setColour(c);
        this->top->setColour(c);
        this->bottom->setColour(c);
        this->ctrlBar->setDisabledColour(c);
        this->ctrlBar->setEnabledColour(c);
    }
};
