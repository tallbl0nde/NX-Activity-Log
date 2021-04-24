#include "Application.hpp"
#include "utils/Lang.hpp"
#include "ui/screen/RecentActivity.hpp"
#include "utils/Utils.hpp"

namespace Screen {
    RecentActivity::RecentActivity(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r;
        if (!this->app->config()->tImage() || this->app->config()->gTheme() != ThemeType::Custom) {
            r = new Aether::Rectangle(400, 88, 850, 559);
            r->setColour(this->app->theme()->altBG());
            this->addElement(r);
        }
        r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::ControlBar * c = new Aether::ControlBar();
        c->addControl(Aether::Button::A, "common.buttonHint.ok"_lang);
        c->addControl(Aether::Button::B, "common.buttonHint.back"_lang);
        c->addControl(Aether::Button::X, "common.buttonHint.date"_lang);
        c->addControl(Aether::Button::Y, "common.buttonHint.view"_lang);
        c->setDisabledColour(this->app->theme()->text());
        c->setEnabledColour(this->app->theme()->text());
        this->addElement(c);
        this->noStats = new Aether::Text(825, 350, "common.noActivity"_lang, 20);
        this->noStats->setXY(this->noStats->x() - this->noStats->w()/2, this->noStats->y() - this->noStats->h()/2);
        this->noStats->setColour(this->app->theme()->mutedText());
        this->addElement(this->noStats);

        this->onButtonPress(Aether::Button::B, [this](){
            if (this->app->isUserPage()) {
                this->app->exit();
            } else {
                this->app->setScreen(ScreenID::UserSelect);
            }
        });
        this->onButtonPress(Aether::Button::X, [this](){
            this->app->createDatePicker();
        });
        this->onButtonPress(Aether::Button::Y, [this](){
            this->app->createPeriodPicker();
        });
        this->onButtonPress(Aether::Button::R, [this](){
            this->app->increaseDate();
        });
        this->onButtonPress(Aether::Button::L, [this](){
            this->app->decreaseDate();
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

    void RecentActivity::updateActivity() {
        // Check if there is any activity + update heading
        struct tm t = this->app->time();
        t.tm_min = 0;
        t.tm_sec = 0;
        struct tm e = t;
        e.tm_min = 59;
        e.tm_sec = 59;
        switch (this->app->viewPeriod()) {
            case ViewPeriod::Day:
                e.tm_hour = 23;
                break;

            case ViewPeriod::Month:
                e.tm_mday = Utils::Time::tmGetDaysInMonth(t);
                break;

            case ViewPeriod::Year:
                e.tm_mon = 11;
                e.tm_mday = Utils::Time::tmGetDaysInMonth(t);
                break;

            default:
                break;
        }
        this->graphHeading->setString(Utils::Time::dateToActivityForString(t, this->app->viewPeriod()));
        this->graphHeading->setX(this->header->x() + (this->header->w() - this->graphHeading->w())/2);
        NX::RecentPlayStatistics * ps = this->app->playdata()->getRecentStatisticsForUser(Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());

        // Remove current sessions regardless
        this->list->removeElementsAfter(this->topElm);
        if (this->focussed() == this->list) {
            this->list->setFocussed(this->header);
        }

        // Only update list if there is activity
        if (ps->playtime != 0) {
            this->gameHeading->setHidden(false);
            this->graph->setHidden(false);
            this->graphSubheading->setHidden(false);
            this->list->setShowScrollBar(true);
            this->list->setCanScroll(true);
            this->noStats->setHidden(true);
            this->updateGraph();
            this->updateTitles();
        } else {
            this->gameHeading->setHidden(true);
            this->graph->setHidden(true);
            this->graphSubheading->setHidden(true);
            this->hours->setString("common.totalPlaytime.0min"_lang);
            this->hours->setX(1215 - this->hours->w());
            this->list->setShowScrollBar(false);
            this->list->setCanScroll(false);
            this->noStats->setHidden(false);
        }

        delete ps;
    }

    void RecentActivity::update(uint32_t dt) {
        if (this->app->timeChanged()) {
            this->updateActivity();
        }
        Screen::update(dt);
    }

    void RecentActivity::updateGraph() {
        // Setup graph columns + labels
        for (unsigned int i = 0; i < this->graph->entries(); i++) {
            this->graph->setLabel(i, "");
        }
        struct tm tm = this->app->time();
        switch (this->app->viewPeriod()) {
            case ViewPeriod::Day:
                this->graph->setFontSize(14);
                this->graph->setMaximumValue(60);
                this->graph->setYSteps(6);
                this->graph->setValuePrecision(0);
                this->graph->setNumberOfEntries(24);
                if (this->app->config()->gIs24H()) {
                    for (int i = 0; i < 24; i += 2) {
                        this->graph->setLabel(i, std::to_string(i));
                    }
                } else {
                    for (int i = 0; i < 24; i += 3) {
                        this->graph->setLabel(i, Utils::format12H(i));
                    }
                }
                break;

            case ViewPeriod::Month: {
                unsigned int c = Utils::Time::tmGetDaysInMonth(tm);
                this->graph->setFontSize(13);
                this->graph->setValuePrecision(1);
                this->graph->setNumberOfEntries(c);
                for (unsigned int i = 0; i < c; i+=3) {
                    this->graph->setLabel(i, std::to_string(i + 1) + Utils::Time::getDateSuffix(i + 1));
                }
                break;
            }

            case ViewPeriod::Year:
                this->graph->setFontSize(16);
                this->graph->setValuePrecision(1);
                this->graph->setNumberOfEntries(12);
                for (int i = 0; i < 12; i++) {
                    this->graph->setLabel(i, Utils::Time::getShortMonthString(i));
                }
                break;

            default:
                break;
        }

        // Read playtime and set graph values
        struct tm t = tm;
        unsigned int totalSecs = 0;
        switch (this->app->viewPeriod()) {
            case ViewPeriod::Day: {
                t.tm_min = 0;
                t.tm_sec = 0;
                struct tm e = t;
                e.tm_min = 59;
                e.tm_sec = 59;
                for (size_t i = 0; i < this->graph->entries(); i++) {
                    t.tm_hour = i;
                    e.tm_hour = i;
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForUser(Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
                    totalSecs += s->playtime;
                    double val = s->playtime/60.0;
                    this->graph->setValue(i, val);
                    delete s;
                }
                break;
            }

            case ViewPeriod::Month: {
                t.tm_hour = 0;
                t.tm_min = 0;
                t.tm_sec = 0;
                struct tm e = t;
                e.tm_hour = 23;
                e.tm_min = 59;
                e.tm_sec = 59;
                unsigned int max = 0;
                for (size_t i = 0; i < this->graph->entries(); i++) {
                    t.tm_mday = i + 1;
                    e.tm_mday = i + 1;
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForUser(Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
                    totalSecs += s->playtime;
                    if (s->playtime > max) {
                        max = s->playtime;
                    }
                    double val = s->playtime/60/60.0;
                    this->graph->setValue(i, val);
                    delete s;
                }
                max /= 60.0;
                max /= 60.0;
                if (max <= 2) {
                    this->graph->setMaximumValue(max + 2 - max%2);
                    this->graph->setYSteps(2);
                } else {
                    this->graph->setMaximumValue(max + 5 - max%5);
                    this->graph->setYSteps(5);
                }
                break;
            }

            case ViewPeriod::Year: {
                t.tm_mday = 1;
                t.tm_hour = 0;
                t.tm_min = 0;
                t.tm_sec = 0;
                struct tm e = t;
                e.tm_hour = 23;
                e.tm_min = 59;
                e.tm_sec = 59;
                unsigned int max = 0;
                for (size_t i = 0; i < this->graph->entries(); i++) {
                    t.tm_mon = i;
                    e.tm_mon = i;
                    e.tm_mday = Utils::Time::tmGetDaysInMonth(t);
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForUser(Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
                    totalSecs += s->playtime;
                    if (s->playtime > max) {
                        max = s->playtime;
                    }
                    double val = s->playtime/60/60.0;
                    this->graph->setValue(i, val);
                    delete s;
                }
                max /= 60.0;
                max /= 60.0;
                if (max <= 2) {
                    this->graph->setMaximumValue(max + 2 - max%2);
                    this->graph->setYSteps(2);
                } else {
                    this->graph->setMaximumValue(max + 5 - max%5);
                    this->graph->setYSteps(5);
                }
                break;
            }

            default:
                break;
        }

        // Set headings
        if (this->app->viewPeriod() == ViewPeriod::Day) {
            this->graphSubheading->setString("common.playtimeMinutes"_lang);
        } else {
            this->graphSubheading->setString("common.playtimeHours"_lang);
        }
        this->graphSubheading->setX(this->header->x() + (this->header->w() - this->graphSubheading->w())/2);
    }

    void RecentActivity::updateTitles() {
        // Get start and end timestamps for date
        char c = ' ';
        switch (this->app->viewPeriod()) {
            case ViewPeriod::Day:
                c = 'D';
                break;

            case ViewPeriod::Month:
                c = 'M';
                break;

            case ViewPeriod::Year:
                c = 'Y';
                break;

            default:
                break;
        }
        unsigned int s = Utils::Time::getTimeT(this->app->time());
        // Minus one second so end time is 11:59pm and not 12:00am next day
        unsigned int e = Utils::Time::getTimeT(Utils::Time::increaseTm(this->app->time(), c)) - 1;

        // Get stats
        unsigned int totalSecs = 0;
        std::vector<std::pair<NX::RecentPlayStatistics *, unsigned int> > stats;
        for (size_t i = 0; i < this->app->titleVector().size(); i++) {
            std::pair<NX::RecentPlayStatistics *, unsigned int> stat;
            stat.first = this->app->playdata()->getRecentStatisticsForTitleAndUser(this->app->titleVector()[i]->titleID(), s, e, this->app->activeUser()->ID());
            stat.second = i;
            stats.push_back(stat);
        }

        // Sort to have most played first
        std::sort(stats.begin(), stats.end(), [](const std::pair<NX::RecentPlayStatistics *, unsigned int> lhs, const std::pair<NX::RecentPlayStatistics *, unsigned int> rhs) {
            return lhs.first->playtime > rhs.first->playtime;
        });

        // Add to list
        for (size_t i = 0; i < stats.size(); i++) {
            // Only show games that have actually been played
            if (stats[i].first->launches > 0) {
                totalSecs += stats[i].first->playtime;
                CustomElm::ListActivity * la = new CustomElm::ListActivity();
                la->setImage(this->app->titleVector()[stats[i].second]->imgPtr(), this->app->titleVector()[stats[i].second]->imgSize());
                la->setTitle(this->app->titleVector()[stats[i].second]->name());
                la->setPlaytime(Utils::playtimeToPlayedForString(stats[i].first->playtime));
                la->setLeftMuted(Utils::launchesToPlayedString(stats[i].first->launches));
                unsigned int j = stats[i].second;
                la->onPress([this, j](){
                    this->app->setActiveTitle(j);
                    this->app->pushScreen();
                    this->app->setScreen(ScreenID::Details);
                });
                la->setTitleColour(this->app->theme()->text());
                la->setPlaytimeColour(this->app->theme()->accent());
                la->setMutedColour(this->app->theme()->mutedText());
                la->setLineColour(this->app->theme()->mutedLine());
                this->list->addElement(la);
            }

            // Can delete each pointer after it's accessed
            delete stats[i].first;
        }

        // Update playtime string
        this->hours->setString(Utils::playtimeToTotalPlaytimeString(totalSecs));
        this->hours->setX(1215 - this->hours->w());
    }


    void RecentActivity::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, Utils::formatHeading(this->app->activeUser()->username()), 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(this->app->theme()->text());
        this->addElement(this->heading);

        // Render total hours string
        this->hours = new Aether::Text(1215, 44, "y", 20);
        this->hours->setY(this->hours->y() - this->hours->h()/2);
        this->hours->setColour(this->app->theme()->mutedText());
        this->addElement(this->hours);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize(), Aether::Render::Wait);
        this->image->setScaleDimensions(60, 60);
        this->image->renderSync();
        this->addElement(this->image);

        // Create side menu
        this->menu = new Aether::Menu(30, 88, 388, 559);
        Aether::MenuOption * opt = new Aether::MenuOption("common.screen.recentActivity"_lang, this->app->theme()->accent(), this->app->theme()->text(), nullptr);
        this->menu->addElement(opt);
        this->menu->setActiveOption(opt);
        this->menu->addElement(new Aether::MenuOption("common.screen.allActivity"_lang, this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(ScreenID::AllActivity);
        }));
        this->menu->addElement(new Aether::MenuSeparator(this->app->theme()->mutedLine()));
        this->menu->addElement(new Aether::MenuOption("common.screen.settings"_lang, this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(ScreenID::Settings);
        }));
        this->menu->setFocussed(opt);
        this->addElement(this->menu);

        // Create list
        this->list = new Aether::List(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // Add heading and L
        this->header = new Aether::Container(0, 0, 100, 70);
        Aether::Element * e = new Aether::Element(this->header->x(), this->header->y(), 80, 50);
        Aether::Text * t = new Aether::Text(e->x() + 10, e->y(), "\uE0E4", 20); // L
        t->setY(e->y() + (e->h() - t->h())/2);
        t->setColour(this->app->theme()->mutedText());
        e->addElement(t);
        t = new Aether::Text(e->x(), e->y(), "\uE149", 26); // <
        t->setXY(e->x() + e->w() - t->w() - 10, e->y() + (e->h() - t->h())/2);
        t->setColour(this->app->theme()->text());
        e->addElement(t);
        e->onPress([this](){
            this->app->decreaseDate();
        });
        this->header->addElement(e);

        // Do this here as it adjusts header's width to the list's width
        this->list->addElement(this->header);

        // R button
        e = new Aether::Element(this->header->x() + this->header->w() - 80, this->header->y(), 80, 50);
        t = new Aether::Text(e->x() + 10, e->y(), "\uE14A", 26); // >
        t->setY(e->y() + (e->h() - t->h())/2);
        t->setColour(this->app->theme()->text());
        e->addElement(t);
        t = new Aether::Text(e->x(), e->y(), "\uE0E5", 20); // R
        t->setXY(e->x() + e->w() - t->w() - 10, e->y() + (e->h() - t->h())/2);
        t->setColour(this->app->theme()->mutedText());
        e->addElement(t);
        e->onPress([this](){
            this->app->increaseDate();
        });
        this->header->addElement(e);

        // Add graph heading to container
        this->graphHeading = new Aether::Text(this->header->x(), this->header->y(), "", 22);
        this->graphHeading->setColour(this->app->theme()->text());
        this->header->addElement(this->graphHeading);
        this->graphSubheading = new Aether::Text(this->header->x(), this->graphHeading->y() + 30, "", 16);
        this->graphSubheading->setColour(this->app->theme()->mutedText());
        this->header->addElement(this->graphSubheading);

        // Setup graph
        this->graph = new CustomElm::Graph(0, 0, this->list->w(), 300, 1);
        this->graph->setBarColour(this->app->theme()->accent());
        this->graph->setLabelColour(this->app->theme()->text());
        this->graph->setLineColour(this->app->theme()->mutedLine());
        this->graph->setValueVisibility(this->app->config()->gGraph());
        this->list->addElement(this->graph);
        this->list->addElement(new Aether::ListSeparator(30));

        // Add games heading
        this->gameHeading = new Aether::ListHeading("recentActivity.titleHeading"_lang);
        this->gameHeading->setRectColour(this->app->theme()->mutedLine());
        this->gameHeading->setTextColour(this->app->theme()->text());
        this->list->addElement(this->gameHeading);

        // Keep pointer to this element to allow removal
        this->topElm = new Aether::ListSeparator(20);
        this->list->addElement(this->topElm);

        this->addElement(this->list);

        // Show update icon if needbe
        this->updateElm =nullptr;
        if (this->app->hasUpdate()) {
            this->updateElm = new Aether::Image(50, 669, "romfs:/icon/download.png");
            this->updateElm->setColour(this->app->theme()->text());
            this->addElement(this->updateElm);
        }

        // Get play sessions
        this->updateActivity();
    }

    void RecentActivity::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->hours);
        this->removeElement(this->image);
        this->removeElement(this->list);
        this->removeElement(this->menu);
        this->removeElement(this->updateElm);
    }

    void RecentActivity::onPush() {
        this->tmCopy = this->app->time();
    }

    void RecentActivity::onPop() {
        // If time was changed in details update this screen too!
        if (Utils::Time::areDifferentDates(this->tmCopy, this->app->time()) || this->viewCopy != this->app->viewPeriod()) {
            this->updateActivity();
        }
    }
};