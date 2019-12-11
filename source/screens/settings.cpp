#include "config.hpp"
#include <filesystem>
#include "list.hpp"
#include "listitem_option.hpp"
#include "listitem_tooltip.hpp"
#include "listitem_separator.hpp"
#include "screenmanager.hpp"

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
    Config * conf = Config::getConfig();

    // If tapped...
    if (d) {
        std::vector<std::string> v;
        v.push_back("Auto");
        v.push_back("Dark");
        v.push_back("Light");
        ScreenManager::getInstance()->createSelection("Theme", v, [conf](int c){
            switch (c) {
                case -1:
                    // Do nothing if nothing selected
                    break;
                case 0:
                    conf->setGeneralTheme(T_Auto);
                    break;
                case 1:
                    conf->setGeneralTheme(T_Dark);
                    break;
                case 2:
                    conf->setGeneralTheme(T_Light);
                    break;
            }
        });
    }

    // Return theme stored in config
    std::string ret;
    ThemeType t = conf->getGeneralTheme();
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
    Config * conf = Config::getConfig();

    // If tapped...
    if (d) {
        std::vector<std::string> v;
        v.push_back("By Name");
        v.push_back("By Most Played");
        v.push_back("By Least Played");
        v.push_back("By Most Launched");
        v.push_back("By Least Launched");
        v.push_back("By First Playtime");
        v.push_back("By Recently Played");
        ScreenManager::getInstance()->createSelection("Default Sort Method", v, [conf](int c){
            switch (c) {
                case -1:
                    // Do nothing if nothing selected
                    break;
                case 0:
                    conf->setGeneralSort(AlphaAsc);
                    break;
                case 1:
                    conf->setGeneralSort(HoursAsc);
                    break;
                case 2:
                    conf->setGeneralSort(HoursDec);
                    break;
                case 3:
                    conf->setGeneralSort(LaunchAsc);
                    break;
                case 4:
                    conf->setGeneralSort(LaunchDec);
                    break;
                case 5:
                    conf->setGeneralSort(FirstPlayed);
                    break;
                case 6:
                    conf->setGeneralSort(LastPlayed);
                    break;
            }
        });
    }

    // Return sort method stored in config
    std::string ret;
    SortType s = conf->getGeneralSort();
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

// Passed to item to toggle forwarder
static std::string func_forwarder(bool d) {
    std::string ret;
    // Check if forwarder exists
    bool atms = false;
    bool rei = false;
    bool sx = false;
    bool hasAtms = false;
    bool hasRei = false;
    bool hasSx = false;
    if (std::filesystem::exists("/atmosphere/contents")) {
        hasAtms = true;
        if (std::filesystem::exists("/atmosphere/contents/0100000000001013/exefs.nsp")) {
            atms = true;
        }
    }
    if (std::filesystem::exists("/ReiNX/titles")) {
        hasRei = true;
        if (std::filesystem::exists("/ReiNX/titles/0100000000001013/exefs.nsp")) {
            rei = true;
        }
    }
    if (std::filesystem::exists("/sxos/titles")) {
        hasRei = true;
        if (std::filesystem::exists("/sxos/titles/0100000000001013/exefs.nsp")) {
            rei = true;
        }
    }

    if (d) {
        // Delete files
        if (atms || rei || sx) {
            if (atms) {
                std::filesystem::remove("/atmosphere/contents/0100000000001013/exefs.nsp");
                atms = false;
            }
            if (rei) {
                std::filesystem::remove("/ReiNX/titles/0100000000001013/exefs.nsp");
                rei = false;
            }
            if (sx) {
                std::filesystem::remove("/sxos/titles/0100000000001013/exefs.nsp");
                sx = false;
            }

        // Copy files
        } else {
            if (hasAtms) {
                std::filesystem::create_directory("/atmosphere/contents/0100000000001013");
                Utils::copyFile("romfs:/exefs.nsp", "/atmosphere/contents/0100000000001013/exefs.nsp");
                atms = true;
            }

            if (hasRei) {
                std::filesystem::create_directory("/ReiNX/titles/0100000000001013");
                Utils::copyFile("romfs:/exefs.nsp", "/ReiNX/titles/0100000000001013/exefs.nsp");
                rei = true;
            }

            if (hasSx) {
                std::filesystem::create_directory("/sxos/titles/0100000000001013");
                Utils::copyFile("romfs:/exefs.nsp", "/sxos/titles/0100000000001013/exefs.nsp");
                sx = true;
            }
        }
    }

    if (atms || rei || sx) {
        return "Yes";
    } else {
        return "No";
    }
}

namespace Screen {
    Settings::Settings(User * u) : Screen::Screen() {
        // Create list
        this->list = new UI::List(&ScreenManager::getInstance()->touch_active, 400, 110, 850, 520);
        this->list->addItem(new UI::ListItem::Separator(15));
        this->list->addItem(new UI::ListItem::Option("Default Sort Method", &func_sort));
        this->list->addItem(new UI::ListItem::ToolTip("Sets the sort method used upon application launch."));
        this->list->addItem(new UI::ListItem::Option("Theme", &func_theme));
        this->list->addItem(new UI::ListItem::ToolTip("Sets the theme for the application. Auto will choose the dark/light theme based on your switch settings. Note: This requires the app to be restarted to take effect."));
        this->list->addItem(new UI::ListItem::Separator());
        this->list->addItem(new UI::ListItem::Option("Hide Deleted Games", &func_deleted));
        this->list->addItem(new UI::ListItem::ToolTip("Excludes and hides deleted games from your play activity."));
        this->list->addItem(new UI::ListItem::Separator());
        this->list->addItem(new UI::ListItem::Option("Override User Page", &func_forwarder));
        this->list->addItem(new UI::ListItem::ToolTip("Uses LayeredFS to override the user page with this app."));
        this->list->addItem(new UI::ListItem::Separator(30));
        this->list->addItem(new UI::ListItem::ToolTip("NX Activity Log v1.1.0a\nThank you for using my app! You can support me on Ko-fi:\nhttps://ko-fi.com/tallbl0nde"));

        this->user = u;
        this->controls->add(KEY_A, "OK", 0);
        if (!this->is_mypage) {
            this->controls->add(KEY_PLUS, "Exit", 1);
        }

        // Set active element
        this->active_element = (int)ActiveElement::SideMenu;
        this->menu->setActive(true);
        this->list->setActive(false);
    }

