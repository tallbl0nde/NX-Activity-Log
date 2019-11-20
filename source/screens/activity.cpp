#include "activity.hpp"
#include "ui/list.hpp"
#include "ui/listitem.hpp"
#include "SDLHelper.hpp"
#include <switch.h>
#include "theme.hpp"
#include "utils.hpp"

namespace Screen {
    Activity::Activity(bool * b, User * u, std::vector<Title *> tls) : Screen::Screen(b) {
        u32 total_mins = 0;
        this->list = new UI::List(&this->touch_active, 400, 100, 850, 550);
        for (size_t i = 0; i < tls.size(); i++) {
            this->list->addItem(new UI::ListItem(tls[i]));
            total_mins += tls[i]->getPlaytime();
        }

        // Sort list by most played
        this->list->sort(HoursAsc);

        // Create total hours texture
        std::string str = "Total Playtime: ";
        str += Utils::formatPlaytime(total_mins);
        this->total_hours = SDLHelper::renderText(str.c_str(), BODY_FONT_SIZE);

        // Create side menu
        this->menu = new UI::SideMenu(&this->touch_active, 30, 130, 400, 500);
        this->menu->addItem(new UI::SideItem("Play Activity"));
        this->menu->addItem(new UI::SideItem("Settings"));

        this->user = u;
        this->controls->reset();
        this->controls->add(KEY_PLUS, "Exit", 0);
        this->controls->add(KEY_MINUS, "Sort", 1);

        // Set active element
        this->active_element = (int)ActiveElement::SideMenu;
        this->menu->setActive(true);
        this->list->setActive(false);
    }

