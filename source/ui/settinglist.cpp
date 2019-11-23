#include "settinglist.hpp"
#include "utils.hpp"

// Gap in pixels between items
#define ITEM_GAP 25

// Decrement scroll velocity by this amount
#define SCROLL_DEC 20

// Offset to delay scroll when need to move
#define DELAY_OFFSET 300
// Amount to scroll per second when button held
#define SCROLL_AMT 1300

namespace UI {
    SettingList::SettingList(bool * b, int x, int y, int w, int h) : Navigable(b, x, y, w, h) {
        this->pos = 0;
        this->t_pos = -1;
        this->is_touched = false;
        this->hi_pos = 0;
    }

    void SettingList::addItem(SettingListItem * item) {
        item->setX(this->x + this->w*0.075);
        item->setW(this->w*0.85);
        item->renderTip();
        this->items.push_back(item);

        // Update maximum scroll pos
        unsigned int total = 0;
        for (size_t i = 0; i < this->items.size(); i++) {
            total += this->items[i]->getH();
        }
        if (total <= h) {
            this->max_pos = 0;
        } else {
            this->max_pos = total;
        }
    }

    void SettingList::update(uint32_t dt) {
        Navigable::update(dt);

        // Handle held button
        if (this->button[Utils::key_map[KEY_DDOWN]].is_pressed && this->button[Utils::key_map[KEY_DDOWN]].time_held > DELAY_OFFSET) {
            if (this->hi_pos < this->items.size()-1) {
                this->hi_pos++;
            }
        } else if (this->button[Utils::key_map[KEY_DUP]].is_pressed && this->button[Utils::key_map[KEY_DUP]].time_held > DELAY_OFFSET) {
            if (this->hi_pos > 0) {
                this->hi_pos--;
            }
        }
    }

    void SettingList::draw() {
        // Draw list items
        unsigned int draw_y = this->y - this->pos + 15;
        for (size_t i = 0; i < this->items.size(); i++) {
            // Highlight appropriate entry
            if (i == this->hi_pos && !(*this->touch_active) && this->is_active) {
                SDLHelper::setColour(this->theme->getHighlight());
                SDLHelper::drawRect(this->items[i]->getX() - 5, this->items[i]->getY() - 5, this->items[i]->getW() + 10, this->items[i]->getBoxHeight() + 10);
                SDLHelper::setColour(this->theme->getHighlightBG());
                SDLHelper::drawRect(this->items[i]->getX(), this->items[i]->getY(), this->items[i]->getW(), this->items[i]->getBoxHeight());
            }

            // List items utilize 85% of list width
            this->items[i]->setY(draw_y);
            this->items[i]->draw();

            // Draw touch overlay if touched
            if (this->t_pos == (int)i) {
                SDLHelper::setColour(this->theme->getTouchOverlay());
                SDLHelper::drawRect(this->items[i]->getX(), this->items[i]->getY(), this->items[i]->getW(), this->items[i]->getBoxHeight());
            }

            // Increment y position
            draw_y += this->items[i]->getH() + ITEM_GAP;
        }

        // Draw side bar
        if (this->max_pos > this->h) {
            SDLHelper::setColour(this->theme->getMutedLine());
            int yPos = this->y + (((float)this->pos / this->max_pos) * (this->h - 95));
            SDLHelper::drawRect(this->x + this->w - 25, yPos, 5, 70);
        }
    }

    void SettingList::touched(uint32_t type, float tx, float ty, float dx, float dy) {
        switch (type) {
            // Pressed
            case SDL_FINGERDOWN:
                // Loop over shown controls and set touched if so
                for (size_t i = 0; i < this->items.size(); i++) {
                    if (tx >= this->items[i]->getX() && tx <= this->items[i]->getX() + this->items[i]->getW() && ty >= this->items[i]->getY() && ty <= this->items[i]->getY() + this->items[i]->getBoxHeight()) {
                        this->t_pos = i;
                        break;
                    }
                }
                this->is_touched = true;
                break;

            // Moved
            case SDL_FINGERMOTION:
                if (this->t_pos == -1){
                    return;
                }

                // If moved outside of selected button treat as lifted finger
                if (tx < this->items[this->t_pos]->getX() || tx > this->items[this->t_pos]->getX() + this->items[this->t_pos]->getW() || ty < this->items[this->t_pos]->getY() || ty > this->items[this->t_pos]->getY() + this->items[this->t_pos]->getBoxHeight()) {
                    this->touched(SDL_FINGERUP, tx, ty);
                }
                break;

            // Released
            case SDL_FINGERUP:
                if (this->t_pos == -1){
                    return;
                }

                // If inside of same button set selected
                if (tx >= this->items[this->t_pos]->getX() && tx <= this->items[this->t_pos]->getX() + this->items[this->t_pos]->getW() && ty >= this->items[this->t_pos]->getY() && ty <= this->items[this->t_pos]->getY() + this->items[this->t_pos]->getBoxHeight()) {
                    this->pos = this->t_pos;
                    this->items[this->t_pos]->pressed();
                }

                this->t_pos = -1;
                this->is_touched = false;
                break;
        }
    }

    bool SettingList::isTouched() {
        return this->is_touched;
    }

    void SettingList::handleButton(uint8_t button, uint8_t state) {
        Navigable::handleButton(button, state);

        // Perform action on tap
        if (state == SDL_PRESSED) {
            if (button == Utils::key_map[KEY_DDOWN]) {
                if (this->hi_pos < this->items.size()-1) {
                    this->hi_pos++;
                    this->button[Utils::key_map[KEY_DDOWN]].time_held = 0;
                } else {
                    this->button[Utils::key_map[KEY_DDOWN]].time_held = DELAY_OFFSET;
                }
            } else if (button == Utils::key_map[KEY_DUP]) {
                if (this->hi_pos > 0) {
                    this->hi_pos--;
                    this->button[Utils::key_map[KEY_DUP]].time_held = 0;
                } else {
                    this->button[Utils::key_map[KEY_DUP]].time_held = DELAY_OFFSET;
                }
            } else if (button == Utils::key_map[KEY_A]) {
                this->items[this->hi_pos]->pressed();
            }
        }
    }

    unsigned int SettingList::getPos() {
        return this->pos;
    }

    void SettingList::setPos(unsigned int pos) {
        // Avoid going from 0 to max uint val
        if (this->pos == 0 && pos > this->max_pos) {
            return;
        }
        this->pos = pos;
        if (this->pos > this->max_pos) {
            this->pos = this->max_pos;
        }
    }

    SettingList::~SettingList() {
        // Delete ListItem objects
        while (this->items.size() > 0) {
            delete this->items[0];
            this->items.erase(this->items.begin());
        }
    }
};