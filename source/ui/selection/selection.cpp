#include "listitem_textentry.hpp"
#include "selection.hpp"
#include "utils.hpp"

// Dimensions
#define LIST_WIDTH 725
#define MAX_HEIGHT 480
#define LIST_GAP 40

// Font size
#define TITLE_FONT_SIZE 26

namespace UI {
    Selection::Selection(bool * b, std::string s, std::vector<std::string> v, int t) : Drawable(0, 0, WIDTH, HEIGHT) {
        this->is_active = true;

        // Set up controls
        this->controls = new Controls(65, 670, 1150, 50);
        this->controls->add(KEY_A, "OK", 0);
        this->controls->add(KEY_B, "Back", 1);

        // Prepare list and use list items to determine max height
        this->list = new List(b, (WIDTH - LIST_WIDTH)/2, 0, LIST_WIDTH, HEIGHT);
        this->list->setActive(true);
        unsigned int total_height = 0;
        for (size_t i = 0; i < v.size(); i++) {
            bool tick = false;
            if (i == t) {
                tick = true;
            }
            UI::ListItem::TextEntry * tmp = new UI::ListItem::TextEntry(v[i], tick);
            this->list->addItem(tmp);
            total_height += tmp->getH();
        }

        // If all items can't fit use max height
        if (total_height >= MAX_HEIGHT - 160 - 2*LIST_GAP) {
            this->y = HEIGHT - MAX_HEIGHT;
            this->h = MAX_HEIGHT;

        // Adjust height to fit
        } else {
            this->y = HEIGHT - total_height - 160 - 2*LIST_GAP;
            this->h = total_height + 160 + 2*LIST_GAP;
        }
        this->list->setY(this->y + 80 + LIST_GAP);
        this->list->setH(this->h - 160 - 2*LIST_GAP);

        this->title = SDLHelper::renderText(s.c_str(), TITLE_FONT_SIZE);
    }

    void Selection::event(SDL_Event e) {
        switch (e.type) {
            case SDL_JOYBUTTONDOWN:
                this->list->handleButton(e.jbutton.button, e.jbutton.state);
                break;

            case SDL_JOYBUTTONUP:
                if (e.jbutton.button == Utils::key_map[KEY_B]) {
                    this->is_active = false;
                } else {
                    this->list->handleButton(e.jbutton.button, e.jbutton.state);
                }
                break;

            // Touch (pressed)
            case SDL_FINGERDOWN: {
                float x = e.tfinger.x * WIDTH;
                float y = e.tfinger.y * HEIGHT;

                // Pressed within list
                if (x >= this->list->getX() && x <= this->list->getX() + this->list->getW() && y >= this->list->getY() && y <= this->list->getY() + this->list->getH()) {
                    this->list->touched(e.type, x, y);

                // Pass event to controls object if below bottom line
                } else if (y > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }

            // Touch (moved)
            case SDL_FINGERMOTION: {
                float x = e.tfinger.x * WIDTH;
                float y = e.tfinger.y * HEIGHT;
                float dx = e.tfinger.dx * WIDTH;
                float dy = e.tfinger.dy * HEIGHT;

                // List scrolling overrides any other actions
                if (this->list->isTouched()) {
                    this->list->touched(e.type, x, y, dx, dy);

                } else {
                    // Pass event to controls object if was below or originally below line
                    if (y > 647 || (HEIGHT * (y - dy)) > 647) {
                        this->controls->touched(e.type, x, y);
                    }
                }
                break;
            }

            // Touch (released)
            case SDL_FINGERUP: {
                float x = e.tfinger.x * WIDTH;
                float y = e.tfinger.y * HEIGHT;
                float dx = e.tfinger.dx * WIDTH;
                float dy = e.tfinger.dy * HEIGHT;

                if (this->list->isTouched()) {
                    this->list->touched(e.type, x, y, (WIDTH * dx), (HEIGHT * dy));

                } else {
                    // Pass event to controls object if below bottom line
                    if (y > 647) {
                        this->controls->touched(e.type, x, y);
                    }
                }
                break;
            }
        }
    }

    void Selection::update(uint32_t dt) {
        this->list->update(dt);

        // Close when an item is chosen
        if (this->list->getChosen() != -1) {
            this->is_active = false;
        }
    }

    void Selection::draw() {
        // Draw background
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::drawRect(this->x, this->y, this->w, this->h);

        // Draw list
        this->list->draw();

        // Draw over list at top/bottom
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::drawRect(this->list->getX(), this->y, this->list->getW(), 72);
        SDLHelper::drawRect(this->list->getX(), this->y + this->h - 72, this->list->getW(), 72);

        // Draw lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(this->x + 30, this->y + 72, 1220, 1);
        SDLHelper::drawRect(this->x + 30, this->y + this->h - 72, 1220, 1);

        // Draw title + controls
        int tw, th;
        SDLHelper::getDimensions(this->title, &tw, &th);
        SDLHelper::drawTexture(this->title, this->theme->getText(), this->x + 72, this->y + 40 - th/2);

        this->controls->draw();
    }

    bool Selection::closed() {
        return !this->is_active;
    }

    int Selection::getChosen() {
        return this->list->getChosen();
    }

    Selection::~Selection() {
        delete this->controls;
        delete this->list;
        SDLHelper::destroyTexture(this->title);
    }
};