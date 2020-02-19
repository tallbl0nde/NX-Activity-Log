#include "AllActivity.hpp"

namespace Screen {
    AllActivity::AllActivity(Main::Application * a) {
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
        c->addItem(new Aether::ControlItem(Aether::Button::X, "Sort"));
        c->setColour(this->app->theme()->text());
        this->addElement(c);

        // Create sort overlay
        this->sortOverlay = new Aether::PopupList("Sort Titles");
        this->sortOverlay->setBackgroundColour(this->app->theme()->altBG());
        this->sortOverlay->setHighlightColour(this->app->theme()->accent());
        this->sortOverlay->setLineColour(this->app->theme()->fg());
        this->sortOverlay->setListLineColour(this->app->theme()->mutedLine());
        this->sortOverlay->setTextColour(this->app->theme()->text());

        // Add callbacks for buttons
        this->onButtonPress(Aether::Button::X, [this](){
            this->setupOverlay();
        });
        if (!(this->app->isUserPage())) {
            this->onButtonPress(Aether::Button::B, [this](){
                this->app->setScreen(Main::ScreenID::UserSelect);
            });
        }
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
    }

    void AllActivity::setupOverlay() {
        // Empty previous entries
        this->sortOverlay->close(false);
        this->sortOverlay->removeEntries();

        // Add entries and highlight current sort
        SortType t = this->list->sort();
        this->sortOverlay->addEntry("By Name", [this](){
            this->list->setSort(SortType::AlphaAsc);
        }, t == SortType::AlphaAsc);
        this->sortOverlay->addEntry("By First Playtime", [this](){
            this->list->setSort(SortType::FirstPlayed);
        }, t == SortType::FirstPlayed);
        this->sortOverlay->addEntry("By Most Recently Played", [this](){
            this->list->setSort(SortType::LastPlayed);
        }, t == SortType::LastPlayed);
        this->sortOverlay->addEntry("By Most Playtime", [this](){
            this->list->setSort(SortType::HoursAsc);
        }, t == SortType::HoursAsc);
        this->sortOverlay->addEntry("By Least Playtime", [this](){
            this->list->setSort(SortType::HoursDec);
        }, t == SortType::HoursDec);
        this->sortOverlay->addEntry("By Most Launched", [this](){
            this->list->setSort(SortType::LaunchAsc);
        }, t == SortType::LaunchAsc);
        this->sortOverlay->addEntry("By Least Launched", [this](){
            this->list->setSort(SortType::LaunchDec);
        }, t == SortType::LaunchDec);

        // List is focussed when overlay is closed
        this->setFocussed(this->list);

        this->app->addOverlay(this->sortOverlay);
    }

    void AllActivity::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, this->app->activeUser()->username() + "'s Activity", 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(this->app->theme()->text());
        this->addElement(this->heading);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize(), 4, 4);
        this->image->setWH(60, 60);
        this->addElement(this->image);

        // Create side menu
        this->menu = new Aether::Menu(30, 88, 388, 559);
        this->menu->addElement(new Aether::MenuOption("Recent Activity", this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(Main::ScreenID::RecentActivity);
        }));
        Aether::MenuOption * opt = new Aether::MenuOption("All Activity", this->app->theme()->accent(), this->app->theme()->text(), nullptr);
        this->menu->addElement(opt);
        this->menu->setActiveOption(opt);
        this->menu->addElement(new Aether::MenuSeparator(this->app->theme()->mutedLine()));
        this->menu->addElement(new Aether::MenuOption("Settings", this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(Main::ScreenID::Settings);
        }));
        this->menu->setFocussed(opt);
        this->addElement(this->menu);

        // Create list
        this->list = new CustomElm::SortedList(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setHeadingColour(this->app->theme()->mutedText());
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // Populate list + count total time
        std::vector<NX::Title *> t = this->app->titleVector();
        unsigned int totalSecs = 0;
        for (size_t i = 0; i < t.size(); i++) {
            // Skip over deleted titles based on config value
            if (!(t[i]->isInstalled()) && this->app->config()->hDeleted()) {
                continue;
            }

            NX::PlayStatistics * ps = this->app->playdata()->getStatisticsForUser(t[i]->titleID(), this->app->activeUser()->ID());
            totalSecs += ps->playtime;
            if (ps->launches == 0) {
                // Skip unplayed titles
                delete ps;
                continue;
            }

            // "Convert" PlayStatistics to SortInfo
            SortInfo * si = new SortInfo;
            si->name = t[i]->name();
            si->titleID = t[i]->titleID();
            si->firstPlayed = ps->firstPlayed;
            si->lastPlayed = ps->lastPlayed;
            si->playtime = ps->playtime;
            si->launches = ps->launches;

            // Create ListActivity and add to list
            CustomElm::ListActivity * la = new CustomElm::ListActivity();
            la->setImage(new Aether::Image(0, 0, t[i]->imgPtr(), t[i]->imgSize(), 2, 2));
            la->setTitle(t[i]->name());
            std::string str = "Played for " + Utils::Time::playtimeToString(ps->playtime, " and ");
            la->setPlaytime(str);
            str = Utils::Time::lastPlayedTimestampToString(pdmPlayTimestampToPosix(ps->lastPlayed));
            la->setLeftMuted(str);
            str = "Played " + std::to_string(ps->launches);
            (ps->launches == 1) ? str += " time" : str += " times";
            la->setRightMuted(str);
            la->setCallback([this, i](){
                this->app->setActiveTitle(i);
                this->app->setScreen(Main::ScreenID::Details);
            });
            la->setTitleColour(this->app->theme()->text());
            la->setPlaytimeColour(this->app->theme()->accent());
            la->setMutedColour(this->app->theme()->mutedText());
            la->setLineColour(this->app->theme()->mutedLine());
            this->list->addElement(la, si);
        }

        // Sort the list
        this->list->setSort(this->app->config()->gSort());
        this->addElement(this->list);

        // Render total hours string
        std::string txt = "Total Playtime: " + Utils::Time::playtimeToString(totalSecs, " and ");
        this->hours = new Aether::Text(1215, 44, txt, 20);
        this->hours->setXY(this->hours->x() - this->hours->w(), this->hours->y() - this->hours->h()/2);
        this->hours->setColour(this->app->theme()->mutedText());
        this->addElement(this->hours);
    }

    void AllActivity::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->hours);
        this->removeElement(this->image);
        this->removeElement(this->list);
        this->removeElement(this->menu);
    }

    AllActivity::~AllActivity() {
        delete this->sortOverlay;
    }
};