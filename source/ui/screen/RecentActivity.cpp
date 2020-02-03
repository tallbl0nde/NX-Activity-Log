#include "RecentActivity.hpp"

namespace Screen {
    RecentActivity::RecentActivity(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(400, 88, 850, 559);
        r->setColour(this->app->theme()->altBG());
        this->addElement(r);
        r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::Controls * c = new Aether::Controls();
        c->addItem(new Aether::ControlItem(Aether::Button::A, "OK"));
        if (!(this->app->isUserPage())) {
            c->addItem(new Aether::ControlItem(Aether::Button::B, "Back"));
        }
        c->addItem(new Aether::ControlItem(Aether::Button::R, "Next"));
        c->addItem(new Aether::ControlItem(Aether::Button::L, "Previous"));
        c->addItem(new Aether::ControlItem(Aether::Button::X, "Change View"));
        c->setColour(this->app->theme()->text());
        this->addElement(c);

        // Create side menu
        Aether::Menu * menu = new Aether::Menu(30, 88, 388, 559);
        Aether::MenuOption * opt = new Aether::MenuOption("Recent Activity", this->app->theme()->accent(), this->app->theme()->text(), nullptr);
        opt->setActive(true);
        menu->addElement(opt);
        menu->addElement(new Aether::MenuOption("All Activity", this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(Main::ScreenID::AllActivity);
        }));
        menu->addElement(new Aether::MenuSeparator(this->app->theme()->mutedLine()));
        menu->addElement(new Aether::MenuOption("Settings", this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(Main::ScreenID::Settings);
        }));
        this->addElement(menu);

        this->noStats = new Aether::Text(825, 300, "No play activity was recorded for this period of time.", 20);
        this->noStats->setXY(this->noStats->x() - this->noStats->w()/2, this->noStats->y() - this->noStats->h()/2);
        this->noStats->setColour(this->app->theme()->mutedText());
        this->addElement(this->noStats);

        // Create overlay
        this->viewOverlay = new Aether::PopupList("View Activity");
        this->viewOverlay->setBackgroundColour(this->app->theme()->altBG());
        this->viewOverlay->setHighlightColour(this->app->theme()->accent());
        this->viewOverlay->setLineColour(this->app->theme()->fg());
        this->viewOverlay->setListLineColour(this->app->theme()->mutedLine());
        this->viewOverlay->setTextColour(this->app->theme()->text());

        if (!(this->app->isUserPage())) {
            this->onButtonPress(Aether::Button::B, [this](){
                this->app->setScreen(Main::ScreenID::UserSelect);
            });
        }
        this->onButtonPress(Aether::Button::X, [this](){
            this->setupOverlay();
        });
        this->onButtonPress(Aether::Button::R, [this](){
            if (this->updateStartTime(1)) {
                this->updateList();
            }
        });
        this->onButtonPress(Aether::Button::L, [this](){
            if (this->updateStartTime(-1)) {
                this->updateList();
            }
        });
        this->onButtonPress(Aether::Button::ZR, [this](){
            this->app->setHoldDelay(30);
        });
        this->onButtonRelease(Aether::Button::ZR, [this](){
            this->app->setHoldDelay(100);
        });
        this->onButtonPress(Aether::Button::ZL, [this](){
            this->app->setHoldDelay(30);
        });
        this->onButtonRelease(Aether::Button::ZL, [this](){
            this->app->setHoldDelay(100);
        });

        // Set view time to the current day
        this->startTime =  Utils::Time::getTmForCurrentTime();
        this->startTime.tm_hour = 0;
        this->startTime.tm_min = 0;
        this->startTime.tm_sec = 0;
        this->viewType = Day;
    }

    void RecentActivity::setupOverlay() {
        this->viewOverlay->close(false);
        this->viewOverlay->removeEntries();

        this->viewOverlay->addEntry("By Day", [this](){
            if (!(this->viewType == Day)) {
                this->viewType = Day;
                this->startTime = Utils::Time::getTmForCurrentTime();
                this->startTime.tm_hour = 0;
                this->startTime.tm_min = 0;
                this->startTime.tm_sec = 0;
                this->updateList();
            }
        }, this->viewType == Day);

        this->viewOverlay->addEntry("By Month", [this](){
            if (!(this->viewType == Month)) {
                this->viewType = Month;
                this->startTime.tm_mday = 1;
                this->startTime.tm_hour = 0;
                this->startTime.tm_min = 0;
                this->startTime.tm_sec = 0;
                this->updateList();
            }
        }, this->viewType == Month);

        this->viewOverlay->addEntry("By Year", [this](){
            if (!(this->viewType == Year)) {
                this->viewType = Year;
                this->startTime.tm_mon = 0;
                this->startTime.tm_mday = 1;
                this->startTime.tm_hour = 0;
                this->startTime.tm_min = 0;
                this->startTime.tm_sec = 0;
                this->updateList();
            }
        }, this->viewType == Year);

        this->app->addOverlay(this->viewOverlay);
    }

    void RecentActivity::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, this->app->activeUser()->username() + "'s Activity", 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(this->app->theme()->text());
        this->addElement(this->heading);

        // Render total hours string
        this->hours = new Aether::Text(1215, 44, "y", 20);
        this->hours->setY(this->hours->y() - this->hours->h()/2);
        this->hours->setColour(this->app->theme()->mutedText());
        this->addElement(this->hours);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize(), 4, 4);
        this->image->setWH(60, 60);
        this->addElement(this->image);

        // Create list
        this->list = new Aether::List(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());
        this->addElement(this->list);
        this->updateList();
    }

    void RecentActivity::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->hours);
        this->removeElement(this->image);
        this->removeElement(this->list);
    }

    void RecentActivity::updateList() {
        this->list->removeAllElements();

        // Change heading to new time
        bool day = true, month = true, year = true;
        switch (this->viewType) {
            case Day: {
                // Hide year if this year
                struct tm now = Utils::Time::getTmForCurrentTime();
                if (this->startTime.tm_year == now.tm_year) {
                    year = false;
                }
                break;
            }

            case Month:
                // Don't show day
                day = false;
                break;

            case Year:
                // Only show year
                day = false;
                month = false;
                break;
        }
        std::string ss = "Activity for: " + Utils::Time::tmToString(this->startTime, day, month, year);
        Aether::Text * text = new Aether::Text(0, 0, ss, 20);
        text->setColour(this->app->theme()->text());
        this->list->addElement(text);
        text->setW(text->texW());
        text->setXY(this->list->x() + (this->list->w() - text->w())/2, text->y() - 15);
        this->list->addElement(new Aether::ListSeparator(20));

        // Get end time (required for getting stats)
        struct tm end = this->startTime;
        end.tm_hour = 23;
        end.tm_min = 59;
        end.tm_sec = 59;
        switch (this->viewType) {
            case Day:
                // Do nothing as it's already the same day
                break;

            case Year:
                // Set to last month
                end.tm_mon = 11;
            case Month:
                // Set to last day in month
                end.tm_mday = Utils::Time::tmGetDaysInMonth(this->startTime);
                break;
        }

        // Get stats and add ListActivities
        unsigned int totalSecs = 0;
        time_t startT = Utils::Time::getTimeT(this->startTime);
        time_t endT = Utils::Time::getTimeT(end);
        for (size_t i = 0; i < this->app->titleVector().size(); i++) {
            NX::RecentPlayStatistics * stat = this->app->playdata()->getRecentStatisticsForUser(this->app->titleVector()[i]->titleID(), startT, endT, this->app->activeUser()->ID());
            // Only show games that have actually been played
            if (stat->playtime > 0) {
                totalSecs += stat->playtime;
                CustomElm::ListActivity * la = new CustomElm::ListActivity();
                la->setImage(new Aether::Image(0, 0, this->app->titleVector()[i]->imgPtr(), this->app->titleVector()[i]->imgSize(), 2, 2));
                la->setTitle(this->app->titleVector()[i]->name());
                std::string str = "Played for " + Utils::Time::playtimeToString(stat->playtime, " and ");
                la->setPlaytime(str);
                str = "Played " + std::to_string(stat->launches);
                (stat->launches == 1) ? str += " time" : str += " times";
                la->setLeftMuted(str);
                la->setCallback([](){

                });
                la->setTitleColour(this->app->theme()->text());
                la->setPlaytimeColour(this->app->theme()->accent());
                la->setMutedColour(this->app->theme()->mutedText());
                la->setLineColour(this->app->theme()->mutedLine());
                this->list->addElement(la);
            }

            delete stat;
        }

        // Show "no activity" string if needed
        if (totalSecs == 0) {
            this->noStats->setHidden(false);
            this->hours->setString("Total Playtime: 0 minutes");
        } else {
            this->noStats->setHidden(true);
            std::string txt = "Total Playtime: " + Utils::Time::playtimeToString(totalSecs, " and ");
            this->hours->setString(txt);
        }

        // Update playtime string
        this->hours->setX(1215 - this->hours->w());
    }

    bool RecentActivity::updateStartTime(int i) {
        if (i < 0) {
            // Decrease
            switch (this->viewType) {
                case Day:
                    if (this->startTime.tm_mday == 3 && this->startTime.tm_mon == 2 && this->startTime.tm_year == 117) {
                        return false;
                    }
                    this->startTime.tm_mday--;
                    break;

                case Month:
                    // Prevent going earlier than March 2017
                    if (this->startTime.tm_mon == 2 && this->startTime.tm_year == 117) {
                        return false;
                    }
                    this->startTime.tm_mon--;
                    break;

                case Year:
                    // Prevent going earlier than 2017
                    if (this->startTime.tm_year == 117) {
                        return false;
                    }
                    this->startTime.tm_year--;
                    break;
            }

             // Fix negative days
            if (this->startTime.tm_mday < 1) {
                // Fix month (and year if need be)
                this->startTime.tm_mon--;
                if (this->startTime.tm_mon < 0) {
                    this->startTime.tm_year--;
                    this->startTime.tm_mon = 11;
                }

                // Fix day
                this->startTime.tm_mday = Utils::Time::tmGetDaysInMonth(this->startTime);
            }

            // Fix negative month
            if (this->startTime.tm_mon < 0) {
                this->startTime.tm_year--;
                this->startTime.tm_mon = 11;
            }
        } else if (i > 0) {
            // Increase
            // Struct for current time (to prevent going into the future)
            struct tm now = Utils::Time::getTmForCurrentTime();

            switch (this->viewType) {
                case Day:
                    if (this->startTime.tm_mday == now.tm_mday && this->startTime.tm_mon == now.tm_mon && this->startTime.tm_year == now.tm_year) {
                        return false;
                    }
                    this->startTime.tm_mday++;
                    break;
                case Month:
                    if (this->startTime.tm_mon == now.tm_mon && this->startTime.tm_year == now.tm_year) {
                        return false;
                    }
                    this->startTime.tm_mon++;
                    break;
                case Year:
                    if (this->startTime.tm_year == now.tm_year) {
                        return false;
                    }
                    this->startTime.tm_year++;
                    break;
            }

            // Fix days
            int d = Utils::Time::tmGetDaysInMonth(this->startTime);
            if (this->startTime.tm_mday > d) {
                // Fix month (and year if need be)
                this->startTime.tm_mday = 1;
                this->startTime.tm_mon++;
                if (this->startTime.tm_mon > 11) {
                    this->startTime.tm_year++;
                    this->startTime.tm_mon = 0;
                }
            }

            // Fix month
            if (this->startTime.tm_mon > 11) {
                this->startTime.tm_year++;
                this->startTime.tm_mon = 0;
            }
        } else {
            // Check constraints
        }

        return true;
    }

    RecentActivity::~RecentActivity() {
        delete this->viewOverlay;
    }
};