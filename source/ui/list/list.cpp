#include <algorithm>
#include "list.hpp"
#include "utils.hpp"

// Offset to delay scroll (in ms) when button is pressed
#define DELAY_OFFSET 300
// Amount to scroll per second when button held
#define SCROLL_AMT 1300
// Decrement scroll velocity by this amount
#define SCROLL_DEC 20
// Multiplier for scroll speed when ZL/ZR held
#define SCROLL_MULT 6
// Maximum scroll velocity
#define SCROLL_V_MAX 50
// Amount touch can deviate (in px) before scrolling
#define TOUCH_RADIUS 30

namespace UI {
    List::List(bool * b, int x, int y, int w, int h, int o) : Navigable(b, x, y, w, h) {
        // Initialize relevant variables
        this->offset = o;
        this->top_item = 0;
        this->is_scrolling = false;
        this->scroll_pos = 0;
        this->max_scroll_pos = 0;
        this->scroll_velocity = 0;
        this->is_touched = false;
        this->highlight_item = 0;
        this->touched_item = -1;
        this->start_touch_y = -1;
        this->chosen = -1;
        this->reset_chosen = false;
    }

    void List::addItem(List_Item * item) {
        // Items take up 85% of list width
        item->setX(this->x + this->w*0.075);
        item->setW(this->w*0.85);
        this->items.push_back(item);

        // Calculate initial highlighted pos
        for (unsigned int i = 0; i < this->items.size(); i++) {
            if (this->items[i]->isSelectable()) {
                this->highlight_item = i;
                break;
            }
        }

        // Update maximum scroll pos
        this->max_scroll_pos = 0;
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setOffset(this->max_scroll_pos);
            this->max_scroll_pos += this->items[i]->getH();
        }
        this->max_scroll_pos += this->offset;

        // If the summed height of all items is less than the list height don't scroll
        if (this->max_scroll_pos <= this->h) {
            this->max_scroll_pos = 0;
            return;
        }

