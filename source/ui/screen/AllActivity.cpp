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
        this->list->setScrollBarColour(Aether::Theme::Dark.mutedLine);
        this->addElement(this->list);

        // Add callbacks for buttons
        this->onButtonPress(Aether::Button::X, [this](){
            // push overlay
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
        PlayData * pd = this->app->playdata();
        std::vector<Title *> t = this->app->titleVector();
        AccountUid u = this->app->activeUser()->ID();
        unsigned int totalSecs = 0;
        for (size_t i = 0; i < t.size(); i++) {
            PlayStatistics * ps = pd->getStatisticsForUser(t[i]->titleID(), u);
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
            la->setRank("#" + std::to_string(i));
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
};