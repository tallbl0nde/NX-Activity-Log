#include "AllActivity.hpp"
#include "TimeHelper.hpp"

namespace Screen {
    AllActivity::AllActivity(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(400, 88, 850, 559);
        r->setColour(Aether::Theme::Dark.altBG);
        this->addElement(r);
        r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(Aether::Theme::Dark.fg);
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(Aether::Theme::Dark.fg);
        this->addElement(r);
        Aether::Controls * c = new Aether::Controls();
        c->addItem(new Aether::ControlItem(Aether::Button::A, "OK"));
        c->addItem(new Aether::ControlItem(Aether::Button::B, "Back"));
        c->addItem(new Aether::ControlItem(Aether::Button::X, "Sort"));
        c->setColour(Aether::Theme::Dark.text);
        this->addElement(c);

        // Create side menu
        Aether::Menu * menu = new Aether::Menu(30, 88, 388, 559);
        menu->addElement(new Aether::MenuOption("Recent Activity", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, [this](){
            this->app->setScreen(Main::ScreenID::RecentActivity);
        }));
        Aether::MenuOption * opt = new Aether::MenuOption("All Activity", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, nullptr);
        opt->setActive(true);
        menu->addElement(opt);
        menu->addElement(new Aether::MenuSeparator(Aether::Theme::Dark.mutedLine));
        menu->addElement(new Aether::MenuOption("Settings", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, [this](){
            this->app->setScreen(Main::ScreenID::Settings);
        }));
        this->addElement(menu);

        // Create list (but don't populate yet)
        this->list = new Aether::List(400, 88, 850, 559);
        this->addElement(this->list);

        // Add callbacks for buttons
        this->onButtonPress(Aether::Button::X, [this](){
            // push overlay
        });
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::UserSelect);
        });
    }

    void AllActivity::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, this->app->activeUser()->username() + "'s Activity", 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(Aether::Theme::Dark.text);
        this->addElement(this->heading);

        // Render total hours string
        std::string txt = "Total Playtime: " + TimeH::playtimeToString(0, " and ");
        this->hours = new Aether::Text(1215, 44, txt, 20);
        this->hours->setXY(this->hours->x() - this->hours->w(), this->hours->y() - this->hours->h()/2);
        this->hours->setColour(Aether::Theme::Dark.mutedText);
        this->addElement(this->hours);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize());
        this->image->setWH(60, 60);
        this->addElement(this->image);

        // Populate list
        this->list->addElement(new Aether::ListComment("Your mum gay\n\nTHat's all."));
    }

    void AllActivity::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->hours);
        this->removeElement(this->image);
        this->list->removeAllElements();
    }
};