#include "Details.hpp"
#include "ListSession.hpp"
#include "Utils.hpp"

// Values for summary appearance
#define SUMMARY_BOX_HEIGHT 60
#define SUMMARY_FONT_SIZE 21

namespace Screen {
    Details::Details(Main::Application * a) {
        this->app = a;

        // Create static elements
        Aether::Rectangle * r = new Aether::Rectangle(30, 88, 600, 559);
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
        c->addItem(new Aether::ControlItem(Aether::Button::B, "Back"));
        c->addItem(new Aether::ControlItem(Aether::Button::X, "View Date"));
        c->addItem(new Aether::ControlItem(Aether::Button::Y, "View By"));
        c->setColour(this->app->theme()->text());
        this->addElement(c);
        this->graph = nullptr;
        this->graphHeading = new Aether::Text(0, 120, "", 22);
        this->graphHeading->setColour(this->app->theme()->text());
        this->addElement(this->graphHeading);
        this->graphSubheading = new Aether::Text(940, 150, "", 16);
        this->graphSubheading->setColour(this->app->theme()->mutedText());
        this->graphSubheading->setX(this->graphSubheading->x() - this->graphSubheading->w()/2);
        this->addElement(this->graphSubheading);
        this->graphTotal = new Aether::Text(0, 550, "Total Play Time", 20);
        this->graphTotal->setColour(this->app->theme()->text());
        this->addElement(this->graphTotal);
        this->graphTotalSub = new Aether::Text(0, 580, "", 18);
        this->graphTotalSub->setColour(this->app->theme()->accent());
        this->addElement(this->graphTotalSub);
        this->graphPercentage = new Aether::Text(0, 550, "Percentage of Total", 20);
        this->graphPercentage->setColour(this->app->theme()->text());
        this->addElement(this->graphPercentage);
        this->graphPercentageSub = new Aether::Text(0, 580, "", 18);
        this->graphPercentageSub->setColour(this->app->theme()->accent());
        this->addElement(this->graphPercentageSub);

        // Add L/R button elements
        Aether::Element * e = new Aether::Element(650, 120, 80, 50);
        Aether::Text * t = new Aether::Text(e->x() + 10, e->y(), "\uE0E4", 20, Aether::FontType::Extended); // L
        t->setY(e->y() + (e->h() - t->h())/2);
        t->setColour(this->app->theme()->mutedText());
        e->addElement(t);
        t = new Aether::Text(e->x(), e->y(), "\uE149", 26, Aether::FontType::Extended); // <
        t->setXY(e->x() + e->w() - t->w() - 10, e->y() + (e->h() - t->h())/2);
        t->setColour(this->app->theme()->text());
        e->addElement(t);
        e->setCallback([this](){
            this->app->decreaseDate();
        });
        this->addElement(e);

        e = new Aether::Element(1150, 120, 80, 50);
        t = new Aether::Text(e->x() + 10, e->y(), "\uE14A", 26, Aether::FontType::Extended); // >
        t->setY(e->y() + (e->h() - t->h())/2);
        t->setColour(this->app->theme()->text());
        e->addElement(t);
        t = new Aether::Text(e->x(), e->y(), "\uE0E5", 20, Aether::FontType::Extended); // R
        t->setXY(e->x() + e->w() - t->w() - 10, e->y() + (e->h() - t->h())/2);
        t->setColour(this->app->theme()->mutedText());
        e->addElement(t);
        e->setCallback([this](){
            this->app->increaseDate();
        });
        this->addElement(e);

        // Add key callbacks
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::AllActivity);
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

    void Details::update(uint32_t dt) {
        if (this->app->timeChanged()) {
            this->createGraph();
        }
        Screen::update(dt);
    }

