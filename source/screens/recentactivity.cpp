#include "listitem_recentactivity.hpp"
#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include "TimeHelper.hpp"
#include "utils.hpp"

namespace Screen {
    RecentActivity::RecentActivity(User * u) : Screen::Screen() {
        this->empty_list = false;
        this->list = new UI::DatedList(&ScreenManager::getInstance()->touch_active, 400, 110, 850, 515);
        this->playdata = new PlayData();
        this->user = u;
        this->total_hours = nullptr;

        // Generate no_activity texture
        std::string str = "No play activity was recorded for this period of time.";
        this->no_activity = SDLHelper::renderText(str.c_str(), BODY_FONT_SIZE);

        // Set initial range to current day
        this->start_tm = TimeH::getTmForCurrentTime();
        this->start_tm.tm_hour = 0;
        this->start_tm.tm_min = 0;
        this->start_tm.tm_sec = 0;
        this->view_period = TP_Day;

        // Populate list
        this->generateList();

        // Set controls
        this->controls->add(KEY_A, "OK", 0);
        if (!this->is_mypage) {
            this->controls->add(KEY_PLUS, "Exit", 1);
        }
        this->controls->add(KEY_R, "Next", 2);
        this->controls->add(KEY_L, "Previous", 3);
        this->controls->add(KEY_X, "Change View", 4);

        // Set active element
        this->active_element = (int)ActiveElement::SideMenu;
        this->menu->setActive(true);
        this->list->setActive(false);
    }

    void RecentActivity::generateList() {
        // Delete old list
        if (this->list != nullptr) {
            delete this->list;
        }
        this->list = new UI::DatedList(&ScreenManager::getInstance()->touch_active, 400, 110, 850, 515);

        // Get play statistics for current time period
        std::vector<u64> ids = this->playdata->getLoggedTitleIDs();
        u32 total_secs = 0;
        time_t start_ts = std::mktime(&this->start_tm);
        struct tm end_tm = this->start_tm;
        end_tm.tm_hour = 23;
        end_tm.tm_min = 59;
        end_tm.tm_sec = 59;
        switch (this->view_period) {
            case TP_Day:
                // Do nothing as it's already the same day
                break;

            case TP_Year:
                // Set to last month
                end_tm.tm_mon = 11;
            case TP_Month:
                // Set to last day in month
                end_tm.tm_mday = TimeH::tmGetDaysInMonth(this->start_tm);
                break;
        }
        time_t end_ts = std::mktime(&end_tm);

        std::vector<RecentPlayStatistics *> stats;
        for (size_t i = 0; i < ids.size(); i++) {
            RecentPlayStatistics * stat = this->playdata->getRecentStatisticsForUser(ids[i], start_ts, end_ts, this->user->getID());
            // Only show games that have actually been played
            if (stat->playtime > 0) {
                stats.push_back(stat);
            }
        }
        if (stats.size() == 0) {
            empty_list = true;
        } else {
            empty_list = false;
        }

        std::sort(stats.begin(), stats.end(), [&](RecentPlayStatistics * lhs, RecentPlayStatistics * rhs){
            return lhs->playtime > rhs->playtime;
        });
        for (size_t i = 0; i < stats.size(); i++) {
            this->list->addItem(new UI::ListItem::RecentActivity(stats[i], i + 1));
            total_secs += stats[i]->playtime;
            delete stats[i];
        }

        // Generate total time texture
        std::string str = "Total Playtime: ";
        if (total_secs != 0) {
            str += TimeH::playtimeToString(total_secs, " and ");
        } else {
            str += "0 minutes";
        }
        if (this->total_hours != nullptr) {
            SDLHelper::destroyTexture(this->total_hours);
        }
        this->total_hours = SDLHelper::renderText(str.c_str(), BODY_FONT_SIZE);

        // Set date string for list
        bool day = true, month = true, year = true;
        switch (this->view_period) {
            case TP_Day: {
                // Hide year if this year
                struct tm now = TimeH::getTmForCurrentTime();
                if (this->start_tm.tm_year == now.tm_year) {
                    year = false;
                }
                break;
            }

            case TP_Month:
                // Don't show day
                day = false;
                break;

            case TP_Year:
                // Only show year
                day = false;
                month = false;
                break;
        }
        this->list->setDate(TimeH::tmToString(start_tm, day, month, year));
        this->menu->setActive(false);
        this->list->setActive(true);
        this->active_element = (int)ActiveElement::List;
    }

