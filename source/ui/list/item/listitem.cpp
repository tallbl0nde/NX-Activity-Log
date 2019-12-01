#include "listitem.hpp"

namespace UI {
    List_Item::List_Item() : Drawable() {
        this->is_selectable = false;
        this->is_selected = false;
        this->offset = 0;
    }

    void List_Item::update(uint32_t dt) {
        Drawable::update(dt);
    }

    void List_Item::draw() {

    }

    void List_Item::pressed() {

    }

    int List_Item::getOffset() {
        return this->offset;
    }

    void List_Item::setOffset(int o) {
        this->offset = o;
    }

    void List_Item::setSelected(bool b) {
        this->is_selected = b;
    }

    bool List_Item::isSelectable() {
        return this->is_selectable;
    }

    List_Item::~List_Item() {

    }
};