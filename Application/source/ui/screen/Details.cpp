#include "Application.hpp"
#include "ui/screen/Details.hpp"
#include "utils/Lang.hpp"
#include "ui/element/ListSession.hpp"
#include "utils/Utils.hpp"

// Values for summary appearance
#define SUMMARY_BOX_HEIGHT 60
#define SUMMARY_FONT_SIZE 21

namespace Screen {
    Details::Details(Main::Application * a) {
        this->app = a;

        // Create static elements
        Aether::Rectangle * r;
        if (!this->app->config()->tImage() || this->app->config()->gTheme() != ThemeType::Custom) {
            r = new Aether::Rectangle(890, 88, 360, 559);
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
        this->noStats = new Aether::Text(460, 350, "common.noActivity"_lang, 20);
        this->noStats->setXY(this->noStats->x() - this->noStats->w()/2, this->noStats->y() - this->noStats->h()/2);
        this->noStats->setColour(this->app->theme()->mutedText());
        this->addElement(this->noStats);

        Aether::Text * t = new Aether::Text(1070, 120, "details.allTime"_lang, 24);
        t->setColour(this->app->theme()->text());
        t->setX(t->x() - t->w()/2);
        this->addElement(t);
        t = new Aether::Text(1070, 190, "details.playtime"_lang, 22);
        t->setColour(this->app->theme()->text());
        t->setX(t->x() - t->w()/2);
        this->addElement(t);
        t = new Aether::Text(1070, 280, "details.avgPlaytime"_lang, 22);
        t->setColour(this->app->theme()->text());
        t->setX(t->x() - t->w()/2);
        this->addElement(t);
        t = new Aether::Text(1070, 370, "details.timesPlayed.heading"_lang, 22);
        t->setColour(this->app->theme()->text());
        t->setX(t->x() - t->w()/2);
        this->addElement(t);
        t = new Aether::Text(1070, 460, "details.firstPlayed"_lang, 22);
        t->setColour(this->app->theme()->text());
        t->setX(t->x() - t->w()/2);
        this->addElement(t);
        t = new Aether::Text(1070, 550, "details.lastPlayed"_lang, 22);
        t->setColour(this->app->theme()->text());
        t->setX(t->x() - t->w()/2);
        this->addElement(t);

        // Add key callbacks
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->popScreen();
            this->popped = true;
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

    void Details::updateActivity() {
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
        NX::RecentPlayStatistics * ps = this->app->playdata()->getRecentStatisticsForTitleAndUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());

        // Remove current sessions regardless
        this->list->removeElementsAfter(this->topElm);
        this->list->setFocussed(this->header);

        // Only update list if there is activity
        if (ps->playtime != 0) {
            this->graph->setHidden(false);
            this->graphSubheading->setHidden(false);
            this->graphTotal->setHidden(false);
            this->list->setShowScrollBar(true);
            this->list->setCanScroll(true);
            this->playHeading->setHidden(false);
            this->noStats->setHidden(true);
            this->updateGraph();
            this->updateSessions();
        } else {
            this->graph->setHidden(true);
            this->graphSubheading->setHidden(true);
            this->graphTotal->setHidden(true);
            this->list->setShowScrollBar(false);
            this->list->setCanScroll(false);
            this->playHeading->setHidden(true);
            this->noStats->setHidden(false);
        }

        delete ps;
    }

    void Details::update(uint32_t dt) {
        // Don't check!!
        if (!this->popped) {
            if (this->app->timeChanged()) {
                this->updateActivity();
            }
        }

        Screen::update(dt);
    }

    void Details::updateGraph() {
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
        uint64_t totalSecs = 0;
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
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForTitleAndUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
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
                uint64_t max = 0;
                for (size_t i = 0; i < this->graph->entries(); i++) {
                    t.tm_mday = i + 1;
                    e.tm_mday = i + 1;
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForTitleAndUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
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
                uint64_t max = 0;
                for (size_t i = 0; i < this->graph->entries(); i++) {
                    t.tm_mon = i;
                    e.tm_mon = i;
                    e.tm_mday = Utils::Time::tmGetDaysInMonth(t);
                    NX::RecentPlayStatistics * s = this->app->playdata()->getRecentStatisticsForTitleAndUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
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

        // Set headings etc...
        switch (this->app->viewPeriod()) {
            case ViewPeriod::Day:
                this->graphSubheading->setString("common.playtimeMinutes"_lang);
                break;

            case ViewPeriod::Month:
            case ViewPeriod::Year:
                this->graphSubheading->setString("common.playtimeHours"_lang);
                break;

            default:
                break;
        }

        this->graphSubheading->setX(this->header->x() + (this->header->w() - this->graphSubheading->w())/2);
        this->graphTotalSub->setString(Utils::playtimeToString(totalSecs));
        int w = this->graphTotalHead->w() + this->graphTotalSub->w();
        this->graphTotalHead->setX(this->graphTotal->x());
        this->graphTotalSub->setX(this->graphTotalHead->x() + this->graphTotalHead->w());
        this->graphTotalHead->setX(this->graphTotalHead->x() + (this->graphTotal->w() - w)/2);
        this->graphTotalSub->setX(this->graphTotalSub->x() + (this->graphTotal->w() - w)/2);
    }

    void Details::updateSessions() {
        // Get relevant play stats
        NX::PlayStatistics * ps = this->app->playdata()->getStatisticsForUser(this->app->activeTitle()->titleID(), this->app->activeUser()->ID());
        std::vector<NX::PlaySession> stats = this->app->playdata()->getPlaySessionsForUser(this->app->activeTitle()->titleID(), this->app->activeUser()->ID());

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
        uint64_t s = Utils::Time::getTimeT(this->app->time());
        // Minus one second so end time is 11:59pm and not 12:00am next day
        uint64_t e = Utils::Time::getTimeT(Utils::Time::increaseTm(this->app->time(), c)) - 1;

        // Add sessions to list
        for (size_t i = 0; i < stats.size(); i++) {
            // Only add session if start or end is within the current time period
            if (stats[i].startTimestamp > e || stats[i].endTimestamp < s) {
                continue;
            }

            // Create element
            CustomElm::ListSession * ls = new CustomElm::ListSession();
            NX::PlaySession ses = stats[i];
            ls->onPress([this, ses](){
                this->setupSessionBreakdown(ses);
            });

            // Defaults for a session within the range
            uint64_t playtime = stats[i].playtime;
            struct tm sTm = Utils::Time::getTm(stats[i].startTimestamp);
            struct tm eTm = Utils::Time::getTm(stats[i].endTimestamp);

            // If started before range set start as start of range
            bool outRange = false;
            if (stats[i].startTimestamp < s) {
                outRange = true;
                sTm = Utils::Time::getTm(s);
            }

            // If finished after range set end as end of range
            if (stats[i].endTimestamp > e) {
                outRange = true;
                eTm = Utils::Time::getTm(e);
            }

            // Get playtime if range is altered
            if (outRange) {
                NX::RecentPlayStatistics * rps = this->app->playdata()->getRecentStatisticsForTitleAndUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(sTm), Utils::Time::getTimeT(eTm), this->app->activeUser()->ID());
                playtime = rps->playtime;
                delete rps;
            }

            // Set timestamp string
            std::string first = "";
            std::string last = "";
            struct tm nTm = Utils::Time::getTmForCurrentTime();
            switch (this->app->viewPeriod()) {
                case ViewPeriod::Year:
                case ViewPeriod::Month:
                    first = Utils::Time::tmToDate(sTm, sTm.tm_year != nTm.tm_year) + " ";
                    // Show date on end timestamp if not the same
                    if (sTm.tm_mday != eTm.tm_mday) {
                        last = Utils::Time::tmToDate(eTm, eTm.tm_year != nTm.tm_year) + " ";
                    }

                case ViewPeriod::Day:
                    if (this->app->config()->gIs24H()) {
                        first += Utils::Time::tmToString(sTm, "%R", 5);
                        last += Utils::Time::tmToString(eTm, "%R", 5);
                    } else {
                        std::string tmp = Utils::Time::tmToString(sTm, "%I:%M", 5);
                        if (tmp[0] == '0') {
                            tmp.erase(0, 1);
                        }
                        first += tmp + Utils::Time::getAMPM(sTm.tm_hour);

                        tmp = Utils::Time::tmToString(eTm, "%I:%M", 5);
                        if (tmp[0] == '0') {
                            tmp.erase(0, 1);
                        }
                        last += tmp + Utils::Time::getAMPM(eTm.tm_hour);
                    }
                    break;

                default:
                    break;
            }
            ls->setTimeString(first + " - " + last + (outRange ? "*" : ""));
            ls->setPlaytimeString(Utils::playtimeToString(playtime));

            // Add percentage of total playtime
            std::string str;
            double percent = 100 * ((double)playtime / ((ps->playtime == 0 || ps->playtime < playtime) ? playtime : ps->playtime));
            percent = Utils::roundToDecimalPlace(percent, 2);
            if (percent < 0.01) {
                str = "< 0.01%";
            } else {
                str = Utils::truncateToDecimalPlace(std::to_string(percent), 2) + "%";
            }
            ls->setPercentageString(str);

            ls->setLineColour(this->app->theme()->mutedLine());
            ls->setPercentageColour(this->app->theme()->mutedText());
            ls->setPlaytimeColour(this->app->theme()->accent());
            ls->setTimeColour(this->app->theme()->text());
            this->list->addElement(ls);
        }

        delete ps;
    }

    void Details::setupSessionHelp() {
        this->msgbox->emptyBody();

        int bw, bh;
        this->msgbox->getBodySize(&bw, &bh);
        Aether::Element * body = new Aether::Element(0, 0, bw, bh);
        Aether::TextBlock * tb = new Aether::TextBlock(50, 40, "details.popup.heading"_lang, 22, bw - 100);
        tb->setColour(this->app->theme()->text());
        body->addElement(tb);
        tb = new Aether::TextBlock(50, tb->y() + tb->h() + 20, "details.popup.msg1"_lang + "\n\n" + "details.popup.msg2"_lang + "\n\n" + "details.popup.msg3"_lang, 20, bw - 100);
        tb->setColour(this->app->theme()->mutedText());
        body->addElement(tb);
        body->setWH(bw, tb->y() + tb->h() + 40);
        this->msgbox->setBodySize(body->w(), body->h());
        this->msgbox->setBody(body);

        this->app->addOverlay(this->msgbox);
    }

    void Details::setupSessionBreakdown(NX::PlaySession s) {
        this->panel->setPlaytime(Utils::playtimeToString(s.playtime));
        this->panel->setLength(Utils::playtimeToString(s.endTimestamp - s.startTimestamp));

        // Container element to prevent stretching of text in list
        std::vector<NX::PlayEvent> events = this->app->playdata()->getPlayEvents(s.startTimestamp, s.endTimestamp, this->app->activeTitle()->titleID(), this->app->activeUser()->ID());
        struct tm lastTime = Utils::Time::getTm(0);
        time_t lastTs = 0;
        for (size_t i = 0; i < events.size(); i++) {
            struct tm time = Utils::Time::getTm(events[i].clockTimestamp);
            std::string str;
            if (this->app->config()->gIs24H()) {
                // Print time of event in 24 hour format
                str = Utils::Time::tmToString(time, "%R", 5) + " - ";

            } else {
                // Print time of event in 12 hour format
                bool isAM = ((time.tm_hour < 12) ? true : false);
                if (time.tm_hour == 0) {
                    time.tm_hour = 12;
                }
                str = std::to_string(((time.tm_hour > 12) ? time.tm_hour - 12 : time.tm_hour)) + ":" + ((time.tm_min < 10) ? "0" : "") + std::to_string(time.tm_min) + ((isAM) ? "common.am"_lang : "common.pm"_lang) + " - ";
            }

            // Add date string if new day
            if (Utils::Time::areDifferentDates(lastTime, time)) {
                struct tm now = Utils::Time::getTmForCurrentTime();
                Aether::Text * t = new Aether::Text(0, 10, Utils::Time::tmToDate(time, time.tm_year != now.tm_year), 18);
                t->setColour(this->app->theme()->text());
                Aether::Element * c = new Aether::Element(0, 0, 100, t->h() + 20);
                c->addElement(t);
                this->panel->addListItem(c);
            }
            lastTime = time;

            // Concatenate event type onto time string
            bool addPlaytime = false;
            switch (events[i].eventType) {
                case NX::EventType::Applet_Launch:
                    str += "details.break.appLaunched"_lang;
                    lastTs = events[i].steadyTimestamp;
                    break;

                case NX::EventType::Applet_InFocus:
                    if (events[i-1].eventType == NX::EventType::Account_Active || events[i-1].eventType == NX::EventType::Applet_Launch) {
                        continue;
                    }
                    str += "details.break.appResumed"_lang;
                    lastTs = events[i].steadyTimestamp;
                    break;

                case NX::EventType::Applet_OutFocus:
                    if (events[i+1].eventType == NX::EventType::Account_Inactive || events[i+1].eventType == NX::EventType::Applet_Exit) {
                        continue;
                    }
                    str += "details.break.appSuspended"_lang;
                    addPlaytime = true;
                    break;

                case NX::EventType::Applet_Exit:
                    str += "details.break.appClosed"_lang;
                    addPlaytime = true;
                    break;

                default:
                    continue;
                    break;
            }

            // Create + add string element
            Aether::Text * t = new Aether::Text(0, 0, str, 18);
            t->setColour(this->app->theme()->mutedText());
            Aether::Element * c = new Aether::Element(0, 0, 100, t->h() + 5);
            c->addElement(t);

            // Add playtime from last "burst"
            if (addPlaytime) {
                t = new Aether::Text(t->x() + t->w() + 10, t->y() + t->h()/2, "(" + Utils::playtimeToString(events[i].steadyTimestamp - lastTs) + ")", 16);
                t->setY(t->y() - t->h()/2);
                t->setColour(this->app->theme()->accent());
                c->addElement(t);
            }
            this->panel->addListItem(c);
        }

        this->app->addOverlay(this->panel);
    }

    void Details::onLoad() {
        this->popped = false;

        // Render user's image
        this->userimage = new Aether::Image(1155, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize(), Aether::Render::Wait);
        this->userimage->setScaleDimensions(60, 60);
        this->userimage->renderSync();
        this->addElement(this->userimage);

        // Render user's name
        this->username = new Aether::Text(1135, 45, this->app->activeUser()->username(), 20);
        this->username->setXY(this->username->x() - this->username->w(), this->username->y() - this->username->h()/2);
        this->username->setColour(this->app->theme()->mutedText());
        this->addElement(this->username);

        // Render title icon
        this->icon = new Aether::Image(65, 15, this->app->activeTitle()->imgPtr(), this->app->activeTitle()->imgSize(), Aether::Render::Wait);
        this->icon->setScaleDimensions(60, 60);
        this->icon->renderSync();
        this->addElement(this->icon);

        // Render title name and limit length
        this->title = new Aether::Text(145, 45, this->app->activeTitle()->name(), 28);
        this->title->setY(this->title->y() - this->title->h()/2);
        this->title->setColour(this->app->theme()->text());
        if (this->title->w() > this->username->x() - this->title->x() - 60) {
            this->title->setW(this->username->x() - this->title->x() - 60);
            this->title->setCanScroll(true);
        }
        this->addElement(this->title);

        // Create list (graph + play sessions)
        this->list = new Aether::List(40, 88, 840, 559);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());
        this->addElement(this->list);

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
        this->graph = new CustomElm::Graph(0, 0, this->list->w(), 350, 1);
        this->graph->setBarColour(this->app->theme()->accent());
        this->graph->setLabelColour(this->app->theme()->text());
        this->graph->setLineColour(this->app->theme()->mutedLine());
        this->graph->setValueVisibility(this->app->config()->gGraph());
        this->list->addElement(this->graph);
        this->list->addElement(new Aether::ListSeparator(30));

        this->graphTotal = new Aether::Element(0, 0, 100, 30);
        this->graphTotalHead = new Aether::Text(this->graphTotal->x(), this->graphTotal->y(), "common.totalPlaytime.heading"_lang + " ", 20);
        this->graphTotalHead->setColour(this->app->theme()->text());
        this->graphTotal->addElement(this->graphTotalHead);
        this->graphTotalSub = new Aether::Text(this->graphTotalHead->x() + this->graphTotalHead->w(), this->graphTotal->y(), "", 20);
        this->graphTotalSub->setColour(this->app->theme()->accent());
        this->graphTotal->addElement(this->graphTotalSub);
        this->list->addElement(this->graphTotal);
        this->list->addElement(new Aether::ListSeparator(20));

        // Add play sessions heading
        this->playHeading = new Aether::ListHeadingHelp("details.playSessions"_lang, [this](){
            this->setupSessionHelp();
        });
        this->playHeading->setHelpColour(this->app->theme()->mutedText());
        this->playHeading->setRectColour(this->app->theme()->mutedLine());
        this->playHeading->setTextColour(this->app->theme()->text());
        this->list->addElement(this->playHeading);

        // Keep pointer to this element to allow removal
        this->topElm = new Aether::ListSeparator(20);
        this->list->addElement(this->topElm);

        // Get play sessions
        this->updateActivity();
        this->setFocussed(this->list);

        // Get statistics and append adjustment if needed
        std::vector<AdjustmentValue> adjustments = this->app->config()->adjustmentValues();
        NX::PlayStatistics * ps = this->app->playdata()->getStatisticsForUser(this->app->activeTitle()->titleID(), this->app->activeUser()->ID());
        std::vector<AdjustmentValue>::iterator it = std::find_if(adjustments.begin(), adjustments.end(), [this](AdjustmentValue val) {
            return (val.titleID == this->app->activeTitle()->titleID() && val.userID == this->app->activeUser()->ID());
        });
        if (it != adjustments.end()) {
            ps->playtime += (*it).value;
        }

        if (ps->launches == 0) {
            // Add in dummy data if not launched before (due to adjustment)
            ps->firstPlayed = Utils::Time::posixTimestampToPdm(Utils::Time::getTimeT(Utils::Time::getTmForCurrentTime()));
            ps->lastPlayed = ps->firstPlayed;
            ps->launches = 1;
        }

        this->playtime = new Aether::Text(1070, 220, Utils::playtimeToString(ps->playtime), 20);
        this->playtime->setColour(this->app->theme()->accent());
        this->playtime->setX(this->playtime->x() - this->playtime->w()/2);
        this->addElement(this->playtime);

        this->avgplaytime = new Aether::Text(1070, 310, Utils::playtimeToString(ps->playtime / ps->launches), 20);
        this->avgplaytime->setColour(this->app->theme()->accent());
        this->avgplaytime->setX(this->avgplaytime->x() - this->avgplaytime->w()/2);
        this->addElement(this->avgplaytime);

        this->timeplayed = new Aether::Text(1070, 400, Utils::launchesToString(ps->launches), 20);
        this->timeplayed->setColour(this->app->theme()->accent());
        this->timeplayed->setX(this->timeplayed->x() - this->timeplayed->w()/2);
        this->addElement(this->timeplayed);

        this->firstplayed = new Aether::Text(1070, 490, Utils::Time::timestampToString(ps->firstPlayed), 20);
        this->firstplayed->setColour(this->app->theme()->accent());
        this->firstplayed->setX(this->firstplayed->x() - this->firstplayed->w()/2);
        this->addElement(this->firstplayed);

        this->lastplayed = new Aether::Text(1070, 580, Utils::Time::timestampToString(ps->lastPlayed), 20);
        this->lastplayed->setColour(this->app->theme()->accent());
        this->lastplayed->setX(this->lastplayed->x() - this->lastplayed->w()/2);
        this->addElement(this->lastplayed);
        delete ps;

        // Show update icon if needbe
        this->updateElm =nullptr;
        if (this->app->hasUpdate()) {
            this->updateElm = new Aether::Image(50, 669, "romfs:/icon/download.png");
            this->updateElm->setColour(this->app->theme()->text());
            this->addElement(this->updateElm);
        }

        // Create blank messagebox
        this->msgbox = new Aether::MessageBox();
        this->msgbox->addTopButton("common.close"_lang, [this](){
            this->msgbox->close();
        });
        this->msgbox->setLineColour(this->app->theme()->mutedLine());
        this->msgbox->setRectangleColour(this->app->theme()->altBG());
        this->msgbox->setTextColour(this->app->theme()->accent());

        // Create blank panel
        this->panel = new CustomOvl::PlaySession();
        this->panel->setAccentColour(this->app->theme()->accent());
        this->panel->setBackgroundColour(this->app->theme()->altBG());
        this->panel->setLineColour(this->app->theme()->fg());
        this->panel->setMutedLineColour(this->app->theme()->mutedLine());
        this->panel->setTextColour(this->app->theme()->text());
    }

    void Details::onUnload() {
        this->removeElement(this->playtime);
        this->removeElement(this->avgplaytime);
        this->removeElement(this->timeplayed);
        this->removeElement(this->firstplayed);
        this->removeElement(this->lastplayed);
        this->removeElement(this->icon);
        this->removeElement(this->list);
        this->removeElement(this->title);
        this->removeElement(this->userimage);
        this->removeElement(this->username);
        this->removeElement(this->updateElm);
        delete this->msgbox;
        delete this->panel;
    }
};
