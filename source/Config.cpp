#include "Config.hpp"
#include <filesystem>

namespace Main {
    Config::Config() {
        this->ini = nullptr;
    }

    Config::~Config() {
        delete this->ini;
    }

    void Config::readConfig() {
        // Check if file exists, and if not create directories
        if (!std::filesystem::exists("/config/NX-Activity-Log/config.ini")) {
            std::filesystem::create_directory("/config");
            std::filesystem::create_directory("/config/NX-Activity-Log");
        }

        // Read (or create) file
        this->ini = simpleIniParser::Ini::parseOrCreateFile("/config/NX-Activity-Log/config.ini");
        simpleIniParser::IniSection * sec = ini->findOrCreateSection("general");
        simpleIniParser::IniOption * option = sec->findOrCreateFirstOption("language", "Default");
        if (option->value == "English") {
            this->gLang_ = English;
        } else if (option->value == "French") {
            this->gLang_ = French;
        } else if (option->value == "German") {
            this->gLang_ = German;
        } else if (option->value == "Italian") {
            this->gLang_ = Italian;
        } else if (option->value == "Portugese") {
            this->gLang_ = Portugese;
        } else if (option->value == "Russian") {
            this->gLang_ = Russian;
        } else if (option->value == "Spanish") {
            this->gLang_ = Spanish;
        } else if (option->value == "Turkish") {
            this->gLang_ = Turkish;
        } else if (option->value == "Korean") {
            this->gLang_ = Korean;
        } else {
            this->gLang_ = Default;
        }

        sec->findOrCreateFirstOption("showGraphValues", "true");
        if (option->value == "false") {
            this->gGraph_ = false;
        } else {
            this->gGraph_ = true;
        }

        option = sec->findOrCreateFirstOption("use24h", "false");
        if (option->value == "true") {
            this->gIs24H_ = true;
        } else {
            this->gIs24H_ = false;
        }

        option = sec->findOrCreateFirstOption("theme", "Auto");
        if (option->value == "Custom") {
            this->gTheme_ = Custom;
        } else if (option->value == "Light") {
            this->gTheme_ = Light;
        } else if (option->value == "Dark") {
            this->gTheme_ = Dark;
        } else {
            this->gTheme_ = Auto;
        }

        option = sec->findOrCreateFirstOption("screen", "RecentActivity");
        if (option->value == "AllActivity") {
            this->lScreen_ = ScreenID::AllActivity;
        } else {
            this->lScreen_ = ScreenID::RecentActivity;
        }

        option = sec->findOrCreateFirstOption("sort", "LastPlayed");
        if (option->value == "AlphaAsc") {
            this->lSort_ = AlphaAsc;
        } else if (option->value == "HoursAsc") {
            this->lSort_ = HoursAsc;
        } else if (option->value == "HoursDec") {
            this->lSort_ = HoursDec;
        } else if (option->value == "LaunchAsc") {
            this->lSort_ = LaunchAsc;
        } else if (option->value == "LaunchDec") {
            this->lSort_ = LaunchDec;
        } else if (option->value == "FirstPlayed") {
            this->lSort_ = FirstPlayed;
        } else {
            this->lSort_ = LastPlayed;
        }

        option = sec->findOrCreateFirstOption("view", "Day");
        if (option->value == "Month") {
            this->lView_ = ViewPeriod::Month;
        } else if (option->value == "Year") {
            this->lView_ = ViewPeriod::Year;
        } else {
            this->lView_ = ViewPeriod::Day;
        }

        sec = ini->findOrCreateSection("hidden");
        option = sec->findOrCreateFirstOption("deleted", "false");
        if (option->value == "true") {
            this->hDeleted_ = true;
        } else {
            this->hDeleted_ = false;
        }

        sec = ini->findOrCreateSection("theme");
        option = sec->findOrCreateFirstOption("image", "false");
        if (option->value == "true") {
            this->tImage_ = true;
        } else {
            this->tImage_ = false;
        }
    }

