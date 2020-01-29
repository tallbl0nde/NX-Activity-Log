#include "ListUser.hpp"
#include "UserSelect.hpp"

namespace Screen {
    UserSelect::UserSelect(Main::Application * a, std::vector<User *> u) : Screen() {
        this->app = a;
        this->users = u;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(Aether::Theme::Dark.fg);
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(Aether::Theme::Dark.fg);
        this->addElement(r);
        Aether::Text * t = new Aether::Text(65, 44, "Select a User", 28);
        t->setY(t->y() - t->h()/2);
        t->setColour(Aether::Theme::Dark.text);
        this->addElement(t);
        Aether::Controls * c = new Aether::Controls();
        c->addItem(new Aether::ControlItem(Aether::Button::A, "OK"));
        c->addItem(new Aether::ControlItem(Aether::Button::PLUS, "Exit"));
        c->setColour(Aether::Theme::Dark.text);
        this->addElement(c);

        // Add handling of plus/B to exit
        this->onButtonPress(Aether::Button::PLUS, [this](){
            this->app->exit();
        });
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->exit();
        });

        // Create list (but don't populate yet)
        this->list = new Aether::List(340, 88, 600, 558);
        this->list->setScrollBarColour(Aether::Theme::Dark.mutedLine);
        this->addElement(this->list);
    }

    void UserSelect::onLoad() {
        // Create list items for each user in the vector
        for (size_t i = 0; i < this->users.size(); i++) {
            CustomElm::ListUser * l = new CustomElm::ListUser(this->users[i]->username(), this->users[i]->imgPtr(), this->users[i]->imgSize());
            l->setLineColour(Aether::Theme::Dark.mutedLine);
            l->setTextColour(Aether::Theme::Dark.text);
            l->setCallback([this, i](){
                this->app->setActiveUser(i);
                this->app->setScreen(Main::ScreenID::AllActivity);
            });
            this->list->addElement(l);
        }
        this->setFocussed(this->list);
    }

    void UserSelect::onUnload() {
        // Remove + delete elements within list
        this->list->removeAllElements();
    }
};