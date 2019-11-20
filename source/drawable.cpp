#include "drawable.hpp"

namespace UI {
    // Create theme object
    Theme * Drawable::theme = new Theme();

    Drawable::Drawable(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    void Drawable::update(uint32_t dt) {

    }

    Drawable::~Drawable() {

    }

    int Drawable::getX() {
        return this->x;
    }

    int Drawable::getY() {
        return this->y;
    }

    int Drawable::getW() {
        return this->w;
    }

    int Drawable::getH() {
        return this->h;
    }

    void Drawable::setX(int x) {
        this->x = x;
    }

    void Drawable::setY(int y) {
        this->y = y;
    }

    void Drawable::setW(int w) {
        this->w = w;
    }

    void Drawable::setH(int h) {
        this->h = h;
    }

};