    void Details::createGraph() {
        // Setup graph columns + labels
        std::string heading = "Activity for ";
        struct tm tm = this->app->time();
        switch (this->app->viewPeriod()) {
            case ViewPeriod::Day:
                heading += Utils::Time::tmToString(tm, true, true, !(tm.tm_year == Utils::Time::getTmForCurrentTime().tm_year));
                this->graph->setFontSize(12);
                this->graph->setMaximumValue(60);
                this->graph->setYSteps(6);
                this->graph->setValuePrecision(0);
                this->graph->setNumberOfEntries(24);
                this->graph->setLabel(0, "12am");
                this->graph->setLabel(3, "3am");
                this->graph->setLabel(6, "6am");
                this->graph->setLabel(9, "9am");
                this->graph->setLabel(12, "12pm");
                this->graph->setLabel(15, "3pm");
                this->graph->setLabel(18, "6pm");
                this->graph->setLabel(21, "9pm");
                break;

            case ViewPeriod::Month: {
                heading += Utils::Time::tmToString(tm, false, true, true);
                unsigned int c = Utils::Time::tmGetDaysInMonth(tm);
                this->graph->setFontSize(12);
                this->graph->setValuePrecision(1);
                this->graph->setNumberOfEntries(c);
                for (unsigned int i = 0; i < c; i+=3) {
                    this->graph->setLabel(i, std::to_string(i + 1) + Utils::Time::getDateSuffix(i + 1));
                }
                break;
            }

            case ViewPeriod::Year:
                heading += Utils::Time::tmToString(tm, false, false, true);
                this->graph->setFontSize(14);
                this->graph->setValuePrecision(1);
                this->graph->setNumberOfEntries(12);
                for (int i = 0; i < 12; i++) {
                    this->graph->setLabel(i, Utils::Time::getShortMonthString(i));
                }
                break;
        }

        // Read play time and set graph values
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
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
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
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
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
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
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
        }

