#include "Application.hpp"
#include "utils/Lang.hpp"
#include "ui/element/ListUser.hpp"
#include "ui/screen/UserSelect.hpp"

namespace Screen {
    UserSelect::UserSelect(Main::Application * a, std::vector<NX::User *> u) {
        this->app = a;
        this->users = u;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::Text * t = new Aether::Text(65, 44, "userSelect.heading"_lang, 28);
        t->setY(t->y() - t->h()/2);
        t->setColour(this->app->theme()->text());
        this->addElement(t);
        Aether::ControlBar * c = new Aether::ControlBar();
        c->addControl(Aether::Button::A, "common.buttonHint.ok"_lang);
        c->addControl(Aether::Button::PLUS, "common.buttonHint.exit"_lang);
        c->setDisabledColour(this->app->theme()->text());
        c->setEnabledColour(this->app->theme()->text());
        this->addElement(c);

        // Add handling of plus/B to exit
        this->onButtonPress(Aether::Button::PLUS, [this](){
            this->app->exit();
        });
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->exit();
        });
    }

    void UserSelect::onLoad() {
        // Create list
        this->list = new Aether::List(340, 88, 600, 558);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());
        this->addElement(this->list);

        // Create list items for each user in the vector
        for (size_t i = 0; i < this->users.size(); i++) {
            CustomElm::ListUser * l = new CustomElm::ListUser(this->users[i]->username(), this->users[i]->imgPtr(), this->users[i]->imgSize());
            l->setLineColour(this->app->theme()->mutedLine());
            l->setTextColour(this->app->theme()->text());
            l->onPress([this, i](){
                this->app->setActiveUser(i);
                this->app->setScreen(this->app->config()->lScreen());
            });
            this->list->addElement(l);
        }
        this->setFocussed(this->list);

        // Show update icon if needbe
        this->updateElm =nullptr;
        if (this->app->hasUpdate()) {
            this->updateElm = new Aether::Image(50, 669, "romfs:/icon/download.png");
            this->updateElm->setColour(this->app->theme()->text());
            this->addElement(this->updateElm);
        }
    }

    void UserSelect::onUnload() {
        // Remove + delete elements within list
        this->removeElement(this->updateElm);
        this->removeElement(this->list);
    }
};