#include "config.hpp"
#include "SDLHelper.hpp"
#include "list.hpp"
#include "listitem_option.hpp"
#include "listitem_tooltip.hpp"
#include "listitem_separator.hpp"
#include "settings.hpp"
#include "utils.hpp"

// FUNCTIONS PASSED TO ITEMS //
// Passed to item to toggle deleted games
static std::string func_deleted(bool d) {
    std::string ret;

    Config * conf = Config::getConfig();
    bool b = conf->getHiddenDeleted();
    if (d) {
        b = !b;
        conf->setHiddenDeleted(b);
    }

    if (b) {
        ret = "Yes";
    } else {
        ret = "No";
    }

    return ret;
}

// Passed to item to change theme
static std::string func_theme(bool d) {
    std::string ret;

    Config * conf = Config::getConfig();
    ThemeType t = conf->getGeneralTheme();
    if (d) {
        if (t == T_Light) {
            t = T_Dark;
        } else if (t == T_Dark) {
            t = T_Auto;
        } else if (t == T_Auto) {
            t = T_Light;
        }
        conf->setGeneralTheme(t);
    }

    if (t == T_Light) {
        ret = "Light";
    } else if (t == T_Dark) {
        ret = "Dark";
    } else if (t == T_Auto) {
        ret = "Auto";
    }

    return ret;
}

// Passed to item to change default sort
static std::string func_sort(bool d) {
    std::string ret;

    Config * conf = Config::getConfig();
    SortType s = conf->getGeneralSort();
    if (d) {
        if (s == AlphaAsc) {
            s = HoursAsc;
        } else if (s == HoursAsc) {
            s = HoursDec;
        } else if (s == HoursDec) {
            s = LaunchAsc;
        } else if (s == LaunchAsc) {
            s = LaunchDec;
        } else if (s == LaunchDec) {
            s = FirstPlayed;
        } else if (s == FirstPlayed) {
            s = LastPlayed;
        } else if (s == LastPlayed) {
            s = AlphaAsc;
        }
        conf->setGeneralSort(s);
    }

    if (s == AlphaAsc) {
        ret = "Alphabetically";
    } else if (s == HoursAsc) {
        ret = "Most Played";
    } else if (s == HoursDec) {
        ret = "Least Played";
    } else if (s == LaunchAsc) {
        ret = "Most Launched";
    } else if (s == LaunchDec) {
        ret = "Least Launched";
    } else if (s == FirstPlayed) {
        ret = "First Played";
    } else if (s == LastPlayed) {
        ret = "Recently Played";
    }

    return ret;
}

// Passed to item to toggle deleted games
static std::string func_unplayed(bool d) {
    std::string ret;

    Config * conf = Config::getConfig();
    bool b = conf->getHiddenUnplayed();
    if (d) {
        b = !b;
        conf->setHiddenUnplayed(b);
    }

    if (b) {
        ret = "Yes";
    } else {
        ret = "No";
    }

    return ret;
}

namespace Screen {
    Settings::Settings(bool * b, User * u) : Screen::Screen(b) {
        // Create side menu
        this->menu = new UI::SideMenu(&this->touch_active, 30, 130, 400, 500);
        this->menu->addItem(new UI::SideItem("Play Activity"));
        this->menu->addItem(new UI::SideItem("Settings"));
        this->menu->setSelected(1);

        // Create list
        this->list = new UI::List(&this->touch_active, 400, 110, 850, 520);
        this->list->addItem(new UI::ListItem::ToolTip("Note: All options currently require the application to be restarted to take effect."));
        this->list->addItem(new UI::ListItem::Separator(10));
        this->list->addItem(new UI::ListItem::Option("Default Sorting", &func_sort));
        this->list->addItem(new UI::ListItem::ToolTip("Sets the sorting used upon application launch."));
        this->list->addItem(new UI::ListItem::Option("Theme", &func_theme));
        this->list->addItem(new UI::ListItem::ToolTip("Sets the theme for the application. Auto will choose the dark/light theme based on your switch settings."));
        this->list->addItem(new UI::ListItem::Separator());
        this->list->addItem(new UI::ListItem::Option("Hide Deleted Games", &func_deleted));
        this->list->addItem(new UI::ListItem::ToolTip("Excludes and hides deleted games from your play activity."));
        this->list->addItem(new UI::ListItem::Option("Hide Unplayed Games", &func_unplayed));
        this->list->addItem(new UI::ListItem::ToolTip("Excludes and hides games that haven't been played from your play activity."));

        this->user = u;
        this->controls->reset();
        this->controls->add(KEY_A, "OK", 0);
        this->controls->add(KEY_PLUS, "Exit", 1);

        // Set active element
        this->active_element = (int)ActiveElement::SideMenu;
        this->menu->setActive(true);
        this->list->setActive(false);
    }

