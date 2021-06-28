#include "ui/element/ListSession.hpp"

namespace CustomElm {
    ListSession::ListSession() : Aether::AsyncItem() {
        this->setH(ListSession::height);

        // Bounding rectangles
        this->top = new Aether::Rectangle(0, 0, this->w(), 1);
        this->addElement(this->top);
        this->bottom = new Aether::Rectangle(0, this->h(), this->w(), 1);
        this->addElement(this->bottom);

        // 'Dummy' elements
        this->time       = new Aether::Text(0, 0, "", 1, Aether::Render::Wait);
        this->playtime   = new Aether::Text(0, 0, "", 1, Aether::Render::Wait);
        this->percentage = new Aether::Text(0, 0, "", 1, Aether::Render::Wait);
    }

    void ListSession::positionElements() {
        this->top->setRectSize(this->w(), 1);
        this->bottom->setRectSize(this->w(), 1);
        this->percentage->setX(this->x() + this->w() - 16 - this->percentage->w());
    }

    void ListSession::processText(Aether::Text * & text, std::function<Aether::Text * ()> getNew) {
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

    void ListSession::update(uint32_t dt) {
        // Update normally first
        AsyncItem::update(dt);

        // Match rectangle alphas with the playtime's
        Aether::Colour col = this->top->colour();
        col.setA(this->playtime->colour().a());
        this->top->setColour(col);
        this->bottom->setColour(col);
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
        this->processText(this->percentage, [this, s]() {
            return new Aether::Text(this->x() + 16, this->y() + 50, s, ListSession::fontSize, Aether::Render::Wait);
        });
    }

    void ListSession::setPlaytimeString(std::string s) {
        this->processText(this->playtime, [this, s]() {
            return new Aether::Text(this->x() + 16, this->y() + 50, s, ListSession::fontSize, Aether::Render::Wait);
        });
    }

    void ListSession::setTimeString(std::string s) {
        this->processText(this->time, [this, s]() {
            return new Aether::Text(this->x() + 16, this->y() + 15, s, ListSession::fontSize, Aether::Render::Wait);
        });
    }
};
