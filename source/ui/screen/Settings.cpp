#include "Settings.hpp"

namespace Screen {
    Settings::Settings(Main::Application * a) {
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
        c->setColour(Aether::Theme::Dark.text);
        this->addElement(c);

        // Create side menu
        Aether::Menu * menu = new Aether::Menu(30, 88, 388, 559);
        menu->addElement(new Aether::MenuOption("Recent Activity", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, [this](){
            this->app->setScreen(Main::ScreenID::RecentActivity);
        }));
        menu->addElement(new Aether::MenuOption("All Activity", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, [this](){
            this->app->setScreen(Main::ScreenID::AllActivity);
        }));
        menu->addElement(new Aether::MenuSeparator(Aether::Theme::Dark.mutedLine));
        Aether::MenuOption * opt = new Aether::MenuOption("Settings", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, nullptr);
        opt->setActive(true);
        menu->addElement(opt);
        this->addElement(menu);

        // Create list
        this->list = new Aether::List(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setScrollBarColour(Aether::Theme::Dark.mutedLine);

        Aether::ListOption * lo = new Aether::ListOption("Default Sort Method", "config.sort", [this](){
            this->setupSortOverlay();
        });
        lo->setHintColour(Aether::Theme::Dark.text);
        lo->setValueColour(Aether::Theme::Dark.accent);
        lo->setLineColour(Aether::Theme::Dark.mutedLine);
        this->list->addElement(lo);

        Aether::ListComment * lc = new Aether::ListComment("Sets the sort method used upon application launch.");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);

        lo = new Aether::ListOption("Theme", "config.theme", [this](){
            this->setupThemeOverlay();
        });
        lo->setHintColour(Aether::Theme::Dark.text);
        lo->setValueColour(Aether::Theme::Dark.accent);
        lo->setLineColour(Aether::Theme::Dark.mutedLine);
        this->list->addElement(lo);

        lc = new Aether::ListComment("Sets the theme for the application. Auto chooses black/white based on your switch settings.");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);

        this->list->addElement(new Aether::ListSeparator());

        lo = new Aether::ListOption("Hide Deleted Games", "config.hidedeleted", [](){
            // change config
            // update value
        });
        lo->setHintColour(Aether::Theme::Dark.text);
        lo->setValueColour(Aether::Theme::Dark.accent);
        lo->setLineColour(Aether::Theme::Dark.mutedLine);
        this->list->addElement(lo);

        lc = new Aether::ListComment("Excludes deleted games from 'All Activity'.");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);

        this->list->addElement(new Aether::ListSeparator());

        lo = new Aether::ListOption("Replace User Page", "function()", [](){
            // call function
            // update value
        });
        lo->setHintColour(Aether::Theme::Dark.text);
        lo->setValueColour(Aether::Theme::Dark.accent);
        lo->setLineColour(Aether::Theme::Dark.mutedLine);
        this->list->addElement(lo);

        lc = new Aether::ListComment("Uses LayeredFS to replace the User Page with this app. Atmosphere 0.10.0+, ReiNX and SXOS supported.");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);

        this->list->addElement(new Aether::ListSeparator());

        lc = new Aether::ListComment("NX Activity Log v" + std::string(VER_STRING) + "\nThanks for using my app! I hope it's been useful! :)\n\nYou can support me on Ko-fi:\nhttps://ko-fi.com/tallbl0nde");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);

        this->addElement(this->list);

        // Create popuplist overlays
        this->sortOverlay = new Aether::PopupList("Default Sort Method");
        this->sortOverlay->setBackgroundColour(Aether::Theme::Dark.altBG);
        this->sortOverlay->setHighlightColour(Aether::Theme::Dark.accent);
        this->sortOverlay->setLineColour(Aether::Theme::Dark.fg);
        this->sortOverlay->setListLineColour(Aether::Theme::Dark.mutedLine);
        this->sortOverlay->setTextColour(Aether::Theme::Dark.text);
        this->themeOverlay = new Aether::PopupList("Application Theme");
        this->themeOverlay->setBackgroundColour(Aether::Theme::Dark.altBG);
        this->themeOverlay->setHighlightColour(Aether::Theme::Dark.accent);
        this->themeOverlay->setLineColour(Aether::Theme::Dark.fg);
        this->themeOverlay->setListLineColour(Aether::Theme::Dark.mutedLine);
        this->themeOverlay->setTextColour(Aether::Theme::Dark.text);

        // Add button callbacks
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::UserSelect);
        });
    }

    void Settings::setupSortOverlay() {
        // Remove previous entries
        this->sortOverlay->close(false);
        this->sortOverlay->removeEntries();

        // Add an entry for each sort method
        SortType t = SortType::AlphaAsc;
        this->sortOverlay->addEntry("Name", [this](){
            // this->app->config()->setSortMethod()
        }, t == SortType::AlphaAsc);
        this->sortOverlay->addEntry("First Playtime", [this](){
            // this->app->config()->setSortMethod()
        }, t == SortType::FirstPlayed);
        this->sortOverlay->addEntry("Most Recently Played", [this](){
            // this->app->config()->setSortMethod()
        }, t == SortType::LastPlayed);
        this->sortOverlay->addEntry("Longest Playtime", [this](){
            // this->app->config()->setSortMethod()
        }, t == SortType::HoursAsc);
        this->sortOverlay->addEntry("Shortest Playtime", [this](){
            // this->app->config()->setSortMethod()
        }, t == SortType::HoursDec);
        this->sortOverlay->addEntry("Most Launched", [this](){
            // this->app->config()->setSortMethod()
        }, t == SortType::LaunchAsc);
        this->sortOverlay->addEntry("Least Launched", [this](){
            // this->app->config()->setSortMethod()
        }, t == SortType::LaunchDec);

        this->app->addOverlay(this->sortOverlay);
    }

    void Settings::setupThemeOverlay() {
        // Remove previous entries
        this->themeOverlay->close(false);
        this->themeOverlay->removeEntries();

        // Add an entry for each sort method
        ThemeType t = ThemeType::T_Auto;
        this->themeOverlay->addEntry("Auto", [this](){
            // this->app->config()->setTheme()
        }, t == ThemeType::T_Auto);
        this->themeOverlay->addEntry("Basic Black", [this](){
            // this->app->config()->setTheme()
        }, t == ThemeType::T_Dark);
        this->themeOverlay->addEntry("Basic White", [this](){
            // this->app->config()->setTheme()
        }, t == ThemeType::T_Light);
        // this->themeOverlay->addEntry("Custom", [this](){
        //     this->app->config()->setTheme()
        // }, t == ThemeType::T_Custom);

        this->app->addOverlay(this->themeOverlay);
    }

    void Settings::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, this->app->activeUser()->username() + "'s Activity", 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(Aether::Theme::Dark.text);
        this->addElement(this->heading);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize());
        this->image->setWH(60, 60);
        this->addElement(this->image);
    }

    void Settings::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->image);
    }

    Settings::~Settings() {
        // Delete overlays
        delete this->sortOverlay;
        delete this->themeOverlay;
    }
};
