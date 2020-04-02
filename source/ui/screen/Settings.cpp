#include <filesystem>
#include "Lang.hpp"
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
        c->addItem(new Aether::ControlItem(Aether::Button::A, "common.buttonHint.ok"_lang));
        if (!(this->app->isUserPage())) {
            c->addItem(new Aether::ControlItem(Aether::Button::B, "common.buttonHint.back"_lang));
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
                int bw, bh;
                this->msgbox->getBodySize(&bw, &bh);
                Aether::Element * body = new Aether::Element(0, 0, bw, bh);
                Aether::TextBlock * tb = new Aether::TextBlock(50, 40, "settings.other.replaceBox.error"_lang, 24, bw - 100);
                tb->setColour(this->app->theme()->text());
                body->addElement(tb);
                tb = new Aether::TextBlock(50, tb->y() + tb->h() + 20, "settings.other.replaceBox.errorMsg1"_lang + "\n/switch/NX-Activity-Log/NX-Activity-Log.nro\n\n" + "settings.other.replaceBox.errorMsg2"_lang + "\n" + "settings.other.replaceBox.errorMsg3"_lang, 20, body->w() - 100);
                tb->setColour(this->app->theme()->mutedText());
                body->addElement(tb);
                this->msgbox->setBodySize(bw, tb->y() + tb->h() + 40);
                this->msgbox->setBody(body);
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
            int bw, bh;
            this->msgbox->getBodySize(&bw, &bh);
            Aether::Element * body = new Aether::Element(0, 0, bw, bh);
            Aether::TextBlock * tb = new Aether::TextBlock(50, 40, "settings.other.replaceBox.success"_lang, 24, bw - 100);
            tb->setColour(this->app->theme()->text());
            body->addElement(tb);
            std::string str = std::string(atms ? "common.atmosphere"_lang + "  " : "") + std::string(rei ? "common.reinx"_lang + "  " : "") + std::string(sx ? "common.sxos"_lang : "");
            tb = new Aether::TextBlock(50, tb->y() + tb->h() + 20, "settings.other.replaceBox.successMsg1"_lang + "\n" + str + "\n\n" + "settings.other.replaceBox.successMsg2"_lang + "\n" + "settings.other.replaceBox.successMsg3"_lang, 20, bw - 100);
            tb->setColour(this->app->theme()->mutedText());
            body->addElement(tb);
            this->msgbox->setBodySize(bw, tb->y() + tb->h() + 40);
            this->msgbox->setBody(body);
            this->app->addOverlay(this->msgbox);
            this->optionPage->setValue("common.enabled"_lang);
        } else {
            this->optionPage->setValue("common.disabled"_lang);
        }
    }

    void Settings::preparePopupList(std::string s) {
        // Remove previous overlay
        if (this->popuplist != nullptr) {
            delete this->popuplist;
        }

        this->popuplist = new Aether::PopupList(s);
        this->popuplist->setBackLabel("common.buttonHint.back"_lang);
        this->popuplist->setOKLabel("common.buttonHint.ok"_lang);
        this->popuplist->setBackgroundColour(this->app->theme()->altBG());
        this->popuplist->setHighlightColour(this->app->theme()->accent());
        this->popuplist->setLineColour(this->app->theme()->fg());
        this->popuplist->setListLineColour(this->app->theme()->mutedLine());
        this->popuplist->setTextColour(this->app->theme()->text());
    }

    void Settings::setupLangOverlay() {
        this->preparePopupList("settings.launch.language"_lang);

        // Add an entry for each language
        Language l = this->app->config()->gLang();
        this->popuplist->addEntry("settings.launch.default"_lang, [this, l](){
            if (l != Language::Default) {
                this->app->config()->setGLang(Language::Default);
                Utils::Lang::setLanguage(Language::Default);
                this->app->reinitScreens();
            }
        }, l == Language::Default);
        this->popuplist->addEntry("English", [this, l](){
            if (l != Language::English) {
                this->app->config()->setGLang(Language::English);
                Utils::Lang::setLanguage(Language::English);
                this->app->reinitScreens();
            }
        }, l == Language::English);
        this->popuplist->addEntry("Français", [this, l](){
            if (l != Language::French) {
                this->app->config()->setGLang(Language::French);
                Utils::Lang::setLanguage(Language::French);
                this->app->reinitScreens();
            }
        }, l == Language::French);
        this->popuplist->addEntry("Deutsch", [this, l](){
            if (l != Language::German) {
                this->app->config()->setGLang(Language::German);
                Utils::Lang::setLanguage(Language::German);
                this->app->reinitScreens();
            }
        }, l == Language::German);
        this->popuplist->addEntry("Italiano", [this, l](){
            if (l != Language::Italian) {
                this->app->config()->setGLang(Language::Italian);
                Utils::Lang::setLanguage(Language::Italian);
                this->app->reinitScreens();
            }
        }, l == Language::Italian);
        this->popuplist->addEntry("Português", [this, l](){
            if (l != Language::Portugese) {
                this->app->config()->setGLang(Language::Portugese);
                Utils::Lang::setLanguage(Language::Portugese);
                this->app->reinitScreens();
            }
        }, l == Language::Portugese);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::setupScreenOverlay() {
        this->preparePopupList("settings.launch.screen"_lang);

        // Add an entry for each sort method
        ScreenID s = this->app->config()->lScreen();
        this->popuplist->addEntry("common.screen.recentActivity"_lang, [this](){
            this->app->config()->setLScreen(ScreenID::RecentActivity);
            this->optionScreen->setValue("common.screen.recentActivity"_lang);
        }, s == ScreenID::RecentActivity);
        this->popuplist->addEntry("common.screen.allActivity"_lang, [this](){
            this->app->config()->setLScreen(ScreenID::AllActivity);
            this->optionScreen->setValue("common.screen.allActivity"_lang);
        }, s == ScreenID::AllActivity);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::setupSortOverlay() {
        this->preparePopupList("settings.launch.sort"_lang);

        // Add an entry for each sort method
        SortType t = this->app->config()->lSort();
        this->popuplist->addEntry("common.sort.name"_lang, [this](){
            this->app->config()->setLSort(SortType::AlphaAsc);
            this->optionSort->setValue("common.sort.name"_lang);
        }, t == SortType::AlphaAsc);
        this->popuplist->addEntry("common.sort.firstPlayed"_lang, [this](){
            this->app->config()->setLSort(SortType::FirstPlayed);
            this->optionSort->setValue("common.sort.firstPlayed"_lang);
        }, t == SortType::FirstPlayed);
        this->popuplist->addEntry("common.sort.recentlyPlayed"_lang, [this](){
            this->app->config()->setLSort(SortType::LastPlayed);
            this->optionSort->setValue("common.sort.recentlyPlayed"_lang);
        }, t == SortType::LastPlayed);
        this->popuplist->addEntry("common.sort.mostPlaytime"_lang, [this](){
            this->app->config()->setLSort(SortType::HoursAsc);
            this->optionSort->setValue("common.sort.mostPlaytime"_lang);
        }, t == SortType::HoursAsc);
        this->popuplist->addEntry("common.sort.leastPlaytime"_lang, [this](){
            this->app->config()->setLSort(SortType::HoursDec);
            this->optionSort->setValue("common.sort.leastPlaytime"_lang);
        }, t == SortType::HoursDec);
        this->popuplist->addEntry("common.sort.mostLaunched"_lang, [this](){
            this->app->config()->setLSort(SortType::LaunchAsc);
            this->optionSort->setValue("common.sort.mostLaunched"_lang);
        }, t == SortType::LaunchAsc);
        this->popuplist->addEntry("common.sort.leastLaunched"_lang, [this](){
            this->app->config()->setLSort(SortType::LaunchDec);
            this->optionSort->setValue("common.sort.leastLaunched"_lang);
        }, t == SortType::LaunchDec);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::setupThemeOverlay() {
        this->preparePopupList("settings.launch.theme.heading"_lang);

        // Add an entry for each sort method
        ThemeType t = this->app->config()->gTheme();
        this->popuplist->addEntry("settings.launch.theme.auto"_lang, [this, t](){
            if (t != ThemeType::Auto) {
                this->app->config()->setGTheme(ThemeType::Auto);
                this->app->theme()->setTheme(ThemeType::Auto);
                this->app->reinitScreens();
            }
        }, t == ThemeType::Auto);
        this->popuplist->addEntry("settings.launch.theme.dark"_lang, [this, t](){
            if (t != ThemeType::Dark) {
                this->app->config()->setGTheme(ThemeType::Dark);
                this->app->theme()->setTheme(ThemeType::Dark);
                this->app->reinitScreens();
            }
        }, t == ThemeType::Dark);
        this->popuplist->addEntry("settings.launch.theme.light"_lang, [this, t](){
            if (t != ThemeType::Light) {
                this->app->config()->setGTheme(ThemeType::Light);
                this->app->theme()->setTheme(ThemeType::Light);
                this->app->reinitScreens();
            }
        }, t == ThemeType::Light);
        // this->popuplist->addEntry("Custom", [this](){
            // this->app->config()->setGTheme(ThemeType::Custom);
            // this->optionTheme->setValue("Custom");
        // }, t == ThemeType::Custom);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::setupViewOverlay() {
        this->preparePopupList("settings.launch.view"_lang);

        // Add an entry for each sort method
        ViewPeriod v = this->app->config()->lView();
        this->popuplist->addEntry("common.view.day"_lang, [this](){
            this->app->config()->setLView(ViewPeriod::Day);
            this->optionView->setValue("common.view.day"_lang);
        }, v == ViewPeriod::Day);
        this->popuplist->addEntry("common.view.month"_lang, [this](){
            this->app->config()->setLView(ViewPeriod::Month);
            this->optionView->setValue("common.view.month"_lang);
        }, v == ViewPeriod::Month);
        this->popuplist->addEntry("common.view.year"_lang, [this](){
            this->app->config()->setLView(ViewPeriod::Year);
            this->optionView->setValue("common.view.year"_lang);
        }, v == ViewPeriod::Year);

        this->app->addOverlay(this->popuplist);
    }

    void Settings::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, Utils::formatHeading(this->app->activeUser()->username()), 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(this->app->theme()->text());
        this->addElement(this->heading);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize(), 4, 4);
        this->image->setWH(60, 60);
        this->addElement(this->image);

        // Create side menu
        this->menu = new Aether::Menu(30, 88, 388, 559);
        this->menu->addElement(new Aether::MenuOption("common.screen.recentActivity"_lang, this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(ScreenID::RecentActivity);
        }));
        this->menu->addElement(new Aether::MenuOption("common.screen.allActivity"_lang, this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(ScreenID::AllActivity);
        }));
        this->menu->addElement(new Aether::MenuSeparator(this->app->theme()->mutedLine()));
        Aether::MenuOption * opt = new Aether::MenuOption("common.screen.settings"_lang, this->app->theme()->accent(), this->app->theme()->text(), nullptr);
        this->menu->addElement(opt);
        this->menu->setActiveOption(opt);
        this->menu->setFocussed(opt);
        this->addElement(this->menu);

        // Create list
        this->list = new Aether::List(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // ===== UPDATE =====
        Aether::ListButton * lb = new Aether::ListButton("settings.checkUpdates"_lang, [this]() {
            this->app->pushScreen();
            this->app->setScreen(ScreenID::Update);
        });
        lb->setLineColour(this->app->theme()->mutedLine());
        lb->setTextColour(this->app->theme()->text());
        this->list->addElement(lb);

        // ===== LAUNCH OPTIONS =====
        Aether::ListHeading * lh = new Aether::ListHeading("settings.launch.heading"_lang);
        lh->setRectColour(this->app->theme()->mutedLine());
        lh->setTextColour(this->app->theme()->text());
        this->list->addElement(lh);

        this->list->addElement(new Aether::ListSeparator(20));

        // LAUNCH SCREEN
        std::string str = "";
        switch (this->app->config()->lScreen()) {
            case ScreenID::RecentActivity:
                str = "common.screen.recentActivity"_lang;
                break;

            case ScreenID::AllActivity:
                str = "common.screen.allActivity"_lang;
                break;

            default:
                // Never called but I don't like compiler warnings
                break;
        }
        this->optionScreen = new Aether::ListOption("settings.launch.screen"_lang, str, [this]() {
            this->setupScreenOverlay();
        });
        this->optionScreen->setHintColour(this->app->theme()->text());
        this->optionScreen->setValueColour(this->app->theme()->accent());
        this->optionScreen->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionScreen);
        Aether::ListComment * lc = new Aether::ListComment("settings.launch.screenHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // DEFAULT SORT METHOD
        switch (this->app->config()->lSort()) {
            case SortType::AlphaAsc:
                str = "common.sort.name"_lang;
                break;

            case SortType::HoursAsc:
                str = "common.sort.mostPlaytime"_lang;
                break;

            case SortType::HoursDec:
                str = "common.sort.leastPlaytime"_lang;
                break;

            case SortType::LaunchAsc:
                str = "common.sort.mostLaunched"_lang;
                break;

            case SortType::LaunchDec:
                str = "common.sort.leastLaunched"_lang;
                break;

            case SortType::FirstPlayed:
                str = "common.sort.firstPlaytime"_lang;
                break;

            case SortType::LastPlayed:
                str = "common.sort.recentlyPlayed"_lang;
                break;
        }
        this->optionSort = new Aether::ListOption("settings.launch.sort"_lang, str, [this](){
            this->setupSortOverlay();
        });
        this->optionSort->setHintColour(this->app->theme()->text());
        this->optionSort->setValueColour(this->app->theme()->accent());
        this->optionSort->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionSort);
        lc = new Aether::ListComment("settings.launch.sortHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // VIEW
        switch (this->app->config()->lView()) {
            case ViewPeriod::Day:
                str = "common.view.day"_lang;
                break;

            case ViewPeriod::Month:
                str = "common.view.month"_lang;
                break;

            case ViewPeriod::Year:
                str = "common.view.year"_lang;
                break;
        }
        this->optionView = new Aether::ListOption("settings.launch.view"_lang, str, [this](){
            this->setupViewOverlay();
        });
        this->optionView->setHintColour(this->app->theme()->text());
        this->optionView->setValueColour(this->app->theme()->accent());
        this->optionView->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionView);
        lc = new Aether::ListComment("settings.launch.viewHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // LANGUAGE
        switch (this->app->config()->gLang()) {
            case Language::Default:
                str = "settings.launch.default"_lang;
                break;

            case Language::English:
                str = "English";
                break;

            case Language::French:
                str = "Français";
                break;

            case Language::German:
                str = "Deutsch";
                break;

            case Language::Italian:
                str = "Italiano";
                break;

            case Language::Portugese:
                str = "Português";
                break;
        }
        this->optionLang = new Aether::ListOption("settings.launch.language"_lang, str, [this](){
            this->setupLangOverlay();
        });
        this->optionLang->setHintColour(this->app->theme()->text());
        this->optionLang->setValueColour(this->app->theme()->accent());
        this->optionLang->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionLang);
        lc = new Aether::ListComment("settings.launch.languageHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // THEME
        switch (this->app->config()->gTheme()) {
            case Auto:
                str = "settings.launch.theme.auto"_lang;
                break;

            case Custom:
                str = "settings.launch.theme.custom"_lang;
                break;

            case Dark:
                str = "settings.launch.theme.dark"_lang;
                break;

            case Light:
                str = "settings.launch.theme.light"_lang;
                break;
        }
        this->optionTheme = new Aether::ListOption("settings.launch.theme.heading"_lang, str, [this](){
            this->setupThemeOverlay();
        });
        this->optionTheme->setHintColour(this->app->theme()->text());
        this->optionTheme->setValueColour(this->app->theme()->accent());
        this->optionTheme->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionTheme);
        lc = new Aether::ListComment("settings.launch.themeHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        this->list->addElement(new Aether::ListSeparator());

        // ===== OTHERS =====
        lh = new Aether::ListHeading("settings.other.heading"_lang);
        lh->setRectColour(this->app->theme()->mutedLine());
        lh->setTextColour(this->app->theme()->text());
        this->list->addElement(lh);

        this->list->addElement(new Aether::ListSeparator(20));

        // GRAPH
        this->optionGraph = new Aether::ListOption("settings.other.graph"_lang, (this->app->config()->gGraph() ? "common.on"_lang : "common.off"_lang), [this](){
            this->app->config()->setGGraph(!this->app->config()->gGraph());
            this->optionGraph->setValue((this->app->config()->gGraph() ? "common.on"_lang : "common.off"_lang));
            this->optionGraph->setValueColour((this->app->config()->gGraph() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        });
        this->optionGraph->setHintColour(this->app->theme()->text());
        this->optionGraph->setValueColour((this->app->config()->gGraph() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        this->optionGraph->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionGraph);
        lc = new Aether::ListComment("settings.other.graphHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // HIDE DELETED
        str = (this->app->config()->hDeleted() ? "common.yes"_lang : "common.no"_lang);
        this->optionDeleted = new Aether::ListOption("settings.other.hideDeleted"_lang, str, [this](){
            this->app->config()->setHDeleted(!this->app->config()->hDeleted());
            this->optionDeleted->setValue((this->app->config()->hDeleted() ? "common.yes"_lang : "common.no"_lang));
            this->optionDeleted->setValueColour((this->app->config()->hDeleted() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        });
        this->optionDeleted->setHintColour(this->app->theme()->text());
        this->optionDeleted->setValueColour((this->app->config()->hDeleted() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        this->optionDeleted->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionDeleted);
        lc = new Aether::ListComment("settings.other.hideDeletedHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // REPLACE USER PAGE
        str = "common.disabled"_lang;
        if (std::filesystem::exists("/atmosphere/contents/0100000000001013/exefs.nsp") || std::filesystem::exists("/ReiNX/titles/0100000000001013/exefs.nsp") || std::filesystem::exists("/sxos/titles/0100000000001013/exefs.nsp")) {
            str = "common.enabled"_lang;
        }
        this->optionPage = new Aether::ListOption("settings.other.replace"_lang, str, [this](){
            this->installForwarder();
        });
        this->optionPage->setHintColour(this->app->theme()->text());
        this->optionPage->setValueColour(this->app->theme()->accent());
        this->optionPage->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->optionPage);

        lc = new Aether::ListComment("settings.other.replaceHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        // 24H
        this->option24H = new Aether::ListOption("settings.other.24H"_lang, (this->app->config()->gIs24H() ? "common.on"_lang : "common.off"_lang), [this](){
            this->app->config()->setGIs24H(!this->app->config()->gIs24H());
            this->option24H->setValue((this->app->config()->gIs24H() ? "common.on"_lang : "common.off"_lang));
            this->option24H->setValueColour((this->app->config()->gIs24H() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        });
        this->option24H->setHintColour(this->app->theme()->text());
        this->option24H->setValueColour((this->app->config()->gIs24H() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        this->option24H->setLineColour(this->app->theme()->mutedLine());
        this->list->addElement(this->option24H);
        lc = new Aether::ListComment("settings.other.24HHint"_lang);
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        this->list->addElement(new Aether::ListSeparator());

        // INFORMATION
        lc = new Aether::ListComment("settings.translations"_lang + "\nAyk\nevertonstz\nReinzanini\nxRock");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);
        lc = new Aether::ListComment("NX Activity Log v" + std::string(VER_STRING) + "\n" + "settings.about"_lang + "\n\n" + "settings.support"_lang + "\nhttps://ko-fi.com/tallbl0nde");
        lc->setTextColour(this->app->theme()->mutedText());
        this->list->addElement(lc);

        this->addElement(this->list);

        // Create base message box
        this->msgbox = new Aether::MessageBox();
        this->msgbox->addTopButton("common.buttonHint.ok"_lang, [this](){
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