    void Config::writeConfig() {
        if (this->ini == nullptr) {
            return;
        }

        // Write file
        simpleIniParser::IniOption * option = ini->findSection("general")->findFirstOption("language");
        if (this->gLang_ == Default) {
            option->value = "Default";
        } else if (this->gLang_ == English) {
            option->value = "English";
        } else if (this->gLang_ == French) {
            option->value = "French";
        } else if (this->gLang_ == German) {
            option->value = "German";
        } else if (this->gLang_ == Italian) {
            option->value = "Italian";
        } else if (this->gLang_ == Portugese) {
            option->value = "Portugese";
        } else if (this->gLang_ == Russian) {
            option->value = "Russian";
        } else if (this->gLang_ == Spanish) {
            option->value = "Spanish";
        } else if (this->gLang_ == Turkish) {
            option->value = "Turkish";
        } else if (this->gLang_ == Korean) {
            option->value = "Korean";
        }

        option = ini->findSection("general")->findFirstOption("showGraphValues");
        if (this->gGraph_ == true) {
            option->value = "true";
        } else if (this->gGraph_ == false) {
            option->value = "false";
        }

        option = ini->findSection("general")->findFirstOption("use24h");
        if (this->gIs24H_ == true) {
            option->value = "true";
        } else if (this->gIs24H_ == false) {
            option->value = "false";
        }

        option = ini->findSection("general")->findFirstOption("theme");
        if (this->gTheme_ == Light) {
            option->value = "Light";
        } else if (this->gTheme_ == Dark) {
            option->value = "Dark";
        } else if (this->gTheme_ == Auto) {
            option->value = "Auto";
        } else if (this->gTheme_ == Custom) {
            option->value = "Custom";
        }

        option = ini->findSection("hidden")->findFirstOption("deleted");
        if (this->hDeleted_ == true) {
            option->value = "true";
        } else if (this->hDeleted_ == false) {
            option->value = "false";
        }

        option = ini->findSection("general")->findFirstOption("screen");
        if (this->lScreen_ == ScreenID::AllActivity) {
            option->value = "AllActivity";
        } else if (this->lScreen_ == ScreenID::RecentActivity) {
            option->value = "RecentActivity";
        }

        option = ini->findSection("general")->findFirstOption("sort");
        if (this->lSort_ == AlphaAsc) {
            option->value = "AlphaAsc";
        } else if (this->lSort_ == HoursAsc) {
            option->value = "HoursAsc";
        } else if (this->lSort_ == HoursDec) {
            option->value = "HoursDec";
        } else if (this->lSort_ == LaunchAsc) {
            option->value = "LaunchAsc";
        } else if (this->lSort_ == LaunchDec) {
            option->value = "LaunchDec";
        } else if (this->lSort_ == FirstPlayed) {
            option->value = "FirstPlayed";
        } else if (this->lSort_ == LastPlayed) {
            option->value = "LastPlayed";
        }

        option = ini->findSection("general")->findFirstOption("view");
        if (this->lView_ == ViewPeriod::Month) {
            option->value = "Month";
        } else if (this->lView_ == ViewPeriod::Year) {
            option->value = "Year";
        } else if (this->lView_ == ViewPeriod::Day) {
            option->value = "Day";
        }

        option = ini->findSection("theme")->findFirstOption("image");
        if (this->tImage_ == true) {
            option->value = "true";
        } else if (this->tImage_ == false) {
            option->value = "false";
        }

        ini->writeToFile("/config/NX-Activity-Log/config.ini");
    }

    bool Config::gGraph() {
        return this->gGraph_;
    }

    bool Config::gIs24H() {
        return this->gIs24H_;
    }

    Language Config::gLang() {
        return this->gLang_;
    }

    ThemeType Config::gTheme() {
        return this->gTheme_;
    }

    bool Config::hDeleted() {
        return this->hDeleted_;
    }

    ScreenID Config::lScreen() {
        return this->lScreen_;
    }

    SortType Config::lSort() {
        return this->lSort_;
    }

    ViewPeriod Config::lView() {
        return this->lView_;
    }

    bool Config::tImage() {
        return this->tImage_;
    }

    void Config::setGGraph(bool b) {
        this->gGraph_ = b;
        this->writeConfig();
    }

    void Config::setGIs24H(bool b) {
        this->gIs24H_ = b;
        this->writeConfig();
    }

    void Config::setGLang(Language l) {
        this->gLang_ = l;
        this->writeConfig();
    }

    void Config::setGTheme(ThemeType v) {
        this->gTheme_ = v;
        this->writeConfig();
    }

    void Config::setHDeleted(bool v) {
        this->hDeleted_ = v;
        this->writeConfig();
    }

    void Config::setLScreen(ScreenID v) {
        this->lScreen_ = v;
        this->writeConfig();
    }

    void Config::setLSort(SortType v) {
        this->lSort_ = v;
        this->writeConfig();
    }

    void Config::setLView(ViewPeriod v) {
        this->lView_ = v;
        this->writeConfig();
    }

    void Config::setTImage(bool b) {
        this->tImage_ = b;
        this->writeConfig();
    }
};