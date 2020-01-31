#include "Config.hpp"
#include <filesystem>
#include "SimpleIniParser.hpp"

// Location of config file on SD card
#define CONFIG_FILE "/config/NX-Activity-Log/config.ini"

namespace Main {
    void Config::initConfigFile() {
        // Create dirs
        std::filesystem::create_directory("/config");
        std::filesystem::create_directory("/config/NX-Activity-Log");

        // File now exists, can be populated
        simpleIniParser::Ini * ini = new simpleIniParser::Ini();

        simpleIniParser::IniSection * section_general = new simpleIniParser::IniSection(simpleIniParser::IniSectionType::Section, "general");
        section_general->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "sort", "LastPlayed"));
        section_general->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "theme", "Auto"));
        ini->sections.push_back(section_general);

        simpleIniParser::IniSection * section_hidden = new simpleIniParser::IniSection(simpleIniParser::IniSectionType::Section, "hidden");
        section_hidden->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "deleted", "false"));
        ini->sections.push_back(section_hidden);

        ini->writeToFile(CONFIG_FILE);
        delete ini;
    }

    void Config::readConfig() {
        // Check if file exists, and if not create it
        if (!std::filesystem::exists(CONFIG_FILE)) {
            this->initConfigFile();
        }

        // Read file
        simpleIniParser::Ini * ini = simpleIniParser::Ini::parseFile(CONFIG_FILE);
        simpleIniParser::IniOption * option = ini->findSection("general")->findFirstOption("sort", false);
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

        option = ini->findSection("general")->findFirstOption("theme", false);
        if (option->value == "Light") {
            this->gTheme_ = Light;
        } else if (option->value == "Dark") {
            this->gTheme_ = Dark;
        } else {
            this->gTheme_ = Auto;
        }

        option = ini->findSection("hidden")->findFirstOption("deleted", false);
        if (option->value == "true") {
            this->hDeleted_ = true;
        } else {
            this->hDeleted_ = false;
        }

        delete ini;
    }

    void Config::writeConfig() {
        // Write file
        simpleIniParser::Ini * ini = simpleIniParser::Ini::parseFile(CONFIG_FILE);

        simpleIniParser::IniOption * option = ini->findSection("general")->findFirstOption("sort", false);
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

        option = ini->findSection("general")->findFirstOption("theme", false);
        if (this->gTheme_ == Light) {
            option->value = "Light";
        } else if (this->gTheme_ == Dark) {
            option->value = "Dark";
        } else if (this->gTheme_ == Auto) {
            option->value = "Auto";
        }

        option = ini->findSection("hidden")->findFirstOption("deleted", false);
        if (this->hDeleted_ == true) {
            option->value = "true";
        } else if (this->hDeleted_ == false) {
            option->value = "false";
        }

        ini->writeToFile(CONFIG_FILE);
        delete ini;
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