        this->max_scroll_pos -= this->h;
    }

    void List::update(uint32_t dt) {
        Navigable::update(dt);

        // Handle touch scrolling
        if (this->is_scrolling) {
            this->setScrollPos(this->scroll_pos - this->scroll_velocity);
            if (this->scroll_velocity < 0) {
                this->scroll_velocity += SCROLL_DEC * (dt/1000.0);
            } else if (this->scroll_velocity > 0) {
                this->scroll_velocity -= SCROLL_DEC * (dt/1000.0);
            }

            if (this->scroll_velocity > -1 && this->scroll_velocity < 1) {
                this->scroll_velocity = 0;
                this->is_scrolling = false;
            }
        }

        // Handle held button
        if (this->button[Utils::key_map[KEY_DDOWN]].is_pressed && this->button[Utils::key_map[KEY_DDOWN]].time_held > DELAY_OFFSET) {
            // Find max position
            unsigned int max_pos = 0;
            if (this->scroll_pos >= this->max_scroll_pos) {
                max_pos = this->items.size() - 1;
            } else {
                for (size_t i = this->top_item; i < this->items.size(); i++) {
                    // Break if not being drawn
                    if ((this->items[i]->getOffset() + this->items[i]->getH()) > this->scroll_pos + this->h - (int)this->offset) {
                        max_pos = i-1;
                        break;
                    }
                }
            }

            // Scroll if on max position (ie. next selectable element not completely visible) otherwise move highlight
            bool moved = false;
            if (this->highlight_item < max_pos) {
                this->items[this->highlight_item]->setSelected(false);

                // Move to next selectable item
                for (unsigned int i = this->highlight_item + 1; i <= max_pos; i++) {
                    if (this->items[i]->isSelectable()) {
                        moved = true;
                        this->highlight_item = i;
                        break;
                    }
                }
            }

            // Scroll if not moved
            if (!moved) {
                // Scroll faster if ZL/ZR held
                if (this->button[Utils::key_map[KEY_ZL]].is_pressed || this->button[Utils::key_map[KEY_ZR]].is_pressed) {
                    this->setScrollPos(this->scroll_pos + SCROLL_MULT*SCROLL_AMT*(dt/1000.0));
                } else {
                    this->setScrollPos(this->scroll_pos + SCROLL_AMT*(dt/1000.0));
                }
            }

        } else if (this->button[Utils::key_map[KEY_DUP]].is_pressed && this->button[Utils::key_map[KEY_DUP]].time_held > DELAY_OFFSET) {
            // Find min position
            unsigned int min_pos;
            if (this->scroll_pos == 0) {
                min_pos = 0;
            } else {
                min_pos = this->top_item + 1;
            }

            // Move selected if next item is fully visible
            bool moved = false;
            if (this->highlight_item > min_pos) {
                this->items[this->highlight_item]->setSelected(false);

                // Move to next selectable item
                for (int i = this->highlight_item - 1; i >= (int)min_pos; i--) {
                    if (i < 0) {
                        break;
                    }

                    if (this->items[i]->isSelectable()) {
                        moved = true;
                        this->highlight_item = i;
                        break;
                    }
                }
            }

            // Scroll if not moved
            if (!moved) {
                // Scroll faster if ZL/ZR held
                if (this->button[Utils::key_map[KEY_ZL]].is_pressed || this->button[Utils::key_map[KEY_ZR]].is_pressed) {
                    this->setScrollPos(this->scroll_pos - SCROLL_MULT*SCROLL_AMT*(dt/1000.0));
                } else {
                    this->setScrollPos(this->scroll_pos - SCROLL_AMT*(dt/1000.0));
                }
            }
        }

        // Select current item
        if (this->highlight_item >= 0 && this->highlight_item < this->items.size()) {
            if (this->is_active) {
                this->items[this->highlight_item]->setSelected(true);
                this->items[this->highlight_item]->update(dt);
            } else {
                this->items[this->highlight_item]->setSelected(false);
            }
        }

        // Reset chosen if necessary
        if (this->reset_chosen) {
            this->chosen = -1;
            this->reset_chosen = false;
        }
        if (this->chosen != -1) {
            this->reset_chosen = true;
        }
    }

    void List::draw() {
        // Draw list items
        size_t i = 0;
        if (this->top_item > 0) {
            i = this->top_item - 1;
        }

        for (; i < this->items.size(); i++) {
            // Break out of loop if element is not seen
            if (i > 0) {
                if (this->items[i-1]->getOffset() > this->scroll_pos + this->h) {
                    break;
                }
            }

            // Highlight appropriate entry
            this->items[i]->setY(this->y + this->offset + (this->items[i]->getOffset() - this->scroll_pos));
            if (i == this->highlight_item && !(*this->touch_active) && this->is_active) {
                SDLHelper::setColour(this->theme->getHighlight());
                SDLHelper::drawRect(this->items[i]->getX() - 5, this->items[i]->getY() - 5, this->items[i]->getW() + 10, this->items[i]->getH() + 10);
                SDLHelper::setColour(this->theme->getHighlightBG());
                SDLHelper::drawRect(this->items[i]->getX(), this->items[i]->getY(), this->items[i]->getW(), this->items[i]->getH());
            }

            this->items[i]->draw();

            // Draw touch overlay if touched
            if (this->touched_item == (int)i) {
                SDLHelper::setColour(this->theme->getTouchOverlay());
                SDLHelper::drawRect(this->items[i]->getX(), this->items[i]->getY(), this->items[i]->getW(), this->items[i]->getH());
            }
        }

        // Draw side bar
        if (this->max_scroll_pos != 0) {
            SDLHelper::setColour(this->theme->getMutedLine());
            int yPos = this->y + (((float)this->scroll_pos / this->max_scroll_pos) * (this->h - 70));
            SDLHelper::drawRect(this->x + this->w - 25, yPos, 5, 70);
        }
    }

    void List::touched(uint32_t type, float tx, float ty, float dx, float dy) {
        switch (type) {
            // Pressed
            case SDL_FINGERDOWN:
                this->is_touched = true;
                this->start_touch_y = ty;

                if (this->is_scrolling) {
                    // Stop scrolling when touched
                    this->scroll_velocity = 0;
                } else {
                    // Loop over visible items and determine if touched
                    for (unsigned int i = this->top_item; i < this->items.size(); i++) {
                        // Break out of loop if element is not seen
                        if (this->items[i]->getOffset() > this->scroll_pos + this->h) {
                            break;
                        }

                        if (tx >= this->items[i]->getX() && tx <= this->items[i]->getX() + this->items[i]->getW() && ty >= this->items[i]->getY() && ty <= this->items[i]->getY() + this->items[i]->getH()) {
                            if (this->items[i]->isSelectable()) {
                                this->touched_item = i;
                            }
                            break;
                        }
                    }
                }
                break;

            // Moved
            case SDL_FINGERMOTION:
                // If moved outside of radius swap from tap to swipe
                if (this->touched_item != -1) {
                    if (ty > this->start_touch_y + TOUCH_RADIUS || ty < this->start_touch_y - TOUCH_RADIUS) {
                        this->touched_item = -1;
                        break;
                    }

                // Scroll if not in "tap mode"
                } else {
                    this->setScrollPos(this->scroll_pos - dy);
                    this->scroll_velocity = dy;
                    if (this->scroll_velocity > SCROLL_V_MAX) {
                        this->scroll_velocity = SCROLL_V_MAX;
                    } else if (this->scroll_velocity < -SCROLL_V_MAX) {
                        this->scroll_velocity = -SCROLL_V_MAX;
                    }
                }
                break;

            // Released
            case SDL_FINGERUP:
                this->start_touch_y = -1;

                // If inside of same button call pressed
                if (this->touched_item != -1) {
                    if (tx >= this->items[this->touched_item]->getX() || tx <= this->items[this->touched_item]->getX() + this->items[this->touched_item]->getW() || ty >= this->items[this->touched_item]->getY() || ty <= this->items[this->touched_item]->getY() + this->items[this->touched_item]->getH()) {
                        this->items[this->touched_item]->pressed();
                        this->chosen = this->touched_item;
                        this->highlight_item = this->touched_item;
                    }
                } else {
                    this->is_scrolling = true;
                }

                this->touched_item = -1;
                this->is_touched = false;
                break;
        }
    }

    bool List::isTouched() {
        return this->is_touched;
    }

    void List::handleButton(uint8_t button, uint8_t state) {
        Navigable::handleButton(button, state);

        // Perform action on tap
        if (state == SDL_PRESSED) {
            this->is_scrolling = false;
            if (this->items.size() != 0) {
                // *** Fix highlight_item if outside of viewable area ***
                unsigned int max_pos = 0;
                if (this->scroll_pos >= this->max_scroll_pos) {
                    max_pos = this->items.size() - 1;
                } else {
                    for (size_t i = this->top_item; i < this->items.size(); i++) {
                        // Break if not being drawn
                        if ((this->items[i]->getOffset() + this->items[i]->getH()) > this->scroll_pos + this->h - (int)this->offset) {
                            max_pos = i-1;
                            break;
                        }
                    }
                }

                unsigned int min_pos;
                if (this->scroll_pos == 0) {
                    min_pos = 0;
                } else {
                    min_pos = this->top_item + 1;
                }

                if (this->highlight_item < min_pos) {
                    this->items[this->highlight_item]->setSelected(false);
                    // Move to next selectable item
                    for (unsigned int i = min_pos; i <= max_pos; i++) {
                        if (this->items[i]->isSelectable()) {
                            this->highlight_item = i;
                            this->items[this->highlight_item]->setSelected(true);
                            break;
                        }
                    }
                    return;
                } else if (this->highlight_item > max_pos) {
                    this->items[this->highlight_item]->setSelected(false);
                    // Move to next selectable item
                    for (unsigned int i = max_pos; i >= min_pos; i--) {
                        if (this->items[i]->isSelectable()) {
                            this->highlight_item = i;
                            this->items[this->highlight_item]->setSelected(true);
                            break;
                        }
                    }
                    return;
                }
                // *** End section ***

                if (button == Utils::key_map[KEY_DDOWN]) {
                    // If within viewable area move to next selectable entry
                    bool moved = false;
                    if (this->highlight_item < max_pos) {
                        this->items[this->highlight_item]->setSelected(false);

                        // Move to next selectable item
                        for (unsigned int i = this->highlight_item + 1; i <= max_pos; i++) {
                            if (this->items[i]->isSelectable()) {
                                moved = true;
                                this->highlight_item = i;
                                break;
                            }
                        }
                    }

                    // Jump (or scroll) to next selectable item if on the edge
                    if (!moved) {
                        // Attempt to jump to next selectable item
                        moved = false;
                        for (unsigned int i = this->highlight_item + 1; i < this->items.size(); i++) {
                            if (this->items[i]->isSelectable()) {
                                unsigned int prev_item = this->highlight_item;
                                this->items[this->highlight_item]->setSelected(false);
                                this->highlight_item = i;
                                this->items[this->highlight_item]->setSelected(true);
                                this->setScrollPos(this->scroll_pos + (this->items[this->highlight_item]->getOffset() - this->items[prev_item]->getOffset()));
                                moved = true;
                                break;
                            }
                        }

                        // If no jump was made (ie. no more selectable in the list), just scroll
                        if (!moved) {
                            this->button[button].time_held = DELAY_OFFSET;
                        }
                    }

                } else if (button == Utils::key_map[KEY_DUP]) {
                    bool moved = false;
                    if (this->highlight_item > min_pos) {
                        this->items[this->highlight_item]->setSelected(false);

                        // Move to next selectable item
                        for (int i = this->highlight_item - 1; i >= min_pos; i--) {
                            if (i < 0) {
                                break;
                            }

                            if (this->items[i]->isSelectable()) {
                                moved = true;
                                this->highlight_item = i;
                                break;
                            }
                        }
                    }

                    // Jump (or scroll) to previous selectable item if on the edge
                    if (!moved) {
                        // Attempt to jump to previous selectable item
                        moved = false;
                        for (int i = this->highlight_item - 1; i >= 0; i--) {
                            if (this->items[i]->isSelectable()) {
                                unsigned int prev_item = this->highlight_item;
                                this->items[this->highlight_item]->setSelected(false);
                                this->highlight_item = i;
                                this->items[this->highlight_item]->setSelected(true);
                                this->setScrollPos(this->scroll_pos - (this->items[prev_item]->getOffset() - this->items[this->highlight_item]->getOffset()));
                                moved = true;
                                break;
                            }
                        }

                        // If no jump was made (ie. no more selectable in the list), just scroll
                        if (!moved) {
                            this->button[button].time_held = DELAY_OFFSET;
                        }
                    }

                } else if (button == Utils::key_map[KEY_A]) {
                    this->touched_item = this->highlight_item;
                }
            }
        } else if (state == SDL_RELEASED) {
            if (button == Utils::key_map[KEY_A] && this->items.size() != 0) {
                this->items[this->highlight_item]->pressed();
                this->chosen = this->highlight_item;
                this->touched_item = -1;
            }
        }
    }

    void List::setScrollPos(int pos) {
        // Prevent going outside of range
        if (pos < 0) {
            this->scroll_pos = 0;
        } else if (pos > this->max_scroll_pos) {
            this->scroll_pos = this->max_scroll_pos;
        } else {
            this->scroll_pos = pos;
        }

        // Set top_item
        for (size_t i = 0; i < this->items.size(); i++) {
            if (this->items[i]->getOffset() >= this->scroll_pos) {
                this->top_item = i - 1;
                if (i == 0) {
                    this->top_item = 0;
                }
                break;
            }
        }
    }

    int List::getChosen() {
        return this->chosen;
    }

    void List::setH(int h) {
        Drawable::setH(h);

        // Update maximum scroll pos
        this->max_scroll_pos = 0;
        for (size_t i = 0; i < this->items.size(); i++) {
            this->items[i]->setOffset(this->max_scroll_pos);
            this->max_scroll_pos += this->items[i]->getH();
        }
        this->max_scroll_pos += this->offset;

        // If the summed height of all items is less than the list height don't scroll
        if (this->max_scroll_pos <= this->h) {
            this->max_scroll_pos = 0;
            return;
        }

        this->max_scroll_pos -= this->h;
    }

    List::~List() {
        // Delete ListItem objects
        while (this->items.size() > 0) {
            delete this->items[0];
            this->items.erase(this->items.begin());
        }
    }
};