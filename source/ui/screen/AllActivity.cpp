#include "AllActivity.hpp"
#include "TimeHelper.hpp"

namespace Screen {
    AllActivity::AllActivity(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(400, 88, 850, 559);
        r->setColour(Aether::Theme::Dark.altBG);
        this->addElement(r);
        r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(Aether::Theme::Dark.fg);
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(Aether::Theme::Dark.fg);
        this->addElement(r);
        Aether::Controls * c = new Aether::Controls();
        c->addItem(new Aether::ControlItem(Aether::Button::A, "OK"));
        c->addItem(new Aether::ControlItem(Aether::Button::B, "Back"));
        c->addItem(new Aether::ControlItem(Aether::Button::X, "Sort"));
        c->setColour(Aether::Theme::Dark.text);
        this->addElement(c);

        // Create side menu
        Aether::Menu * menu = new Aether::Menu(30, 88, 388, 559);
        menu->addElement(new Aether::MenuOption("Recent Activity", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, [this](){
            this->app->setScreen(Main::ScreenID::RecentActivity);
        }));
        Aether::MenuOption * opt = new Aether::MenuOption("All Activity", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, nullptr);
        opt->setActive(true);
        menu->addElement(opt);
        menu->addElement(new Aether::MenuSeparator(Aether::Theme::Dark.mutedLine));
        menu->addElement(new Aether::MenuOption("Settings", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, [this](){
            this->app->setScreen(Main::ScreenID::Settings);
        }));
        this->addElement(menu);

        // Create list (but don't populate yet)
        this->list = new CustomElm::SortedList(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setHeadingColour(Aether::Theme::Dark.mutedText);
        this->list->setScrollBarColour(Aether::Theme::Dark.mutedLine);
        this->addElement(this->list);

        // Create sort overlay
        this->sortOverlay = new Aether::PopupList("Sort Titles");
        this->sortOverlay->setBackgroundColour(Aether::Theme::Dark.altBG);
        this->sortOverlay->setHighlightColour(Aether::Theme::Dark.accent);
        this->sortOverlay->setLineColour(Aether::Theme::Dark.fg);
        this->sortOverlay->setListLineColour(Aether::Theme::Dark.mutedLine);
        this->sortOverlay->setTextColour(Aether::Theme::Dark.text);

        // Add callbacks for buttons
        this->onButtonPress(Aether::Button::X, [this](){
            this->setupOverlay();
        });
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::UserSelect);
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
        this->sortOverlay->addEntry("By Longest Playtime", [this](){
            this->list->setSort(SortType::HoursAsc);
        }, t == SortType::HoursAsc);
        this->sortOverlay->addEntry("By Shortest Playtime", [this](){
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
        this->heading->setColour(Aether::Theme::Dark.text);
        this->addElement(this->heading);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize());
        this->image->setWH(60, 60);
        this->addElement(this->image);

        // Populate list + count total time
        std::vector<Title *> t = this->app->titleVector();
        unsigned int totalSecs = 0;
        for (size_t i = 0; i < t.size(); i++) {
            PlayStatistics * ps = this->app->playdata()->getStatisticsForUser(t[i]->titleID(), this->app->activeUser()->ID());
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
            la->setImage(new Aether::Image(0, 0, t[i]->imgPtr(), t[i]->imgSize()));
            la->setTitle(t[i]->name());
            std::string str = "Played for " + TimeH::playtimeToString(ps->playtime, " and ");
            la->setPlaytime(str);
            str = TimeH::lastPlayedTimestampToString(pdmPlayTimestampToPosix(ps->lastPlayed));
            la->setLeftMuted(str);
            str = "Played " + std::to_string(ps->launches);
            (ps->launches == 1) ? str += " time" : str += " times";
            la->setRightMuted(str);
            la->setCallback([this, i](){
                this->app->setActiveTitle(i);
                this->app->setScreen(Main::ScreenID::Details);
            });
            la->setTitleColour(Aether::Theme::Dark.text);
            la->setPlaytimeColour(Aether::Theme::Dark.accent);
            la->setMutedColour(Aether::Theme::Dark.mutedText);
            la->setLineColour(Aether::Theme::Dark.mutedLine);
            this->list->addElement(la, si);
        }

        // Sort the list
        this->list->setSort(SortType::HoursAsc);
        this->setFocussed(this->list);

        // Render total hours string
        std::string txt = "Total Playtime: " + TimeH::playtimeToString(totalSecs, " and ");
        this->hours = new Aether::Text(1215, 44, txt, 20);
        this->hours->setXY(this->hours->x() - this->hours->w(), this->hours->y() - this->hours->h()/2);
        this->hours->setColour(Aether::Theme::Dark.mutedText);
        this->addElement(this->hours);
    }

    void AllActivity::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->hours);
        this->removeElement(this->image);
        this->list->removeAllElements();
    }

    AllActivity::~AllActivity() {
        delete this->sortOverlay;
    }
};