    void Settings::event(SDL_Event e) {
        switch (e.type) {
            // Button pressed
            case SDL_JOYBUTTONDOWN:
                if (e.jbutton.which == 0 || e.jbutton.which == 99) {
                    if (e.jbutton.button == Utils::key_map[KEY_PLUS]) {
                        if (!this->is_mypage) {
                            ScreenManager::getInstance()->stopLoop();
                        }

                    // Left and right will swap active element
                    } else if (e.jbutton.button == Utils::key_map[KEY_DLEFT]) {
                        this->active_element = (int)ActiveElement::SideMenu;
                        this->menu->setActive(true);
                        this->list->setActive(false);
                    } else if (e.jbutton.button == Utils::key_map[KEY_DRIGHT]) {
                        this->active_element = (int)ActiveElement::List;
                        this->menu->setActive(false);
                        this->list->setActive(true);

                    // All other buttons get handled by active element
                    } else {
                        switch (this->active_element) {
                            case (int)ActiveElement::SideMenu:
                                this->menu->handleButton(e.jbutton.button, e.jbutton.state);
                                break;

                            case (int)ActiveElement::List:
                                this->list->handleButton(e.jbutton.button, e.jbutton.state);
                                break;
                        }
                    }
                }
                break;

            case SDL_JOYBUTTONUP:
                if (e.jbutton.which == 0 || e.jbutton.which == CONTROLS_ID) {
                    switch (this->active_element) {
                        case (int)ActiveElement::SideMenu:
                            this->menu->handleButton(e.jbutton.button, e.jbutton.state);
                            break;

                        case (int)ActiveElement::List:
                            this->list->handleButton(e.jbutton.button, e.jbutton.state);
                            break;
                    }
                }
                break;

            // Touch (pressed)
            case SDL_FINGERDOWN: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                // Pressed within list
                if (x >= this->list->getX() && x <= this->list->getX() + this->list->getW() && y >= this->list->getY() && y <= this->list->getY() + this->list->getH()) {
                    this->list->touched(e.type, x, y);
                    this->active_element = (int)ActiveElement::List;
                    this->menu->setActive(false);
                    this->list->setActive(true);

                // Pressed within menu
                } else if (x >= this->menu->getX() && x <= this->menu->getX() + this->menu->getW() && y >= this->menu->getY() && y <= this->menu->getY() + this->menu->getH()) {
                    this->menu->touched(e.type, x, y);
                    this->active_element = (int)ActiveElement::SideMenu;
                    this->menu->setActive(true);
                    this->list->setActive(false);

                // Pass event to controls object if below bottom line
                } else if (y > 647) {
                    this->controls->touched(e.type, x, y);
                }
                break;
            }

            // Touch (moved)
            case SDL_FINGERMOTION: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;
                float dx = WIDTH * e.tfinger.dx;
                float dy = HEIGHT * e.tfinger.dy;

                // // List scrolling overrides any other actions
                if (this->list->isTouched()) {
                    this->list->touched(e.type, x, y, dx, dy);

                // Moved after being in menu
                } else if ((x-dx) >= this->menu->getX() && (x-dx) <= this->menu->getX() + this->menu->getW() && (y-dy) >= this->menu->getY() && (y-dy) <= this->menu->getY() + this->menu->getH()) {
                    this->menu->touched(e.type, x, y);

                } else {
                    // Pass event to controls object if was below or originally below line
                    if (y > 647 || (HEIGHT * (e.tfinger.y - e.tfinger.dy)) > 647) {
                        this->controls->touched(e.type, x, y);
                    }
                }
                break;
            }

            // Touch (released)
            case SDL_FINGERUP: {
                float x = WIDTH * e.tfinger.x;
                float y = HEIGHT * e.tfinger.y;

                if (this->list->isTouched()) {
                    this->list->touched(e.type, x, y, (WIDTH * e.tfinger.dx), (HEIGHT * e.tfinger.dy));

                // Released within menu
                } else if (x >= this->menu->getX() && x <= this->menu->getX() + this->menu->getW() && y >= this->menu->getY() && y <= this->menu->getY() + this->menu->getH()) {
                    this->menu->touched(e.type, x, y);

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

    void Settings::update(uint32_t dt) {
        Screen::update(dt);
        this->menu->update(dt);
        this->list->update(dt);

        if (ScreenManager::getInstance()->touch_active) {
            this->controls->disable(KEY_A);
        } else {
            this->controls->enable(KEY_A);
        }

        switch (this->menu->getSelected()) {
            // Recent Activity
            case 0:
                ScreenManager::getInstance()->setScreen(new RecentActivity(this->user));
                break;
            // All Activity
            case 1:
                ScreenManager::getInstance()->popScreen();
                break;
            // Settings (this so do nothing)
            case 2:
                break;
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
        SDLHelper::drawRect(430, 648, 780, 72);

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

    Settings::~Settings() {
        delete this->list;
    }
};