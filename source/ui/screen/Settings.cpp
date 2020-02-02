#include <filesystem>
#include "Settings.hpp"
#include "Utils.hpp"

namespace Screen {
    Settings::Settings(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(400, 88, 850, 559);
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

        // Create side menu
        Aether::Menu * menu = new Aether::Menu(30, 88, 388, 559);
        menu->addElement(new Aether::MenuOption("Recent Activity", this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(Main::ScreenID::RecentActivity);
        }));
        menu->addElement(new Aether::MenuOption("All Activity", this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(Main::ScreenID::AllActivity);
        }));
        menu->addElement(new Aether::MenuSeparator(this->app->theme()->mutedLine()));
        Aether::MenuOption * opt = new Aether::MenuOption("Settings", this->app->theme()->accent(), this->app->theme()->text(), nullptr);
        opt->setActive(true);
        menu->addElement(opt);
        this->addElement(menu);

        // Create list
        this->list = new Aether::List(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // DEFAULT SORT METHOD
        std::string str = "";
        switch (this->app->config()->gSort()) {
            case SortType::AlphaAsc:
                str = "By Name";
                break;

            case SortType::HoursAsc:
                str = "By Most Playtime";
                break;

            case SortType::HoursDec:
                str = "By Least Playtime";
                break;

            case SortType::LaunchAsc:
                str = "By Most Launched";
                break;

            case SortType::LaunchDec:
                str = "By Least Launched";
                break;

            case SortType::FirstPlayed:
                str = "By First Playtime";
                break;

            case SortType::LastPlayed:
                str = "By Most Recently Played";
                break;
        }
        this->optionSort = new Aether::ListOption("Default Sort Method", str, [this](){
            this->setupSortOverlay();
        });
        this->optionSort->setHintColour(this->app->theme()->text());
        this->optionSort->setValueColour(this->app->theme()->accent());
        this->optionSort->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionSort);
        Aether::ListComment * lc = new Aether::ListComment("Sets the sort method used upon application launch.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // THEME
        switch (this->app->config()->gTheme()) {
            case Auto:
                str = "Auto";
                break;

            case Custom:
                str = "Custom";
                break;

            case Dark:
                str = "Basic Black";
                break;

            case Light:
                str = "Basic White";
                break;
        }
        this->optionTheme = new Aether::ListOption("Theme", str, [this](){
            this->setupThemeOverlay();
        });
        this->optionTheme->setHintColour(this->app->theme()->text());
        this->optionTheme->setValueColour(this->app->theme()->accent());
        this->optionTheme->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionTheme);
        lc = new Aether::ListComment("Sets the theme for the application. Auto chooses black/white based on your switch settings. Note: This currently requires the app to be restarted to take effect.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);
        this->list->addElement(new Aether::ListSeparator());

        // HIDE DELETED
        str = (this->app->config()->hDeleted() ? "Yes" : "No");
        this->optionDeleted = new Aether::ListOption("Hide Deleted Games", str, [this](){
            this->app->config()->setHDeleted(!this->app->config()->hDeleted());
            this->optionDeleted->setValue((this->app->config()->hDeleted() ? "Yes" : "No"));
        });
        this->optionDeleted->setHintColour(this->app->theme()->text());
        this->optionDeleted->setValueColour(this->app->theme()->accent());
        this->optionDeleted->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionDeleted);
        lc = new Aether::ListComment("Excludes deleted games from 'All Activity'.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);
        this->list->addElement(new Aether::ListSeparator());

        // REPLACE USER PAGE
        str = "Disabled";
        if (std::filesystem::exists("/atmosphere/contents/0100000000001013/exefs.nsp") || std::filesystem::exists("/ReiNX/titles/0100000000001013/exefs.nsp") || std::filesystem::exists("/sxos/titles/0100000000001013/exefs.nsp")) {
            str = "Enabled";
        }
        this->optionPage = new Aether::ListOption("Replace User Page", str, [this](){
            this->installForwarder();
        });
        this->optionPage->setHintColour(this->app->theme()->text());
        this->optionPage->setValueColour(this->app->theme()->accent());
        this->optionPage->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionPage);

        lc = new Aether::ListComment("Uses LayeredFS to replace the User Page with this app. Atmosphere 0.10.0+, ReiNX and SXOS supported.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);
        this->list->addElement(new Aether::ListSeparator());

        // INFORMATION
        lc = new Aether::ListComment("NX Activity Log v" + std::string(VER_STRING) + "\nThanks for using my app! I hope it's been useful! :)\n\nYou can support me on Ko-fi:\nhttps://ko-fi.com/tallbl0nde");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        this->addElement(this->list);

        // Create popuplist overlays
        this->sortOverlay = new Aether::PopupList("Default Sort Method");
        this->sortOverlay->setBackgroundColour(this->app->theme()->altBG());
        this->sortOverlay->setHighlightColour(this->app->theme()->accent());
        this->sortOverlay->setLineColour(this->app->theme()->fg());
        this->sortOverlay->setListLineColour(this->app->theme()->mutedLine());
        this->sortOverlay->setTextColour(this->app->theme()->text());
        this->themeOverlay = new Aether::PopupList("Application Theme");
        this->themeOverlay->setBackgroundColour(this->app->theme()->altBG());
        this->themeOverlay->setHighlightColour(this->app->theme()->accent());
        this->themeOverlay->setLineColour(this->app->theme()->fg());
        this->themeOverlay->setListLineColour(this->app->theme()->mutedLine());
        this->themeOverlay->setTextColour(this->app->theme()->text());

        // Create base message box
        this->msgbox = new Aether::MessageBox();
        this->msgbox->addTopButton("OK", [this](){
            this->msgbox->close(true);
        });
        this->msgbox->setLineColour(this->app->theme()->mutedLine());
        this->msgbox->setRectangleColour(this->app->theme()->altBG());
        this->msgbox->setTextColour(this->app->theme()->accent());

        // Add button callbacks
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::UserSelect);
        });
    }

    void Settings::installForwarder() {
        // A message box will be shown regardless of the outcome
        this->msgbox->close(false);
        this->msgbox->emptyBody();

        // Check if forwarder exists
        bool atms = false;
        bool rei = false;
        bool sx = false;
        bool hasAtms = false;
        bool hasRei = false;
        bool hasSx = false;
        if (std::filesystem::exists("/atmosphere/contents")) {
            hasAtms = true;
            if (std::filesystem::exists("/atmosphere/contents/0100000000001013/exefs.nsp")) {
                atms = true;
            }
        }
        if (std::filesystem::exists("/ReiNX/titles")) {
            hasRei = true;
            if (std::filesystem::exists("/ReiNX/titles/0100000000001013/exefs.nsp")) {
                rei = true;
            }
        }
        if (std::filesystem::exists("/sxos/titles")) {
            hasSx = true;
            if (std::filesystem::exists("/sxos/titles/0100000000001013/exefs.nsp")) {
                sx = true;
            }
        }

        // If the file is present, delete it
        if (atms || rei || sx) {
            if (atms) {
                std::filesystem::remove("/atmosphere/contents/0100000000001013/exefs.nsp");
                atms = false;
            }
            if (rei) {
                std::filesystem::remove("/ReiNX/titles/0100000000001013/exefs.nsp");
                rei = false;
            }
            if (sx) {
                std::filesystem::remove("/sxos/titles/0100000000001013/exefs.nsp");
                sx = false;
            }

        // Otherwise if it isn't, copy it
        } else {
            // First check if .nro is in correct location
            if (!(std::filesystem::exists("/switch/NX-Activity-Log/NX-Activity-Log.nro"))) {
                // Create text and add to overlay
                Aether::Element * body = new Aether::Element(0, 0, 770, 230);
                Aether::Text * heading = new Aether::Text(50, 40, "An error occurred:", 24);
                heading->setColour(this->app->theme()->text());
                body->addElement(heading);
                Aether::TextBlock * message = new Aether::TextBlock(50, 90, "This application's .nro was not found at the expected location:\n/switch/NX-Activity-Log/NX-Activity-Log.nro\n\nThis feature requires the .nro be present at this location.\nPlease move/rename any files + folders and try again.", 20, body->w() - 100);
                message->setColour(this->app->theme()->mutedText());
                body->addElement(message);
                this->msgbox->setBody(body);
                this->msgbox->setBodySize(body->w(), body->h());
                this->app->addOverlay(this->msgbox);
                return;
            }

            if (hasAtms) {
                std::filesystem::create_directory("/atmosphere/contents/0100000000001013");
                Utils::copyFile("romfs:/exefs.nsp", "/atmosphere/contents/0100000000001013/exefs.nsp");
                atms = true;
            }

            if (hasRei) {
                std::filesystem::create_directory("/ReiNX/titles/0100000000001013");
                Utils::copyFile("romfs:/exefs.nsp", "/ReiNX/titles/0100000000001013/exefs.nsp");
                rei = true;
            }

            if (hasSx) {
                std::filesystem::create_directory("/sxos/titles/0100000000001013");
                Utils::copyFile("romfs:/exefs.nsp", "/sxos/titles/0100000000001013/exefs.nsp");
                sx = true;
            }
        }

        // Create message box based on result (success)
        if (atms || rei || sx) {
            Aether::Element * body = new Aether::Element(0, 0, 770, 250);
            Aether::Text * heading = new Aether::Text(50, 40, "Installation Successful!", 24);
            heading->setColour(this->app->theme()->text());
            body->addElement(heading);
            std::string str = std::string(atms ? "Atmosphere  " : "") + std::string(rei ? "ReiNX  " : "") + std::string(sx ? "SX OS" : "");
            Aether::TextBlock * message = new Aether::TextBlock(50, 90, "The following CFWs were detected and have this feature enabled:\n" + str + "\n\nNote: This feature does not use a sysmodule, if you encounter any errors either disable this feature in the app or delete\n<cfw contents folder>/0100000000001013/exefs.nsp", 20, body->w() - 100);
            message->setColour(this->app->theme()->mutedText());
            body->addElement(message);
            this->msgbox->setBody(body);
            this->msgbox->setBodySize(body->w(), body->h());
            this->app->addOverlay(this->msgbox);
            this->optionPage->setValue("Enabled");
        } else {
            this->optionPage->setValue("Disabled");
        }
    }

    void Settings::setupSortOverlay() {
        // Remove previous entries
        this->sortOverlay->close(false);
        this->sortOverlay->removeEntries();

        // Add an entry for each sort method
        SortType t = this->app->config()->gSort();
        this->sortOverlay->addEntry("By Name", [this](){
            this->app->config()->setGSort(SortType::AlphaAsc);
            this->optionSort->setValue("By Name");
        }, t == SortType::AlphaAsc);
        this->sortOverlay->addEntry("By First Playtime", [this](){
            this->app->config()->setGSort(SortType::FirstPlayed);
            this->optionSort->setValue("By First Playtime");
        }, t == SortType::FirstPlayed);
        this->sortOverlay->addEntry("By Most Recently Played", [this](){
            this->app->config()->setGSort(SortType::LastPlayed);
            this->optionSort->setValue("By Most Recently Played");
        }, t == SortType::LastPlayed);
        this->sortOverlay->addEntry("By Most Playtime", [this](){
            this->app->config()->setGSort(SortType::HoursAsc);
            this->optionSort->setValue("By Most Playtime");
        }, t == SortType::HoursAsc);
        this->sortOverlay->addEntry("By Least Playtime", [this](){
            this->app->config()->setGSort(SortType::HoursDec);
            this->optionSort->setValue("By Least Playtime");
        }, t == SortType::HoursDec);
        this->sortOverlay->addEntry("By Most Launched", [this](){
            this->app->config()->setGSort(SortType::LaunchAsc);
            this->optionSort->setValue("By Most Launched");
        }, t == SortType::LaunchAsc);
        this->sortOverlay->addEntry("By Least Launched", [this](){
            this->app->config()->setGSort(SortType::LaunchDec);
            this->optionSort->setValue("By Least Launched");
        }, t == SortType::LaunchDec);

        this->app->addOverlay(this->sortOverlay);
    }

    void Settings::setupThemeOverlay() {
        // Remove previous entries
        this->themeOverlay->close(false);
        this->themeOverlay->removeEntries();

        // Add an entry for each sort method
        ThemeType t = this->app->config()->gTheme();
        this->themeOverlay->addEntry("Auto", [this](){
            this->app->config()->setGTheme(ThemeType::Auto);
            this->optionTheme->setValue("Auto");
        }, t == ThemeType::Auto);
        this->themeOverlay->addEntry("Basic Black", [this](){
            this->app->config()->setGTheme(ThemeType::Dark);
            this->optionTheme->setValue("Basic Black");
        }, t == ThemeType::Dark);
        this->themeOverlay->addEntry("Basic White", [this](){
            this->app->config()->setGTheme(ThemeType::Light);
            this->optionTheme->setValue("Basic White");
        }, t == ThemeType::Light);
        // this->themeOverlay->addEntry("Custom", [this](){
            // this->app->config()->setGTheme(ThemeType::Custom);
            // this->optionTheme->setValue("Custom");
        // }, t == ThemeType::Custom);

        this->app->addOverlay(this->themeOverlay);
    }

    void Settings::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, this->app->activeUser()->username() + "'s Activity", 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(this->app->theme()->text());
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
        delete this->msgbox;
        delete this->sortOverlay;
        delete this->themeOverlay;
    }
};
