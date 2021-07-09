#include "Application.hpp"
#include "utils/Lang.hpp"
#include "ui/element/ListAdjust.hpp"
#include "ui/overlay/PlaytimePicker.hpp"
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
        c->addControl(Aether::Button::X, "adjustPlaytime.save"_lang);
        c->setDisabledColour(this->app->theme()->text());
        c->setEnabledColour(this->app->theme()->text());
        this->addElement(c);

        // Save on X
        this->onButtonPress(Aether::Button::X, [this]() {
            this->app->config()->setAdjustmentValues(this->adjustments);
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

    std::string AdjustPlaytime::getValueString(int val) {
        return (val < 0 ? "- " : "+ ") + Utils::playtimeToString(std::abs(val));
    }

    void AdjustPlaytime::setupPlaytimePicker(const std::string & title, size_t idx, CustomElm::ListAdjust * l) {
        delete this->picker;
        this->picker = new CustomOvl::PlaytimePicker(title, this->adjustments[idx].value, [this, idx, l](int val) {
            this->adjustments[idx].value = val;
            l->setAdjustedTime(this->getValueString(val));
        });
        this->picker->setBackLabel("common.buttonHint.back"_lang);
        this->picker->setOKLabel("common.buttonHint.ok"_lang);
        this->picker->setTipText("customTheme.picker.tip"_lang);
        this->picker->setHourHint("adjustPlaytime.picker.hour"_lang);
        this->picker->setMinuteHint("adjustPlaytime.picker.minute"_lang);
        this->picker->setSecondHint("adjustPlaytime.picker.second"_lang);
        this->picker->setBackgroundColour(this->app->theme()->altBG());
        this->picker->setHighlightColour(this->app->theme()->accent());
        this->picker->setInactiveColour(this->app->theme()->mutedText());
        this->picker->setTextColour(this->app->theme()->text());
        this->app->addOverlay(this->picker);
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

        this->adjustments = this->app->config()->adjustmentValues();
        std::vector<uint64_t> hidden = this->app->config()->hiddenTitles();
        for (NX::Title * title : titles) {
            // Skip over hidden games
            if (std::find(hidden.begin(), hidden.end(), title->titleID()) != hidden.end()) {
                continue;
            }

            // Find adjustment value or create if one doesn't exist
            std::vector<AdjustmentValue>::iterator it = std::find_if(this->adjustments.begin(), this->adjustments.end(), [this, title](AdjustmentValue val) {
                return (val.titleID == title->titleID() && val.userID == this->app->activeUser()->ID());
            });
            if (it == this->adjustments.end()) {
                this->adjustments.push_back(AdjustmentValue{title->titleID(), this->app->activeUser()->ID(), 0});
                it = (this->adjustments.end() - 1);
            }

            size_t idx = std::distance(this->adjustments.begin(), it);
            NX::PlayStatistics * stats = this->app->playdata()->getStatisticsForUser(title->titleID(), this->app->activeUser()->ID());
            CustomElm::ListAdjust * l = new CustomElm::ListAdjust(title->name(), Utils::playtimeToPlayedForString(stats->playtime), this->getValueString(this->adjustments[idx].value));
            delete stats;

            l->setImage(title->imgPtr(), title->imgSize());
            l->setAdjustColour(this->app->theme()->accent());
            l->setLineColour(this->app->theme()->mutedLine());
            l->setRecordColour(this->app->theme()->mutedText());
            l->setTitleColour(this->app->theme()->text());
            l->onPress([this, title, idx, l]() {
                this->setupPlaytimePicker(title->name(), idx, l);
            });

            this->list->addElement(l);
        }

        this->addElement(this->list);
        this->picker = nullptr;
    }

    void AdjustPlaytime::onUnload() {
        delete this->picker;
        this->removeElement(this->userimage);
        this->removeElement(this->username);
        this->removeElement(this->list);
    }
};
