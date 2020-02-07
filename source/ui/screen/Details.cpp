#include "Details.hpp"
#include "ListSession.hpp"
#include "Time.hpp"
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
        c->addItem(new Aether::ControlItem(Aether::Button::X, "Graph Time"));
        c->addItem(new Aether::ControlItem(Aether::Button::Y, "Graph Type"));
        c->setColour(this->app->theme()->text());
        this->addElement(c);
        this->graphHeading = new Aether::Text(0, 120, "", 22);
        this->graphHeading->setColour(this->app->theme()->text());
        this->addElement(this->graphHeading);
        this->graphSubheading = new Aether::Text(940, 150, "(in hours)", 16);
        this->graphSubheading->setColour(this->app->theme()->mutedText());
        this->graphSubheading->setX(this->graphSubheading->x() - this->graphSubheading->w()/2);
        this->addElement(this->graphSubheading);
        this->graphTotal = new Aether::Text(0, 545, "", 20);
        this->graphTotal->setColour(this->app->theme()->text());
        this->addElement(this->graphTotal);
        this->graphTotalSub = new Aether::Text(0, 570, "", 18);
        this->graphTotalSub->setColour(this->app->theme()->accent());
        this->addElement(this->graphTotalSub);
        this->graphPercentage = new Aether::Text(0, 545, "", 20);
        this->graphPercentage->setColour(this->app->theme()->text());
        this->addElement(this->graphPercentage);
        this->graphPercentageSub = new Aether::Text(0, 570, "", 18);
        this->graphPercentageSub->setColour(this->app->theme()->accent());
        this->addElement(this->graphPercentageSub);

        // Add key callbacks
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::AllActivity);
        });
        this->onButtonPress(Aether::Button::X, [this](){
            this->setupGraphPeriod();
        });
        this->onButtonPress(Aether::Button::Y, [this](){
            this->setupGraphType();
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

        // Set default graph variables
        this->graphData = GraphDataType::Playtime;
        this->graphView = GraphViewType::Month;
    }

    void Details::createGraph() {
        // Create graph
        if (this->graph != nullptr) {
            this->removeElement(this->graph);
        }
        this->graph = new CustomElm::Graph(660, 190, 560, 300, 2);
        this->graph->setBarColour(this->app->theme()->accent());
        this->graph->setLabelColour(this->app->theme()->text());
        this->graph->setLineColour(this->app->theme()->mutedLine());
        this->graph->setMaximumValue(20);
        this->graph->setYSteps(4);
        this->addElement(this->graph);

        // Get current time (will be set to start of range)
        struct tm t = Utils::Time::getTmForCurrentTime();

        // // Handle view period
        std::string view;
        switch (this->graphView) {
            case GraphViewType::Day:
                view = "Day";
                this->graph->setNumberOfEntries(10);
                break;

            case GraphViewType::Month:
                view = "Month";
                this->graph->setNumberOfEntries(12);
                // Set labels
                for (int i = 0; i < 12; i++) {
                    this->graph->setLabel(i, Utils::Time::getShortMonthString(i));
                }

                break;
        }

        // Handle type of data
        std::string data;
        NX::RecentPlayStatistics * s;
        switch (this->graphData) {
            case GraphDataType::Launches:
                data = "Launches";
                this->graphSubheading->setHidden(true);
                break;

            case GraphDataType::Playtime:
                data = "Play Time";
                this->graphSubheading->setHidden(false);
                t.tm_year = 119;
                t.tm_mday = 1;
                t.tm_hour = 0;
                t.tm_min = 0;
                t.tm_sec = 0;
                struct tm e = t;
                e.tm_hour = 23;
                e.tm_min = 59;
                e.tm_sec = 59;
                for (size_t i = 0; i < 12; i++) {
                    // Find end timestamp and get stats
                    t.tm_mon = i;
                    e.tm_mon = t.tm_mon;
                    e.tm_mday = Utils::Time::tmGetDaysInMonth(e);
                    s = this->app->playdata()->getRecentStatisticsForUser(this->app->activeTitle()->titleID(), Utils::Time::getTimeT(t), Utils::Time::getTimeT(e), this->app->activeUser()->ID());
                    float val = s->playtime/60/60.0;
                    this->graph->setValue(i, std::round(val));
                    delete s;
                }
                break;
        }

        // Set headings etc...
        this->graphHeading->setString(data + " per " + view);
        this->graphHeading->setX(940 - this->graphHeading->w()/2);
        this->graphTotal->setString("Total " + data + " for ...");
        this->graphTotal->setX(800 - this->graphTotal->w()/2);
        this->graphTotalSub->setString("A number");
        this->graphTotalSub->setX(800 - this->graphTotalSub->w()/2);
        this->graphPercentage->setString("Percentage of Total ...");
        this->graphPercentage->setX(1080 - this->graphPercentage->w()/2);
        this->graphPercentageSub->setString("A percentage");
        this->graphPercentageSub->setX(1080 - this->graphPercentageSub->w()/2);
    }

    void Details::setupGraphPeriod() {
        if (this->popup != nullptr) {
            delete this->popup;
        }
        this->popup = new Aether::PopupList("Graph View Period");
        this->popup->setBackgroundColour(this->app->theme()->altBG());
        this->popup->setTextColour(this->app->theme()->text());
        this->popup->setLineColour(this->app->theme()->fg());
        this->popup->setHighlightColour(this->app->theme()->accent());
        this->popup->setListLineColour(this->app->theme()->mutedLine());
        this->popup->addEntry("By Day", [this](){
            this->graphView = GraphViewType::Day;
            this->createGraph();
        }, this->graphView == GraphViewType::Day);
        this->popup->addEntry("By Month", [this](){
            this->graphView = GraphViewType::Month;
            this->createGraph();
        }, this->graphView == GraphViewType::Month);
        this->app->addOverlay(this->popup);
    }

    void Details::setupGraphType() {
        if (this->popup != nullptr) {
            delete this->popup;
        }
        this->popup = new Aether::PopupList("Graph View Activity");
        this->popup->setBackgroundColour(this->app->theme()->altBG());
        this->popup->setTextColour(this->app->theme()->text());
        this->popup->setLineColour(this->app->theme()->fg());
        this->popup->setHighlightColour(this->app->theme()->accent());
        this->popup->setListLineColour(this->app->theme()->mutedLine());
        this->popup->addEntry("Launches", [this](){
            this->graphData = GraphDataType::Launches;
            this->createGraph();
        }, this->graphData == GraphDataType::Launches);
        this->popup->addEntry("Playtime", [this](){
            this->graphData = GraphDataType::Playtime;
            this->createGraph();
        }, this->graphData == GraphDataType::Playtime);
        this->app->addOverlay(this->popup);
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
                str = std::to_string(percent);
                while (str[str.length() - 3] != '.') {
                    str = str.substr(0, str.length() - 1);
                }
                str += "%";
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
        this->graph = nullptr;
        this->createGraph();
        this->popup = nullptr;
    }

    void Details::onUnload() {
        this->removeElement(this->graph);
        this->removeElement(this->icon);
        this->removeElement(this->list);
        this->removeElement(this->title);
        this->removeElement(this->userimage);
        this->removeElement(this->username);
        delete this->msgbox;
        if (this->popup != nullptr) {
            delete this->popup;
        }
    }
};