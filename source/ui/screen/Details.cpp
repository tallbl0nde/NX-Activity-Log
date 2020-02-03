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
        c->setColour(this->app->theme()->text());
        this->addElement(c);

        // Add key callbacks
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::AllActivity);
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

    void Details::setupSessionHelp() {
        this->msgboxHelp->emptyBody();
        this->msgboxHelp->close(false);

        int bw, bh;
        this->msgboxHelp->getBodySize(&bw, &bh);
        Aether::Element * body = new Aether::Element(0, 0, bw, bh);
        Aether::TextBlock * tb = new Aether::TextBlock(50, 40, "A Play Session represents the time between when a game was launched to when it was quit in succession.", 22, bw - 100);
        tb->setColour(this->app->theme()->text());
        body->addElement(tb);
        tb = new Aether::TextBlock(50, tb->y() + tb->h() + 20, "The time of launch, the session's play time and percentage of overall playtime is shown.\n\nSelect a session to view a more detailed breakdown.", 22, bw - 100);
        tb->setColour(this->app->theme()->mutedText());
        body->addElement(tb);
        body->setWH(bw, tb->y() + tb->h() + 40);
        this->msgboxHelp->setBodySize(body->w(), body->h());
        this->msgboxHelp->setBody(body);

        this->app->addOverlay(this->msgboxHelp);
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
        this->loPlayTime = new Aether::ListOption("Play Time", Utils::Time::playtimeToString(ps->playtime, ", "), nullptr);
        this->loPlayTime->setH(SUMMARY_BOX_HEIGHT);
        this->loPlayTime->setFontSize(SUMMARY_FONT_SIZE);
        this->loPlayTime->setSelectable(false);
        this->loPlayTime->setTouchable(false);
        this->loPlayTime->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loPlayTime);
        this->loAverageTime = new Aether::ListOption("Avg. Play Time", Utils::Time::playtimeToString(ps->playtime / ps->launches, ", "), nullptr);
        this->loAverageTime->setH(SUMMARY_BOX_HEIGHT);
        this->loAverageTime->setFontSize(SUMMARY_FONT_SIZE);
        this->loAverageTime->setSelectable(false);
        this->loAverageTime->setTouchable(false);
        this->loAverageTime->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loAverageTime);
        this->loLaunched = new Aether::ListOption("Times Played", Utils::formatNumberComma(ps->launches) + " times", nullptr);
        this->loLaunched->setH(SUMMARY_BOX_HEIGHT);
        this->loLaunched->setFontSize(SUMMARY_FONT_SIZE);
        this->loLaunched->setSelectable(false);
        this->loLaunched->setTouchable(false);
        this->loLaunched->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loLaunched);
        this->loFirstTime = new Aether::ListOption("First Played", Utils::Time::timestampToString(pdmPlayTimestampToPosix(ps->firstPlayed)), nullptr);
        this->loFirstTime->setH(SUMMARY_BOX_HEIGHT);
        this->loFirstTime->setFontSize(SUMMARY_FONT_SIZE);
        this->loFirstTime->setSelectable(false);
        this->loFirstTime->setTouchable(false);
        this->loFirstTime->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loFirstTime);
        this->loLastTime = new Aether::ListOption("Last Played", Utils::Time::timestampToString(pdmPlayTimestampToPosix(ps->lastPlayed)), nullptr);
        this->loLastTime->setH(SUMMARY_BOX_HEIGHT);
        this->loLastTime->setFontSize(SUMMARY_FONT_SIZE);
        this->loLastTime->setSelectable(false);
        this->loLastTime->setTouchable(false);
        this->loLastTime->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loLastTime);
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
            ls->setCallback(nullptr);
            this->list->addElement(ls);
        }

        delete ps;
        this->addElement(this->list);
        this->setFocussed(this->list);

        // Create blank messagebox
        this->msgboxHelp = new Aether::MessageBox();
        this->msgboxHelp->addTopButton("Close", [this](){
            this->msgboxHelp->close(true);
        });
        this->msgboxHelp->setLineColour(this->app->theme()->mutedLine());
        this->msgboxHelp->setRectangleColour(this->app->theme()->altBG());
        this->msgboxHelp->setTextColour(this->app->theme()->accent());
    }

    void Details::onUnload() {
        this->removeElement(this->icon);
        this->removeElement(this->list);
        this->removeElement(this->title);
        this->removeElement(this->userimage);
        this->removeElement(this->username);
        delete this->msgboxHelp;
    }
};