    void Activity::event(){
        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    // Break on first press (ie. only active highlighting)
                    if (this->touch_active && events.jbutton.which != 99) {
                        this->touch_active = false;
                        if (events.jbutton.button >= Utils::key_map[KEY_DLEFT] && events.jbutton.button <= Utils::key_map[KEY_DDOWN]) {
                            break;
                        }
                    }

                    if (events.jbutton.which == 0 || events.jbutton.which == 99) {
                        // Plus exits app
                        if (events.jbutton.button == Utils::key_map[KEY_PLUS]) {
                            *(this->loop) = false;

                        // Minus changes sorting method
                        } else if (events.jbutton.button == Utils::key_map[KEY_MINUS]) {
                            switch (this->list->getSorting()) {
                                case AlphaAsc:
                                    this->list->sort(HoursAsc);
                                    break;
                                case HoursAsc:
                                    this->list->sort(HoursDec);
                                    break;
                                case HoursDec:
                                    this->list->sort(LaunchAsc);
                                    break;
                                case LaunchAsc:
                                    this->list->sort(LaunchDec);
                                    break;
                                case LaunchDec:
                                    this->list->sort(FirstPlayed);
                                    break;
                                case FirstPlayed:
                                    this->list->sort(LastPlayed);
                                    break;
                                case LastPlayed:
                                    this->list->sort(AlphaAsc);
                                    break;
                            }

                            this->list->setPos(0);

                        // Left and right will swap active element
                        } else if (events.jbutton.button == Utils::key_map[KEY_DLEFT]) {
                            this->active_element = (int)ActiveElement::SideMenu;
                            this->menu->setActive(true);
                            this->list->setActive(false);
                        } else if (events.jbutton.button == Utils::key_map[KEY_DRIGHT]) {
                            this->active_element = (int)ActiveElement::List;
                            this->menu->setActive(false);
                            this->list->setActive(true);

                        // All other buttons get handled by active element
                        } else {
                            switch (this->active_element) {
                                case (int)ActiveElement::SideMenu:
                                    this->menu->button(events.jbutton.button, events.jbutton.state);
                                    break;

                                case (int)ActiveElement::List:
                                    this->list->button(events.jbutton.button, events.jbutton.state);
                                    break;
                            }
                        }
                    }
                    break;

                case SDL_JOYBUTTONUP:
                    if (events.jbutton.which == 0) {
                        // All other buttons get handled by active element
                        switch (this->active_element) {
                            case (int)ActiveElement::SideMenu:
                                this->menu->button(events.jbutton.button, events.jbutton.state);
                                break;

                            case (int)ActiveElement::List:
                                this->list->button(events.jbutton.button, events.jbutton.state);
                                break;
                        }
                    }
                    break;

                // Touch (pressed)
                case SDL_FINGERDOWN: {
                    this->touch_active = true;
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pressed within list
                    if (x >= this->list->getX() && x <= this->list->getX() + this->list->getW() && y >= this->list->getY() && y <= this->list->getY() + this->list->getH()) {
                        this->list->touched(events.type, x, y);
                        this->active_element = (int)ActiveElement::List;
                        this->menu->setActive(false);
                        this->list->setActive(true);

                    // Pressed within menu
                    } else if (x >= this->menu->getX() && x <= this->menu->getX() + this->menu->getW() && y >= this->menu->getY() && y <= this->menu->getY() + this->menu->getH()) {
                        this->menu->touched(events.type, x, y);
                        this->active_element = (int)ActiveElement::SideMenu;
                        this->menu->setActive(true);
                        this->list->setActive(false);

                    // Pass event to controls object if below bottom line
                    } else if (y > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }

                // Touch (moved)
                case SDL_FINGERMOTION: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;
                    float dx = WIDTH * events.tfinger.dx;
                    float dy = HEIGHT * events.tfinger.dy;

                    // List scrolling overrides any other actions
                    if (this->list->isTouched()) {
                        this->list->touched(events.type, x, y, dx, dy);

                    // Moved after being in menu
                    } else if ((x-dx) >= this->menu->getX() && (x-dx) <= this->menu->getX() + this->menu->getW() && (y-dy) >= this->menu->getY() && (y-dy) <= this->menu->getY() + this->menu->getH()) {
                        this->menu->touched(events.type, x, y);

                    } else {
                        // Pass event to controls object if was below or originally below line
                        if (y > 647 || (HEIGHT * (events.tfinger.y - events.tfinger.dy)) > 647) {
                            this->controls->touched(events.type, x, y);
                        }
                    }
                    break;
                }

                // Touch (released)
                case SDL_FINGERUP: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    if (this->list->isTouched()) {
                        this->list->touched(events.type, x, y, (WIDTH * events.tfinger.dx), (HEIGHT * events.tfinger.dy));

                    // Released within menu
                    } else if (x >= this->menu->getX() && x <= this->menu->getX() + this->menu->getW() && y >= this->menu->getY() && y <= this->menu->getY() + this->menu->getH()) {
                        this->menu->touched(events.type, x, y);

                    } else {
                        // Pass event to controls object if below bottom line
                        if (y > 647) {
                            this->controls->touched(events.type, x, y);
                        }
                    }
                    break;
                }
            }
        }
    }

    void Activity::update(uint32_t dt) {
        Screen::update(dt);
        this->menu->update(dt);
        this->list->update(dt);
    }

    void Activity::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::clearScreen();

        // Draw menu
        this->menu->draw();

        // Different shade behind list
        SDLHelper::setColour(this->theme->getAltBG());
        SDLHelper::drawRect(400, 88, 850, 560);

        // Draw list of items
        this->list->draw();

        // Draw over list to hide scrolling
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::drawRect(430, 0, 780, 87);
        SDLHelper::drawRect(430, 648, 1220, 72);

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->getFG());
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Draw player icon
        SDLHelper::drawTexture(this->user->getImage(), 65, 14, 60, 60);

        // Print heading
        std::string str = this->user->getUsername() + "'s Activity";
        SDLHelper::drawText(str.c_str(), this->theme->getText(), 150, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);

        // Print total hours
        int tw, th;
        SDLHelper::getDimensions(this->total_hours, &tw, &th);
        SDLHelper::drawTexture(this->total_hours, this->theme->getMutedText(), 1215 - tw, 44 - th/2);

        // Draw controls
        this->controls->draw();
    }

    Activity::~Activity() {
        delete this->list;
        delete this->menu;
    }
}