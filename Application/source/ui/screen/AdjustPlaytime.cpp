#include "Application.hpp"
#include "utils/Lang.hpp"
#include "ui/element/ListAdjust.hpp"
#include "ui/screen/AdjustPlaytime.hpp"
#include "utils/Utils.hpp"

namespace Screen {
    AdjustPlaytime::AdjustPlaytime(Main::Application * app) : Aether::Screen() {
        this->app = app;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::Text * t = new Aether::Text(65, 44, "adjustPlaytime.heading"_lang, 28);
        t->setY(t->y() - t->h()/2);
        t->setColour(this->app->theme()->text());
        this->addElement(t);
        Aether::ControlBar * c = new Aether::ControlBar();
        c->addControl(Aether::Button::A, "common.buttonHint.ok"_lang);
        c->addControl(Aether::Button::B, "common.buttonHint.back"_lang);
        c->setDisabledColour(this->app->theme()->text());
        c->setEnabledColour(this->app->theme()->text());
        this->addElement(c);

        // Save on X
        this->onButtonPress(Aether::Button::X, [this]() {
            // TODO: save
            this->app->popScreen();
        });
        // Quit without saving on B
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->popScreen();
        });

        // Scroll faster with ZL/ZR
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

    void AdjustPlaytime::onLoad() {
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

        // Create list
        this->list = new Aether::List(200, 88, 880, 559);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // Populate list with all titles
        std::vector<NX::Title *> titles = this->app->titleVector();
        std::sort(titles.begin(), titles.end(), [](NX::Title * lhs, NX::Title * rhs) {
            return (lhs->name() < rhs->name());
        });

        for (NX::Title * title : titles) {
            NX::PlayStatistics * stats = this->app->playdata()->getStatisticsForUser(title->titleID(), this->app->activeUser()->ID());
            int adjustTime = 0;
            CustomElm::ListAdjust * l = new CustomElm::ListAdjust(title->name(), Utils::playtimeToPlayedForString(stats->playtime), (adjustTime < 0 ? "- " : "+ ") + Utils::playtimeToString(std::abs(adjustTime)));
            delete stats;

            l->setImage(title->imgPtr(), title->imgSize());
            l->setAdjustColour(this->app->theme()->accent());
            l->setLineColour(this->app->theme()->mutedLine());
            l->setRecordColour(this->app->theme()->mutedText());
            l->setTitleColour(this->app->theme()->text());
            l->onPress([this]() {

            });

            this->list->addElement(l);
        }

        this->addElement(this->list);
    }

    void AdjustPlaytime::onUnload() {
        this->removeElement(this->userimage);
        this->removeElement(this->username);
        this->removeElement(this->list);
    }
};
