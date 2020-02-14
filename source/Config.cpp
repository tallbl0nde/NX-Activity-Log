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
        simpleIniParser::IniOption * option = sec->findOrCreateFirstOption("showGraphValues", "true");
        if (option->value == "false") {
            this->gGraph_ = false;
        } else {
            this->gGraph_ = true;
        }

        option = sec->findOrCreateFirstOption("sort", "LastPlayed");
        if (option->value == "AlphaAsc") {
            this->gSort_ = AlphaAsc;
        } else if (option->value == "HoursAsc") {
            this->gSort_ = HoursAsc;
        } else if (option->value == "HoursDec") {
            this->gSort_ = HoursDec;
        } else if (option->value == "LaunchAsc") {
            this->gSort_ = LaunchAsc;
        } else if (option->value == "LaunchDec") {
            this->gSort_ = LaunchDec;
        } else if (option->value == "FirstPlayed") {
            this->gSort_ = FirstPlayed;
        } else {
            this->gSort_ = LastPlayed;
        }

        option = sec->findOrCreateFirstOption("theme", "Auto");
        if (option->value == "Light") {
            this->gTheme_ = Light;
        } else if (option->value == "Dark") {
            this->gTheme_ = Dark;
        } else {
            this->gTheme_ = Auto;
        }

        sec = ini->findOrCreateSection("hidden");
        option = sec->findOrCreateFirstOption("deleted", "false");
        if (option->value == "true") {
            this->hDeleted_ = true;
        } else {
            this->hDeleted_ = false;
        }
    }

    void Config::writeConfig() {
        if (this->ini == nullptr) {
            return;
        }

        // Write file
        simpleIniParser::IniOption * option = ini->findSection("general")->findFirstOption("showGraphValues", false);
        if (this->gGraph_ == true) {
            option->value = "true";
        } else if (this->gGraph_ == false) {
            option->value = "false";
        }

        option = ini->findSection("general")->findFirstOption("sort");
        if (this->gSort_ == AlphaAsc) {
            option->value = "AlphaAsc";
        } else if (this->gSort_ == HoursAsc) {
            option->value = "HoursAsc";
        } else if (this->gSort_ == HoursDec) {
            option->value = "HoursDec";
        } else if (this->gSort_ == LaunchAsc) {
            option->value = "LaunchAsc";
        } else if (this->gSort_ == LaunchDec) {
            option->value = "LaunchDec";
        } else if (this->gSort_ == FirstPlayed) {
            option->value = "FirstPlayed";
        } else if (this->gSort_ == LastPlayed) {
            option->value = "LastPlayed";
        }

        option = ini->findSection("general")->findFirstOption("theme");
        if (this->gTheme_ == Light) {
            option->value = "Light";
        } else if (this->gTheme_ == Dark) {
            option->value = "Dark";
        } else if (this->gTheme_ == Auto) {
            option->value = "Auto";
        }

        option = ini->findSection("hidden")->findFirstOption("deleted");
        if (this->hDeleted_ == true) {
            option->value = "true";
        } else if (this->hDeleted_ == false) {
            option->value = "false";
        }

        ini->writeToFile("/config/NX-Activity-Log/config.ini");
    }

    bool Config::gGraph() {
        return this->gGraph_;
    }

    SortType Config::gSort() {
        return this->gSort_;
    }

    ThemeType Config::gTheme() {
        return this->gTheme_;
    }

    bool Config::hDeleted() {
        return this->hDeleted_;
    }

    void Config::setGGraph(bool b) {
        this->gGraph_ = b;
        this->writeConfig();
    }

    void Config::setGSort(SortType v) {
        this->gSort_ = v;
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
};