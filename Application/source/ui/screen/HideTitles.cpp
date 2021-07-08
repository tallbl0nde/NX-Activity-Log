#include <algorithm>
#include "Application.hpp"
#include "utils/Lang.hpp"
#include "ui/element/ListHide.hpp"
#include "ui/screen/HideTitles.hpp"
#include "utils/Utils.hpp"

namespace Screen {
    HideTitles::HideTitles(Main::Application * a) : Aether::Screen() {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::Text * t = new Aether::Text(65, 44, "hideTitles.heading"_lang, 28);
        t->setY(t->y() - t->h()/2);
        t->setColour(this->app->theme()->text());
        this->addElement(t);
        Aether::ControlBar * c = new Aether::ControlBar();
        c->addControl(Aether::Button::A, "common.buttonHint.ok"_lang);
        c->addControl(Aether::Button::B, "common.buttonHint.back"_lang);
        c->setDisabledColour(this->app->theme()->text());
        c->setEnabledColour(this->app->theme()->text());
        this->addElement(c);

        // Create side panel
        if (!this->app->config()->tImage() || this->app->config()->gTheme() != ThemeType::Custom) {
            r = new Aether::Rectangle(890, 88, 360, 559);
            r->setColour(this->app->theme()->altBG());
            this->addElement(r);
        }

        // Titles hidden and save button
        this->hiddenCountText = new Aether::Text(0, 190, "0", 30);
        this->hiddenCountText->setColour(this->app->theme()->text());
        this->addElement(this->hiddenCountText);
        this->hiddenSubText = new Aether::TextBlock(0, this->hiddenCountText->y() + this->hiddenCountText->h() + 10, "hideTitles.titlesHidden"_lang, 20, 300);
        this->hiddenSubText->setColour(this->app->theme()->mutedText());
        this->addElement(this->hiddenSubText);

        Aether::FilledButton * b = new Aether::FilledButton(980, 500, 180, 60, "hideTitles.save"_lang, 22, [this]() {
            this->app->config()->setHiddenTitles(this->hiddenIDs);
            this->app->popScreen();
        });
        b->setFillColour(this->app->theme()->accent());
        b->setTextColour(this->app->theme()->altBG());
        this->addElement(b);

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

    void HideTitles::updateHiddenCounter() {
        this->hiddenCountText->setString(std::to_string(this->hiddenIDs.size()));
        this->hiddenCountText->setX(1070 - this->hiddenCountText->w()/2);
        this->hiddenSubText->setString(this->hiddenIDs.size() == 1 ? "hideTitles.titleHidden"_lang : "hideTitles.titlesHidden"_lang);
        this->hiddenSubText->setX(1070 - this->hiddenSubText->w()/2);
    }

    void HideTitles::onLoad() {
        // Create list
        this->list = new Aether::List(30, 88, 830, 559);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // Populate list with all titles
        this->hiddenIDs = this->app->config()->hiddenTitles();
        std::vector<NX::Title *> titles = this->app->titleVector();
        std::sort(titles.begin(), titles.end(), [](NX::Title * lhs, NX::Title * rhs) {
            return (lhs->name() < rhs->name());
        });

        for (NX::Title * title : titles) {
            CustomElm::ListHide * l = new CustomElm::ListHide(title->name(), Utils::formatHexString(title->titleID()));
            l->setImage(title->imgPtr(), title->imgSize());
            l->setIDColour(this->app->theme()->mutedText());
            l->setLineColour(this->app->theme()->mutedLine());
            l->setTitleColour(this->app->theme()->text());
            l->setTickBackgroundColour(this->app->theme()->accent());
            l->setTickForegroundColour(this->app->theme()->mutedLine());
            l->onPress([this, title, l]() {
                if (l->isTicked()) {
                    l->setTicked(false);
                    this->hiddenIDs.erase(std::remove(this->hiddenIDs.begin(), this->hiddenIDs.end(), title->titleID()), this->hiddenIDs.end());

                } else {
                    l->setTicked(true);
                    this->hiddenIDs.push_back(title->titleID());
                }
                this->updateHiddenCounter();
            });
            bool hidden = std::find(this->hiddenIDs.begin(), this->hiddenIDs.end(), title->titleID()) != this->hiddenIDs.end();
            l->setTicked(hidden);

            this->list->addElement(l);
        }

        this->addElement(this->list);
        this->setFocused(this->list);
        this->updateHiddenCounter();
    }

    void HideTitles::onUnload() {
        this->removeElement(this->list);
    }
};