    void Settings::event() {
        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    // Break on first press (ie. only active highlighting)
                    if (this->touch_active && events.jbutton.which != 99) {
                        if (!(events.jbutton.button >= Utils::key_map[KEY_LSTICK_LEFT] && events.jbutton.button <= Utils::key_map[KEY_RSTICK_DOWN])) {
                            this->touch_active = false;
                        }
                        if (events.jbutton.button >= Utils::key_map[KEY_DLEFT] && events.jbutton.button <= Utils::key_map[KEY_DDOWN] && this->active_element != (int)ActiveElement::List) {
                            break;
                        }
                    }

                    if (events.jbutton.which == 0 || events.jbutton.which == 99) {
                        // Joysticks push appropriate button event
                        if (events.jbutton.button >= Utils::key_map[KEY_LSTICK_LEFT] && events.jbutton.button <= Utils::key_map[KEY_RSTICK_DOWN]) {
                            SDL_Event event;
                            event.type = SDL_JOYBUTTONDOWN;
                            event.jbutton.which = 0;
                            event.jbutton.state = SDL_PRESSED;
                            if (events.jbutton.button == Utils::key_map[KEY_LSTICK_LEFT] || events.jbutton.button == Utils::key_map[KEY_RSTICK_LEFT]) {
                                event.jbutton.button = Utils::key_map[KEY_DLEFT];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_RIGHT] || events.jbutton.button == Utils::key_map[KEY_RSTICK_RIGHT]) {
                                event.jbutton.button = Utils::key_map[KEY_DRIGHT];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_UP] || events.jbutton.button == Utils::key_map[KEY_RSTICK_UP]) {
                                event.jbutton.button = Utils::key_map[KEY_DUP];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_DOWN] || events.jbutton.button == Utils::key_map[KEY_RSTICK_DOWN]) {
                                event.jbutton.button = Utils::key_map[KEY_DDOWN];
                            }
                            SDL_PushEvent(&event);

                        // Plus exits app
                        } else if (events.jbutton.button == Utils::key_map[KEY_PLUS]) {
                            *(this->loop) = false;

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
                                    this->menu->handleButton(events.jbutton.button, events.jbutton.state);
                                    break;

                                case (int)ActiveElement::List:
                                    this->list->handleButton(events.jbutton.button, events.jbutton.state);
                                    break;
                            }
                        }
                    }
                    break;

                case SDL_JOYBUTTONUP:
                    if (events.jbutton.which == 0) {
                        // Joysticks push appropriate button event
                        if (events.jbutton.button >= Utils::key_map[KEY_LSTICK_LEFT] && events.jbutton.button <= Utils::key_map[KEY_RSTICK_DOWN]) {
                            SDL_Event event;
                            event.type = SDL_JOYBUTTONUP;
                            event.jbutton.which = 0;
                            event.jbutton.state = SDL_RELEASED;
                            if (events.jbutton.button == Utils::key_map[KEY_LSTICK_LEFT] || events.jbutton.button == Utils::key_map[KEY_RSTICK_LEFT]) {
                                event.jbutton.button = Utils::key_map[KEY_DLEFT];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_RIGHT] || events.jbutton.button == Utils::key_map[KEY_RSTICK_RIGHT]) {
                                event.jbutton.button = Utils::key_map[KEY_DRIGHT];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_UP] || events.jbutton.button == Utils::key_map[KEY_RSTICK_UP]) {
                                event.jbutton.button = Utils::key_map[KEY_DUP];
                            } else if (events.jbutton.button == Utils::key_map[KEY_LSTICK_DOWN] || events.jbutton.button == Utils::key_map[KEY_RSTICK_DOWN]) {
                                event.jbutton.button = Utils::key_map[KEY_DDOWN];
                            }
                            SDL_PushEvent(&event);

                        // All other buttons get handled by active element
                        } else {
                            switch (this->active_element) {
                                case (int)ActiveElement::SideMenu:
                                    this->menu->handleButton(events.jbutton.button, events.jbutton.state);
                                    break;

                                case (int)ActiveElement::List:
                                    this->list->handleButton(events.jbutton.button, events.jbutton.state);
                                    break;
                            }
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

                    // // List scrolling overrides any other actions
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

    void Settings::update(uint32_t dt) {
        Screen::update(dt);
        this->menu->update(dt);
        this->list->update(dt);

        if (this->touch_active) {
            this->controls->disable(KEY_A);
        } else {
            this->controls->enable(KEY_A);
        }
    }

    void Settings::draw() {
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

        // Draw controls
        this->controls->draw();
    }

    ScreenID Settings::change() {
        if (this->menu->getSelected() == 0) {
            return S_Activity;
        }
        return S_Nothing;
    }

    Settings::~Settings() {
        delete this->list;
        delete this->menu;
    }
};