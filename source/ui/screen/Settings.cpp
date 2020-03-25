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
        if (!(this->app->isUserPage())) {
            c->addItem(new Aether::ControlItem(Aether::Button::B, "Back"));
        }
        c->setColour(this->app->theme()->text());
        this->addElement(c);

        // Add button callbacks
        if (!(this->app->isUserPage())) {
            this->onButtonPress(Aether::Button::B, [this](){
                this->app->setScreen(ScreenID::UserSelect);
            });
        }
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

    void Settings::preparePopupList(std::string s) {
        // Remove previous overlay
        if (this->popuplist != nullptr) {
            delete this->popuplist;
        }

        this->popuplist = new Aether::PopupList(s);
        this->popuplist->setBackgroundColour(this->app->theme()->altBG());
        this->popuplist->setHighlightColour(this->app->theme()->accent());
        this->popuplist->setLineColour(this->app->theme()->fg());
        this->popuplist->setListLineColour(this->app->theme()->mutedLine());
        this->popuplist->setTextColour(this->app->theme()->text());
    }

    void Settings::setupScreenOverlay() {
        this->preparePopupList("Default Screen");

        // Add an entry for each sort method
        ScreenID s = this->app->config()->lScreen();
        this->popuplist->addEntry("Recent Activity", [this](){
            this->app->config()->setLScreen(ScreenID::RecentActivity);
            this->optionScreen->setValue("Recent Activity");
        }, s == ScreenID::RecentActivity);
        this->popuplist->addEntry("All Activity", [this](){
            this->app->config()->setLScreen(ScreenID::AllActivity);
            this->optionScreen->setValue("All Activity");
        }, s == ScreenID::AllActivity);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::setupSortOverlay() {
        this->preparePopupList("Default Sort Method");

        // Add an entry for each sort method
        SortType t = this->app->config()->lSort();
        this->popuplist->addEntry("By Name", [this](){
            this->app->config()->setLSort(SortType::AlphaAsc);
            this->optionSort->setValue("By Name");
        }, t == SortType::AlphaAsc);
        this->popuplist->addEntry("By First Playtime", [this](){
            this->app->config()->setLSort(SortType::FirstPlayed);
            this->optionSort->setValue("By First Playtime");
        }, t == SortType::FirstPlayed);
        this->popuplist->addEntry("By Most Recently Played", [this](){
            this->app->config()->setLSort(SortType::LastPlayed);
            this->optionSort->setValue("By Most Recently Played");
        }, t == SortType::LastPlayed);
        this->popuplist->addEntry("By Most Playtime", [this](){
            this->app->config()->setLSort(SortType::HoursAsc);
            this->optionSort->setValue("By Most Playtime");
        }, t == SortType::HoursAsc);
        this->popuplist->addEntry("By Least Playtime", [this](){
            this->app->config()->setLSort(SortType::HoursDec);
            this->optionSort->setValue("By Least Playtime");
        }, t == SortType::HoursDec);
        this->popuplist->addEntry("By Most Launched", [this](){
            this->app->config()->setLSort(SortType::LaunchAsc);
            this->optionSort->setValue("By Most Launched");
        }, t == SortType::LaunchAsc);
        this->popuplist->addEntry("By Least Launched", [this](){
            this->app->config()->setLSort(SortType::LaunchDec);
            this->optionSort->setValue("By Least Launched");
        }, t == SortType::LaunchDec);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::setupThemeOverlay() {
        this->preparePopupList("Application Theme");

        // Add an entry for each sort method
        ThemeType t = this->app->config()->gTheme();
        this->popuplist->addEntry("Auto", [this](){
            this->app->config()->setGTheme(ThemeType::Auto);
            this->optionTheme->setValue("Auto");
        }, t == ThemeType::Auto);
        this->popuplist->addEntry("Basic Black", [this](){
            this->app->config()->setGTheme(ThemeType::Dark);
            this->optionTheme->setValue("Basic Black");
        }, t == ThemeType::Dark);
        this->popuplist->addEntry("Basic White", [this](){
            this->app->config()->setGTheme(ThemeType::Light);
            this->optionTheme->setValue("Basic White");
        }, t == ThemeType::Light);
        // this->popuplist->addEntry("Custom", [this](){
            // this->app->config()->setGTheme(ThemeType::Custom);
            // this->optionTheme->setValue("Custom");
        // }, t == ThemeType::Custom);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::setupViewOverlay() {
        this->preparePopupList("Default View Type");

        // Add an entry for each sort method
        ViewPeriod v = this->app->config()->lView();
        this->popuplist->addEntry("By Day", [this](){
            this->app->config()->setLView(ViewPeriod::Day);
            this->optionView->setValue("By Day");
        }, v == ViewPeriod::Day);
        this->popuplist->addEntry("By Month", [this](){
            this->app->config()->setLView(ViewPeriod::Month);
            this->optionView->setValue("By Month");
        }, v == ViewPeriod::Month);
        this->popuplist->addEntry("By Year", [this](){
            this->app->config()->setLView(ViewPeriod::Year);
            this->optionView->setValue("By Year");
        }, v == ViewPeriod::Year);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, this->app->activeUser()->username() + "'s Activity", 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(this->app->theme()->text());
        this->addElement(this->heading);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize(), 4, 4);
        this->image->setWH(60, 60);
        this->addElement(this->image);

        // Create side menu
        this->menu = new Aether::Menu(30, 88, 388, 559);
        this->menu->addElement(new Aether::MenuOption("Recent Activity", this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(ScreenID::RecentActivity);
        }));
        this->menu->addElement(new Aether::MenuOption("All Activity", this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(ScreenID::AllActivity);
        }));
        this->menu->addElement(new Aether::MenuSeparator(this->app->theme()->mutedLine()));
        Aether::MenuOption * opt = new Aether::MenuOption("Settings", this->app->theme()->accent(), this->app->theme()->text(), nullptr);
        this->menu->addElement(opt);
        this->menu->setActiveOption(opt);
        this->menu->setFocussed(opt);
        this->addElement(this->menu);

        // Create list
        this->list = new Aether::List(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // ===== LAUNCH OPTIONS =====
        Aether::ListHeading * lh = new Aether::ListHeading("Launch Options");
        lh->setRectColour(this->app->theme()->mutedLine());
        lh->setTextColour(this->app->theme()->text());
        this->list->addElement(lh);

        this->list->addElement(new Aether::ListSeparator(20));

        // LAUNCH SCREEN
        std::string str = "";
        switch (this->app->config()->lScreen()) {
            case ScreenID::RecentActivity:
                str = "Recent Activity";
                break;

            case ScreenID::AllActivity:
                str = "All Activity";
                break;

            default:
                // Never called but I don't like compiler warnings
                break;
        }
        this->optionScreen = new Aether::ListOption("Default Screen", str, [this]() {
            this->setupScreenOverlay();
        });
        this->optionScreen->setHintColour(this->app->theme()->text());
        this->optionScreen->setValueColour(this->app->theme()->accent());
        this->optionScreen->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionScreen);
        Aether::ListComment * lc = new Aether::ListComment("The screen to show on launch/after selecting a user.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // DEFAULT SORT METHOD
        switch (this->app->config()->lSort()) {
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
        lc = new Aether::ListComment("The sort method for 'All Activity' to use on launch.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);
        
        // VIEW
        switch (this->app->config()->lView()) {
            case ViewPeriod::Day:
                str = "By Day";
                break;

            case ViewPeriod::Month:
                str = "By Month";
                break;

            case ViewPeriod::Year:
                str = "By Year";
                break;
        }
        this->optionView = new Aether::ListOption("Default View Type", str, [this](){
            this->setupViewOverlay();
        });
        this->optionView->setHintColour(this->app->theme()->text());
        this->optionView->setValueColour(this->app->theme()->accent());
        this->optionView->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionView);
        lc = new Aether::ListComment("The period of activity to view on launch.");
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
        lc = new Aether::ListComment("The theme to use for the application. Auto chooses black/white based on your Switch settings. Note: This currently requires the app to be restarted to take effect.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        this->list->addElement(new Aether::ListSeparator());

        // ===== OTHERS =====
        lh = new Aether::ListHeading("Other Options");
        lh->setRectColour(this->app->theme()->mutedLine());
        lh->setTextColour(this->app->theme()->text());
        this->list->addElement(lh);

        this->list->addElement(new Aether::ListSeparator(20));

        // GRAPH
        this->optionGraph = new Aether::ListOption("Graph Bar Labels", (this->app->config()->gGraph() ? "On" : "Off"), [this](){
            this->app->config()->setGGraph(!this->app->config()->gGraph());
            this->optionGraph->setValue((this->app->config()->gGraph() ? "On" : "Off"));
            this->optionGraph->setValueColour((this->app->config()->gGraph() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        });
        this->optionGraph->setHintColour(this->app->theme()->text());
        this->optionGraph->setValueColour((this->app->config()->gGraph() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        this->optionGraph->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionGraph);
        lc = new Aether::ListComment("Toggles the small numbers visible above each bar on graphs.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // HIDE DELETED
        str = (this->app->config()->hDeleted() ? "Yes" : "No");
        this->optionDeleted = new Aether::ListOption("Hide Deleted Games", str, [this](){
            this->app->config()->setHDeleted(!this->app->config()->hDeleted());
            this->optionDeleted->setValue((this->app->config()->hDeleted() ? "Yes" : "No"));
            this->optionDeleted->setValueColour((this->app->config()->hDeleted() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        });
        this->optionDeleted->setHintColour(this->app->theme()->text());
        this->optionDeleted->setValueColour((this->app->config()->hDeleted() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        this->optionDeleted->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionDeleted);
        lc = new Aether::ListComment("Excludes deleted games from 'All Activity'.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

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

        lc = new Aether::ListComment("Replace the User Page with this app. Requires LayeredFS and either Atmosphere 0.10.0+, ReiNX or SXOS.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // 24H
        this->option24H = new Aether::ListOption("Use 24 Hour Time", (this->app->config()->gIs24H() ? "On" : "Off"), [this](){
            this->app->config()->setGIs24H(!this->app->config()->gIs24H());
            this->option24H->setValue((this->app->config()->gIs24H() ? "On" : "Off"));
            this->option24H->setValueColour((this->app->config()->gIs24H() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        });
        this->option24H->setHintColour(this->app->theme()->text());
        this->option24H->setValueColour((this->app->config()->gIs24H() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        this->option24H->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->option24H);
        lc = new Aether::ListComment("Use 24 hour time instead of 12 hour where applicable.");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        this->list->addElement(new Aether::ListSeparator());

        // INFORMATION
        lc = new Aether::ListComment("NX Activity Log v" + std::string(VER_STRING) + "\nThanks for using my app! I hope it's been useful! :)\n\nYou can support me on Ko-fi:\nhttps://ko-fi.com/tallbl0nde");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        this->addElement(this->list);

        // Create base message box
        this->msgbox = new Aether::MessageBox();
        this->msgbox->addTopButton("OK", [this](){
            this->msgbox->close(true);
        });
        this->msgbox->setLineColour(this->app->theme()->mutedLine());
        this->msgbox->setRectangleColour(this->app->theme()->altBG());
        this->msgbox->setTextColour(this->app->theme()->accent());

        this->popuplist = nullptr;
    }

    void Settings::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->image);
        this->removeElement(this->list);
        this->removeElement(this->menu);
        delete this->msgbox;
        delete this->popuplist;
    }
};