        // Set headings etc...
        this->graphHeading->setString(heading);
        this->graphHeading->setX(940 - this->graphHeading->w()/2);
        switch (this->app->viewPeriod()) {
            case ViewPeriod::Day:
                this->graphSubheading->setString("Play Time (in minutes)");
                break;

            case ViewPeriod::Month:
            case ViewPeriod::Year:
                this->graphSubheading->setString("Play Time (in hours)");
                break;
        }
        this->graphSubheading->setX(940- this->graphSubheading->w()/2);
        this->graphTotal->setX(800 - this->graphTotal->w()/2);
        if (totalSecs == 0) {
            this->graphTotalSub->setString("0 seconds");
        } else {
            this->graphTotalSub->setString(Utils::Time::playtimeToString(totalSecs, ", "));
        }
        this->graphTotalSub->setX(800 - this->graphTotalSub->w()/2);
        this->graphPercentage->setX(1080 - this->graphPercentage->w()/2);
        NX::PlayStatistics * ps = this->app->playdata()->getStatisticsForUser(this->app->activeTitle()->titleID(), this->app->activeUser()->ID());
        double percent = 0.0;
        if (ps->playtime != 0) {
            percent = 100 * (totalSecs / (double)ps->playtime);
        }
        delete ps;
        this->graphPercentageSub->setString(Utils::truncateToDecimalPlace(std::to_string(Utils::roundToDecimalPlace(percent, 2)), 2) + "%");
        this->graphPercentageSub->setX(1080 - this->graphPercentageSub->w()/2);
    }

    void Details::setupSessionHelp() {
        this->msgbox->emptyBody();
        this->msgbox->close(false);

        int bw, bh;
        this->msgbox->getBodySize(&bw, &bh);
        Aether::Element * body = new Aether::Element(0, 0, bw, bh);
        Aether::TextBlock * tb = new Aether::TextBlock(50, 40, "A Play Session represents the time between when a game was launched to when it was quit in succession.", 22, bw - 100);
        tb->setColour(this->app->theme()->text());
        body->addElement(tb);
        tb = new Aether::TextBlock(50, tb->y() + tb->h() + 20, "The time of launch, the session's play time and percentage of overall playtime is shown.\n\nSelect a session to view a more detailed breakdown.", 22, bw - 100);
        tb->setColour(this->app->theme()->mutedText());
        body->addElement(tb);
        body->setWH(bw, tb->y() + tb->h() + 40);
        this->msgbox->setBodySize(body->w(), body->h());
        this->msgbox->setBody(body);

        this->app->addOverlay(this->msgbox);
    }

    void Details::setupSessionBreakdown(NX::PlaySession s) {
        this->msgbox->emptyBody();
        this->msgbox->close(false);

        // Create headings
        int bw, bh;
        this->msgbox->getBodySize(&bw, &bh);
        Aether::Element * body = new Aether::Element(0, 0, bw, bh);
        Aether::Text * t = new Aether::Text(50, 40, "Play Session Details", 26);
        t->setColour(this->app->theme()->text());
        body->addElement(t);
        t = new Aether::Text(50, t->y() + t->h() + 10, "Time spent in-game: " + Utils::Time::playtimeToString(s.playtime, " and "), 22);
        t->setColour(this->app->theme()->accent());
        body->addElement(t);
        t = new Aether::Text(50, t->y() + t->h() + 10, "Session length: " + Utils::Time::playtimeToString(s.endTimestamp - s.startTimestamp, " and "), 20);
        t->setColour(this->app->theme()->mutedText());
        body->addElement(t);

        std::vector<NX::PlayEvent> events = this->app->playdata()->getPlayEvents(s.startTimestamp, s.endTimestamp, this->app->activeTitle()->titleID(), this->app->activeUser()->ID());
        struct tm lastTime = Utils::Time::getTm(0);
        int Y = t->y() + t->h() + 25;
        time_t lastTs = 0;
        for (size_t i = 0; i < events.size(); i++) {
            // Print time of event in 12 hour format
            struct tm time = Utils::Time::getTm(events[i].clockTimestamp);
            bool isAM = ((time.tm_hour < 12) ? true : false);
            if (time.tm_hour == 0) {
                time.tm_hour = 12;
            }
            std::string str = std::to_string(((time.tm_hour > 12) ? time.tm_hour - 12 : time.tm_hour)) + ":" + ((time.tm_min < 10) ? "0" : "") + std::to_string(time.tm_min) + ((isAM) ? "am" : "pm") + " - ";

            // Add date string if new day
            if (Utils::Time::areDifferentDates(lastTime, time)) {
                struct tm now = Utils::Time::getTmForCurrentTime();
                t = new Aether::Text(50, Y, Utils::Time::tmToString(time, true, true, (time.tm_year != now.tm_year)), 18);
                t->setColour(this->app->theme()->text());
                body->addElement(t);
                Y += t->h() + 10;
            }
            lastTime = time;

            // Concatenate event type onto time string
            bool addPlaytime = false;
            switch (events[i].eventType) {
                case NX::EventType::Applet_Launch:
                    str += "Application Launched";
                    lastTs = events[i].steadyTimestamp;
                    break;

                case NX::EventType::Applet_InFocus:
                    if (events[i-1].eventType == NX::EventType::Account_Active || events[i-1].eventType == NX::EventType::Applet_Launch) {
                        continue;
                    }
                    str += "Application Resumed";
                    lastTs = events[i].steadyTimestamp;
                    break;

                case NX::EventType::Applet_OutFocus:
                    if (events[i+1].eventType == NX::EventType::Account_Inactive || events[i+1].eventType == NX::EventType::Applet_Exit) {
                        continue;
                    }
                    str += "Application Suspended";
                    addPlaytime = true;
                    break;

                case NX::EventType::Applet_Exit:
                    str += "Application Closed";
                    addPlaytime = true;
                    break;

                default:
                    continue;
                    break;
            }

            // Create + add string element
            t = new Aether::Text(50, Y, str, 18);
            t->setColour(this->app->theme()->mutedText());
            body->addElement(t);
            Y += t->h() + 10;

            // Add playtime from last "burst"
            if (addPlaytime) {
                t = new Aether::Text(t->x() + t->w() + 10, t->y() + t->h()/2, "(" + Utils::Time::playtimeToString(events[i].steadyTimestamp - lastTs, " and ") + ")", 16);
                t->setY(t->y() - t->h()/2);
                t->setColour(this->app->theme()->accent());
                body->addElement(t);
            }
        }

        // Set body element and show message box
        body->setWH(bw, t->y() + t->h() + 40);
        this->msgbox->setBodySize(body->w(), body->h());
        this->msgbox->setBody(body);
        this->app->addOverlay(this->msgbox);
    }

    void Details::onLoad() {
        // Render user's image
        this->userimage = new Aether::Image(1155, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize(), 4, 4);
        this->userimage->setWH(60, 60);
        this->addElement(this->userimage);

        // Render user's name
        this->username = new Aether::Text(1135, 45, this->app->activeUser()->username(), 20);
        this->username->setXY(this->username->x() - this->username->w(), this->username->y() - this->username->h()/2);
        this->username->setColour(this->app->theme()->mutedText());
        this->addElement(this->username);

        // Render title icon
        this->icon = new Aether::Image(65, 15, this->app->activeTitle()->imgPtr(), this->app->activeTitle()->imgSize(), 4, 4);
        this->icon->setWH(60, 60);
        this->addElement(this->icon);

        // Render title name and limit length
        this->title = new Aether::Text(145, 45, this->app->activeTitle()->name(), 28);
        this->title->setY(this->title->y() - this->title->h()/2);
        this->title->setColour(this->app->theme()->text());
        if (this->title->w() > this->username->x() - this->title->x() - 60) {
            this->title->setW(this->username->x() - this->title->x() - 60);
            this->title->setScroll(true);
        }
        this->addElement(this->title);

        // Create list and items
        NX::PlayStatistics * ps = this->app->playdata()->getStatisticsForUser(this->app->activeTitle()->titleID(), this->app->activeUser()->ID());
        this->list = new Aether::List(40, 88, 580, 559);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());
        Aether::ListHeading * h = new Aether::ListHeading("Summary");
        h->setRectColour(this->app->theme()->mutedLine());
        h->setTextColour(this->app->theme()->text());
        this->list->addElement(h);
        this->list->addElement(new Aether::ListSeparator(20));
        Aether::ListOption * lo = new Aether::ListOption("Play Time", Utils::Time::playtimeToString(ps->playtime, ", "), nullptr);
        lo->setH(SUMMARY_BOX_HEIGHT);
        lo->setFontSize(SUMMARY_FONT_SIZE);
        lo->setSelectable(false);
        lo->setTouchable(false);
        lo->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(lo);
        lo = new Aether::ListOption("Avg. Play Time", Utils::Time::playtimeToString(ps->playtime / ps->launches, ", "), nullptr);
        lo->setH(SUMMARY_BOX_HEIGHT);
        lo->setFontSize(SUMMARY_FONT_SIZE);
        lo->setSelectable(false);
        lo->setTouchable(false);
        lo->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(lo);
        lo = new Aether::ListOption("Times Played", Utils::formatNumberComma(ps->launches) + " times", nullptr);
        lo->setH(SUMMARY_BOX_HEIGHT);
        lo->setFontSize(SUMMARY_FONT_SIZE);
        lo->setSelectable(false);
        lo->setTouchable(false);
        lo->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(lo);
        lo = new Aether::ListOption("First Played", Utils::Time::timestampToString(pdmPlayTimestampToPosix(ps->firstPlayed)), nullptr);
        lo->setH(SUMMARY_BOX_HEIGHT);
        lo->setFontSize(SUMMARY_FONT_SIZE);
        lo->setSelectable(false);
        lo->setTouchable(false);
        lo->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(lo);
        lo = new Aether::ListOption("Last Played", Utils::Time::timestampToString(pdmPlayTimestampToPosix(ps->lastPlayed)), nullptr);
        lo->setH(SUMMARY_BOX_HEIGHT);
        lo->setFontSize(SUMMARY_FONT_SIZE);
        lo->setSelectable(false);
        lo->setTouchable(false);
        lo->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(lo);
        this->list->addElement(new Aether::ListSeparator());

        Aether::ListHeadingHelp * lhh = new Aether::ListHeadingHelp("Play Sessions", [this](){
            this->setupSessionHelp();
        });
        lhh->setHelpColour(this->app->theme()->mutedText());
        lhh->setRectColour(this->app->theme()->mutedLine());
        lhh->setTextColour(this->app->theme()->text());
        this->list->addElement(lhh);
        this->list->addElement(new Aether::ListSeparator(20));

        // Add play sessions to list
        std::vector<NX::PlaySession> stats = this->app->playdata()->getPlaySessionsForUser(this->app->activeTitle()->titleID(), this->app->activeUser()->ID());
        std::reverse(stats.begin(), stats.end());
        for (size_t i = 0; i < stats.size(); i++) {
            CustomElm::ListSession * ls = new CustomElm::ListSession();
            ls->setLineColour(this->app->theme()->mutedLine());
            ls->setPercentageColour(this->app->theme()->mutedText());
            ls->setPlaytimeColour(this->app->theme()->accent());
            ls->setTimeColour(this->app->theme()->text());
            ls->setPlaytimeString(Utils::Time::playtimeToString(stats[i].playtime, ", "));

            // Determine time range
            struct tm sTm = Utils::Time::getTm(stats[i].startTimestamp);
            struct tm nTm = Utils::Time::getTmForCurrentTime();
            bool isAM = ((sTm.tm_hour < 12) ? true : false);
            if (sTm.tm_hour == 0) {
                sTm.tm_hour = 12;
            }
            std::string str = Utils::Time::tmToString(sTm, true, true, !(sTm.tm_year == nTm.tm_year)) + " - " + std::to_string(((isAM) ? sTm.tm_hour : sTm.tm_hour - 12)) + ":" \
                + ((sTm.tm_min < 10) ? "0" : "" ) + std::to_string(sTm.tm_min) + ((isAM) ? "am" : "pm");
            ls->setTimeString(str);

            double percent = 100 * ((double)stats[i].playtime / ((ps->playtime == 0) ? stats[i].playtime : ps->playtime));
            percent = roundf(percent * 100) / 100;
            if (percent < 0.01) {
                str = "< 0.01%";
            } else {
                str = Utils::truncateToDecimalPlace(std::to_string(percent), 2) + "%";
            }
            ls->setPercentageString(str);
            NX::PlaySession ses = stats[i];
            ls->setCallback([this, ses](){
                this->setupSessionBreakdown(ses);
            });
            this->list->addElement(ls);
        }

        delete ps;
        this->addElement(this->list);
        this->setFocussed(this->list);

        // Create blank messagebox
        this->msgbox = new Aether::MessageBox();
        this->msgbox->addTopButton("Close", [this](){
            this->msgbox->close(true);
        });
        this->msgbox->setLineColour(this->app->theme()->mutedLine());
        this->msgbox->setRectangleColour(this->app->theme()->altBG());
        this->msgbox->setTextColour(this->app->theme()->accent());

        // Create graph
        this->graph = new CustomElm::Graph(660, 190, 560, 320, 2);
        this->graph->setBarColour(this->app->theme()->accent());
        this->graph->setLabelColour(this->app->theme()->text());
        this->graph->setLineColour(this->app->theme()->mutedLine());
        this->addElement(this->graph);
        this->createGraph();
    }

    void Details::onUnload() {
        this->removeElement(this->graph);
        this->removeElement(this->icon);
        this->removeElement(this->list);
        this->removeElement(this->title);
        this->removeElement(this->userimage);
        this->removeElement(this->username);
        delete this->msgbox;
    }
};