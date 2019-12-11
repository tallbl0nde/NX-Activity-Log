#include <algorithm>
#include "config.hpp"
#include "listitem_activity.hpp"
#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include "TimeHelper.hpp"
#include "utils.hpp"

// Maximum number of titles to read using pdm...()
#define MAX_TITLES 2000

// Reads all installed title IDs and creates Title objects using them
// The caller must deallocate the memory
static std::vector<Title *> getTitleObjects(AccountUid userID) {
    Result rc;

    // Get ALL played titles (this doesn't include installed games that haven't been played)
    s32 playedTotal = 0;
    u64 * playedIDs = new u64[MAX_TITLES];
    pdmqryQueryRecentlyPlayedApplication(userID, playedIDs, MAX_TITLES, &playedTotal);

    // Get all installed titles
    std::vector<u64> installedIDs;
    NsApplicationRecord * records = new NsApplicationRecord[MAX_TITLES];
    s32 count = 0;
    s32 installedTotal = 0;
    while (true){
        rc = nsListApplicationRecord(records, MAX_TITLES, count, &installedTotal);
        // Break if at the end or no titles
        if (R_FAILED(rc) || installedTotal == 0){
            break;
        }
        count++;
        installedIDs.push_back(records->application_id);
    }

    // Create Title objects from IDs
    std::vector<Title *> titles;
    for (s32 i = 0; i < playedTotal; i++) {
        // Loop over installed titles to determine if installed or not
        bool installed = false;
        for (size_t j = 0; j < installedIDs.size(); j++) {
            if (installedIDs[j] == playedIDs[i]) {
                installed = true;
                break;
            }
        }

        titles.push_back(new Title(playedIDs[i], userID, installed));
    }

    delete[] playedIDs;
    delete[] records;

    return titles;
}

namespace Screen {
    AllActivity::AllActivity(User * u) : Screen::Screen() {
        this->hide_deleted = Config::getConfig()->getHiddenDeleted();
        this->list = nullptr;
        this->user = u;
        this->titles = getTitleObjects(this->user->getID());
        this->total_hours = nullptr;

        // Generate list
        this->generateList();

        // Create side menu
        this->menu->addItem(new UI::SideItem("Recent Activity"));
        this->menu->addItem(new UI::SideItem("All Activity"));
        this->menu->addItem(new UI::SideItem("Settings"));
        this->menu->setSelected(1);

        // Set controls
        this->controls->add(KEY_A, "OK", 0);
        this->controls->add(KEY_X, "Sort", 1);
        if (!this->is_mypage) {
            this->controls->add(KEY_PLUS, "Exit", 2);
        }

        // Set active element
        this->active_element = (int)ActiveElement::List;
        this->menu->setActive(false);
        this->list->setActive(true);
    }

    void AllActivity::event(SDL_Event e){
        switch (e.type) {
            // Button pressed
            case SDL_JOYBUTTONDOWN:
                if (e.jbutton.which == 0 || e.jbutton.which == CONTROLS_ID) {
                    // Plus exits app
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

                    // X presents sort selection
                    } else if (e.jbutton.button == Utils::key_map[KEY_X]) {
                        std::vector<std::string> v;
                        v.push_back("By Name");
                        v.push_back("By Most Played");
                        v.push_back("By Least Played");
                        v.push_back("By Most Launched");
                        v.push_back("By Least Launched");
                        v.push_back("By First Playtime");
                        v.push_back("By Recently Played");
                        this->list->setActive(false);
                        this->menu->setActive(false);
                        ScreenManager::getInstance()->createSelection("Sort Titles", v, [this](int c){
                            switch (c) {
                                case -1:
                                    // Do nothing if nothing selected
                                    break;
                                case 0:
                                    this->list->sort(AlphaAsc);
                                    break;
                                case 1:
                                    this->list->sort(HoursAsc);
                                    break;
                                case 2:
                                    this->list->sort(HoursDec);
                                    break;
                                case 3:
                                    this->list->sort(LaunchAsc);
                                    break;
                                case 4:
                                    this->list->sort(LaunchDec);
                                    break;
                                case 5:
                                    this->list->sort(FirstPlayed);
                                    break;
                                case 6:
                                    this->list->sort(LastPlayed);
                                    break;
                            }
                        });

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

                // List scrolling overrides any other actions
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

    void AllActivity::update(uint32_t dt) {
        Screen::update(dt);
        this->menu->update(dt);

        // Check if hidden option toggled and if so regen list
        if (this->hide_deleted != Config::getConfig()->getHiddenDeleted()) {
            this->generateList();
            this->hide_deleted = Config::getConfig()->getHiddenDeleted();
        }
        this->list->update(dt);

        if (ScreenManager::getInstance()->touch_active) {
            this->controls->disable(KEY_A);
        } else {
            this->controls->enable(KEY_A);
        }

        // Change screen if menu selected
        switch (this->menu->getSelected()) {
            // Recent Activity
            case 0:
                ScreenManager::getInstance()->pushScreen();
                ScreenManager::getInstance()->setScreen(new RecentActivity(this->user));
                break;
            // All Activity (this one so do nothing)
            case 1:
                break;
            // Settings
            case 2:
                ScreenManager::getInstance()->pushScreen();
                ScreenManager::getInstance()->setScreen(new Settings(this->user));
                break;
        }

        // Change to details if listitem chosen
        if (this->list->getChosen() != -1) {
            ScreenManager::getInstance()->pushScreen();
            ScreenManager::getInstance()->setScreen(new Details(this->user, this->list->getTitleVector(), this->list->getChosen()));
        }
    }

    void AllActivity::draw() {
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

        // Print total hours
        int tw, th;
        SDLHelper::getDimensions(this->total_hours, &tw, &th);
        SDLHelper::drawTexture(this->total_hours, this->theme->getMutedText(), 1215 - tw, 44 - th/2);

        // Draw controls
        this->controls->draw();
    }

    void AllActivity::generateList() {
        // Delete old list
        if (this->list != nullptr) {
            delete this->list;
        }
        this->list = new UI::SortedList(&ScreenManager::getInstance()->touch_active, 400, 110, 850, 515);

        // Add items to list based on config
        Config * conf = Config::getConfig();
        u32 total_mins = 0;
        for (size_t i = 0; i < this->titles.size(); i++) {
            // Hide deleted if toggled
            if (!this->titles[i]->getInstalled() && conf->getHiddenDeleted()) {
                continue;
            }

            this->list->addItem(new UI::ListItem::Activity(this->titles[i]));
            total_mins += this->titles[i]->getPlaytime();
        }
        // "Adding" nullptr finalizes list
        this->list->addItem(nullptr);

        // Create total hours texture
        std::string str = "Total Playtime: ";
        str += TimeH::playtimeToString(total_mins * 60, " and ");
        if (this->total_hours != nullptr) {
            SDLHelper::destroyTexture(this->total_hours);
            this->total_hours = nullptr;
        }
        this->total_hours = SDLHelper::renderText(str.c_str(), BODY_FONT_SIZE);
    }

    AllActivity::~AllActivity() {
        if (this->list != nullptr) {
            delete this->list;
        }

        if (this->total_hours != nullptr) {
            SDLHelper::destroyTexture(this->total_hours);
        }

        // Delete title objects
        while (this->titles.size() > 0) {
            delete this->titles[0];
            this->titles.erase(this->titles.begin());
        }
    }
}