    void RecentActivity::event(SDL_Event e){
        switch (e.type) {
            // Button pressed
            case SDL_JOYBUTTONDOWN:
                if (e.jbutton.which == 0 || e.jbutton.which == 99) {
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

                    // X brings up period to view
                    } else if (e.jbutton.button == Utils::key_map[KEY_X]) {
                        std::vector<std::string> v;
                        v.push_back("By Day");
                        v.push_back("By Month");
                        v.push_back("By Year");
                        this->list->setActive(false);
                        this->menu->setActive(false);
                        ScreenManager::getInstance()->createSelection("View Activity", v, [this](int c) {
                            switch (c) {
                                case -1:
                                    // Do nothing if nothing selected
                                    break;

                                case 0: {
                                    // Set to day
                                    this->view_period = TP_Day;
                                    this->start_tm = TimeH::getTmForCurrentTime();
                                    this->start_tm.tm_hour = 0;
                                    this->start_tm.tm_min = 0;
                                    this->start_tm.tm_sec = 0;
                                    this->generateList();
                                    break;
                                }

                                case 1: {
                                    // Set to month
                                    this->view_period = TP_Month;
                                    this->start_tm = TimeH::getTmForCurrentTime();
                                    this->start_tm.tm_mday = 1;
                                    this->start_tm.tm_hour = 0;
                                    this->start_tm.tm_min = 0;
                                    this->start_tm.tm_sec = 0;
                                    this->generateList();
                                    break;
                                }

                                case 2: {
                                    // Set to year
                                    this->view_period = TP_Year;
                                    this->start_tm = TimeH::getTmForCurrentTime();
                                    this->start_tm.tm_mon = 0;
                                    this->start_tm.tm_mday = 1;
                                    this->start_tm.tm_hour = 0;
                                    this->start_tm.tm_min = 0;
                                    this->start_tm.tm_sec = 0;
                                    this->generateList();
                                    break;
                                }
                            }
                        });

                    // L goes back a day/month/year
                    } else if (e.jbutton.button == Utils::key_map[KEY_L]) {
                        if (this->decreaseTm()) {
                            this->generateList();
                        }

                    // R goes forward a day/month/year
                    } else if (e.jbutton.button == Utils::key_map[KEY_R]) {
                        if (this->increaseTm()) {
                            this->generateList();
                        }

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
                if (e.jbutton.which == 0 || e.jbutton.which == 99) {
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

    void RecentActivity::update(uint32_t dt) {
        Screen::update(dt);
        this->menu->update(dt);

        if (ScreenManager::getInstance()->touch_active) {
            this->controls->disable(KEY_A);
        } else {
            this->controls->enable(KEY_A);
        }
        this->list->update(dt);

        // Change screen if menu selected
        switch (this->menu->getSelected()) {
            // Recent Activity (this one so do nothing)
            case 0:
                break;
            // All Activity
            case 1:
                ScreenManager::getInstance()->popScreen();
                break;
            // Settings
            case 2:
                ScreenManager::getInstance()->setScreen(new Settings(this->user));
                break;
        }
    }

    void RecentActivity::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->getBG());
        SDLHelper::clearScreen();

        // Draw menu
        this->menu->draw();

        // Different shade behind list
        SDLHelper::setColour(this->theme->getAltBG());
        SDLHelper::drawRect(400, 88, 850, 560);

        // Draw text if list is empty
        if (this->empty_list) {
            int tw, th;
            SDLHelper::getDimensions(this->no_activity, &tw, &th);
            SDLHelper::drawTexture(this->no_activity, this->theme->getMutedText(), this->list->getX() + (this->list->getW() - tw)/2, this->list->getY() + 200);
        }

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

     bool RecentActivity::decreaseTm() {
        switch (this->view_period) {
            case TP_Day:
                // Prevent going earlier than 3rd March 2017
                if (this->start_tm.tm_mday == 3 && this->start_tm.tm_mon == 2 && this->start_tm.tm_year == 117) {
                    return false;
                }
                this->start_tm.tm_mday--;
                break;
            case TP_Month:
                // Prevent going earlier than March 2017
                if (this->start_tm.tm_mon == 2 && this->start_tm.tm_year == 117) {
                    return false;
                }
                this->start_tm.tm_mon--;
                break;
            case TP_Year:
                // Prevent going earlier than 2017
                if (this->start_tm.tm_year == 117) {
                    return false;
                }
                this->start_tm.tm_year--;
                break;
        }

        // Fix negative days
        if (this->start_tm.tm_mday < 1) {
            // Fix month (and year if need be)
            this->start_tm.tm_mon--;
            if (this->start_tm.tm_mon < 0) {
                this->start_tm.tm_year--;
                this->start_tm.tm_mon = 11;
            }

            // Fix day
            this->start_tm.tm_mday = TimeH::tmGetDaysInMonth(this->start_tm);
        }

        // Fix negative month
        if (this->start_tm.tm_mon < 0) {
            this->start_tm.tm_year--;
            this->start_tm.tm_mon = 11;
        }

        return true;
    }

    bool RecentActivity::increaseTm() {
        // Struct for current time (to prevent going into the future)
        struct tm now = TimeH::getTmForCurrentTime();

        switch (this->view_period) {
            case TP_Day:
                if (this->start_tm.tm_mday == now.tm_mday && this->start_tm.tm_mon == now.tm_mon && this->start_tm.tm_year == now.tm_year) {
                    return false;
                }
                this->start_tm.tm_mday++;
                break;
            case TP_Month:
                if (this->start_tm.tm_mon == now.tm_mon && this->start_tm.tm_year == now.tm_year) {
                    return false;
                }
                this->start_tm.tm_mon++;
                break;
            case TP_Year:
                if (this->start_tm.tm_year == now.tm_year) {
                    return false;
                }
                this->start_tm.tm_year++;
                break;
        }

        // Fix days
        int d = TimeH::tmGetDaysInMonth(this->start_tm);
        if (this->start_tm.tm_mday > d) {
            // Fix month (and year if need be)
            this->start_tm.tm_mday = 1;
            this->start_tm.tm_mon++;
            if (this->start_tm.tm_mon > 11) {
                this->start_tm.tm_year++;
                this->start_tm.tm_mon = 0;
            }
        }

        // Fix month
        if (this->start_tm.tm_mon > 11) {
            this->start_tm.tm_year++;
            this->start_tm.tm_mon = 0;
        }

        return true;
    }

    RecentActivity::~RecentActivity() {
        delete this->list;
        SDLHelper::destroyTexture(this->no_activity);
        delete this->playdata;
        if (this->total_hours != nullptr) {
            SDLHelper::destroyTexture(this->total_hours);
        }
    }
}