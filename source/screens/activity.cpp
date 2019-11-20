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
        this->list = new UI::List(400, 100, 850, 550);
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
        this->menu = new UI::SideMenu();
        this->menu->addItem(new UI::SideItem("Play Activity"));
        this->menu->addItem(new UI::SideItem("Settings"));

        this->user = u;
        this->controls->add(KEY_PLUS, "Exit", 0);
        this->controls->add(KEY_MINUS, "Sort", 1);
        this->controls->add(KEY_X, "Theme (Dark)", 2);
        this->controls->add(KEY_Y, "Theme (Light)", 3);
        this->controls->disable(KEY_Y);

        this->list_scroll = false;
    }

    void Activity::event() {
        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    if (events.jbutton.which == 0) {
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
                        } else if (events.jbutton.button == Utils::key_map[KEY_DRIGHT]) {
                            this->active_element = (int)ActiveElement::List;

                        // These will be removed at some point
                        } else if (events.jbutton.button == Utils::key_map[KEY_X]) {
                            UI::theme = UI::theme_dark;
                            this->controls->disable(KEY_X);
                            this->controls->enable(KEY_Y);
                        } else if (events.jbutton.button == Utils::key_map[KEY_Y]) {
                            UI::theme = UI::theme_light;
                            this->controls->disable(KEY_Y);
                            this->controls->enable(KEY_X);

                        // All other buttons get handled by active element
                        } else {
                            switch (this->active_element) {
                                case (int)ActiveElement::SideMenu:
                                    this->menu->button(events.jbutton.button, events.jbutton.state);
                                    break;

                                case (int)ActiveElement::List:

                                    break;
                            }
                        }
                    }
                    break;

                // Touch (pressed)
                case SDL_FINGERDOWN: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Pressed within list: activate scroll
                    if (x >= this->list->getX() && x <= this->list->getX() + this->list->getW() && y >= this->list->getY() && y <= this->list->getY() + this->list->getH()) {
                        this->list->touched(events.type, x, y);
                    }

                    // Pass event to controls object if below bottom line
                    if (y > 647) {
                        this->controls->touched(events.type, x, y);
                    }
                    break;
                }

                // Touch (moved)
                case SDL_FINGERMOTION: {
                    float x = WIDTH * events.tfinger.x;
                    float y = HEIGHT * events.tfinger.y;

                    // Scrolling overrides any other actions
                    if (x >= this->list->getX() && x <= this->list->getX() + this->list->getW() && y >= this->list->getY() && y <= this->list->getY() + this->list->getH()) {
                        this->list->touched(events.type, x, y, (WIDTH * events.tfinger.dx), (HEIGHT * events.tfinger.dy));
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

                    if (x >= this->list->getX() && x <= this->list->getX() + this->list->getW() && y >= this->list->getY() && y <= this->list->getY() + this->list->getH()) {
                        this->list->touched(events.type, x, y, (WIDTH * events.tfinger.dx), (HEIGHT * events.tfinger.dy));
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
        this->list->update(dt);
    }

    void Activity::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(UI::theme.background);
        SDLHelper::clearScreen();

        // Draw menu
        this->menu->draw(30, 130, 400, 500);

        // Different shade behind list
        SDLHelper::setColour(UI::theme.alt_background);
        SDLHelper::drawRect(400, 88, 850, 560);

        // Draw list of items
        this->list->draw();

        // Draw over list to hide scrolling
        SDLHelper::setColour(UI::theme.background);
        SDLHelper::drawRect(430, 0, 780, 87);
        SDLHelper::drawRect(430, 648, 1220, 72);

        // Draw top and bottom lines
        SDLHelper::setColour(UI::theme.foreground);
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Draw player icon
        SDLHelper::drawTexture(this->user->getImage(), 65, 14, 60, 60);

        // Print heading
        std::string str = this->user->getUsername() + "'s Activity";
        SDLHelper::drawText(str.c_str(), UI::theme.text, 150, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);

        // Print total hours
        int tw, th;
        SDLHelper::getDimensions(this->total_hours, &tw, &th);
        SDLHelper::drawTexture(this->total_hours, UI::theme.muted_text, 1215 - tw, 44 - th/2);

        // Draw controls
        SDLHelper::setColour(SDL_Color{255, 255, 255, 255});
        this->controls->draw();

        std::string stra = "ACTIVE: " + std::to_string(this->active_element);
        SDLHelper::drawText(stra.c_str(), UI::theme.text, 0, 500, 30);
    }

    Activity::~Activity() {
        delete this->list;
        delete this->menu;
    }
}