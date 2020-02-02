#include "Details.hpp"
#include "Utils.hpp"

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

        // Create list with placeholders
        this->list = new Aether::List(40, 88, 580, 559);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());
        Aether::ListHeading * h = new Aether::ListHeading("Summary");
        h->setRectColour(this->app->theme()->mutedLine());
        h->setTextColour(this->app->theme()->text());
        this->list->addElement(h);
        this->list->addElement(new Aether::ListSeparator(20));
        this->loPlayTime = new Aether::ListOption("Play Time", "", nullptr);
        this->loPlayTime->setSelectable(false);
        this->loPlayTime->setTouchable(false);
        this->loPlayTime->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loPlayTime);
        this->loAverageTime = new Aether::ListOption("Average Play Time", "", nullptr);
        this->loAverageTime->setSelectable(false);
        this->loAverageTime->setTouchable(false);
        this->loAverageTime->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loAverageTime);
        this->loLaunched = new Aether::ListOption("Times Played", "", nullptr);
        this->loLaunched->setSelectable(false);
        this->loLaunched->setTouchable(false);
        this->loLaunched->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loLaunched);
        this->loFirstTime = new Aether::ListOption("First Played", "", nullptr);
        this->loFirstTime->setSelectable(false);
        this->loFirstTime->setTouchable(false);
        this->loFirstTime->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loFirstTime);
        this->loLastTime = new Aether::ListOption("Last Played", "", nullptr);
        this->loLastTime->setSelectable(false);
        this->loLastTime->setTouchable(false);
        this->loLastTime->setColours(this->app->theme()->mutedLine(), this->app->theme()->text(), this->app->theme()->accent());
        this->list->addElement(this->loLastTime);
        this->list->addElement(new Aether::ListSeparator());

        Aether::ListHeadingHelp * lhh = new Aether::ListHeadingHelp("Play Sessions", nullptr);
        lhh->setHelpColour(this->app->theme()->mutedText());
        lhh->setRectColour(this->app->theme()->mutedLine());
        lhh->setTextColour(this->app->theme()->text());
        this->list->addElement(lhh);
        this->addElement(this->list);
        this->setFocussed(this->list);

        // Add key callback
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::AllActivity);
        });
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

        // Set values of summary
        NX::PlayStatistics * ps = this->app->playdata()->getStatisticsForUser(this->app->activeTitle()->titleID(), this->app->activeUser()->ID());
        this->loPlayTime->setValue(Utils::Time::playtimeToString(ps->playtime, ", "));
        this->loAverageTime->setValue(Utils::Time::playtimeToString(ps->playtime / ps->launches, ", "));
        this->loLaunched->setValue(Utils::formatNumberComma(ps->launches) + " times");
        this->loFirstTime->setValue(Utils::Time::timestampToString(pdmPlayTimestampToPosix(ps->firstPlayed)));
        this->loLastTime->setValue(Utils::Time::timestampToString(pdmPlayTimestampToPosix(ps->lastPlayed)));
        delete ps;
    }

    void Details::onUnload() {
        this->removeElement(this->icon);
        this->removeElement(this->title);
        this->removeElement(this->userimage);
        this->removeElement(this->username);
    }
};