#include "config.hpp"
#include <filesystem>
#include "SimpleIniParser.hpp"

#define CONFIG_FILE "/config/NX-Activity-Log/config.ini"

Config * Config::instance = nullptr;

Config::Config() {

}

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
    section_hidden->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "unplayed", "false"));
    ini->sections.push_back(section_hidden);

    simpleIniParser::IniSection * section_offset = new simpleIniParser::IniSection(simpleIniParser::IniSectionType::Section, "offset");
    // Need to add this in
    ini->sections.push_back(section_offset);

    ini->writeToFile(CONFIG_FILE);
    delete ini;
}

Config * Config::getConfig() {
    // Create instance if it doesn't exist
    if (!Config::instance) {
        Config::instance = new Config();
    }
    return Config::instance;
}

void Config::readConfig() {
    // Check if file exists, and if not create it
    if (!std::filesystem::exists(CONFIG_FILE)) {
        this->instance->initConfigFile();
    }

    // Read file
    simpleIniParser::Ini * ini = simpleIniParser::Ini::parseFile(CONFIG_FILE);
    simpleIniParser::IniOption * option = ini->findSection("general")->findFirstOption("sort", false);
    if (option->value == "AlphaAsc") {
        this->general_sort = AlphaAsc;
    } else if (option->value == "HoursAsc") {
        this->general_sort = HoursAsc;
    } else if (option->value == "HoursDec") {
        this->general_sort = HoursDec;
    } else if (option->value == "LaunchAsc") {
        this->general_sort = LaunchAsc;
    } else if (option->value == "LaunchDec") {
        this->general_sort = LaunchDec;
    } else if (option->value == "FirstPlayed") {
        this->general_sort = FirstPlayed;
    } else {
        this->general_sort = LastPlayed;
    }

    option = ini->findSection("general")->findFirstOption("theme", false);
    if (option->value == "Light") {
        this->general_theme = T_Light;
    } else if (option->value == "Dark") {
        this->general_theme = T_Dark;
    } else {
        this->general_theme = T_Auto;
    }

    option = ini->findSection("hidden")->findFirstOption("deleted", false);
    if (option->value == "true") {
        this->hidden_deleted = true;
    } else {
        this->hidden_deleted = false;
    }

    option = ini->findSection("hidden")->findFirstOption("unplayed", false);
    if (option->value == "true") {
        this->hidden_unplayed = true;
    } else {
        this->hidden_unplayed = false;
    }

    delete ini;
}

void Config::writeConfig() {
    // Write file
    simpleIniParser::Ini * ini = simpleIniParser::Ini::parseFile(CONFIG_FILE);

    simpleIniParser::IniOption * option = ini->findSection("general")->findFirstOption("sort", false);
    if (this->general_sort == AlphaAsc) {
        option->value = "AlphaAsc";
    } else if (this->general_sort == HoursAsc) {
        option->value = "HoursAsc";
    } else if (this->general_sort == HoursDec) {
        option->value = "HoursDec";
    } else if (this->general_sort == LaunchAsc) {
        option->value = "LaunchAsc";
    } else if (this->general_sort == LaunchDec) {
        option->value = "LaunchDec";
    } else if (this->general_sort == FirstPlayed) {
        option->value = "FirstPlayed";
    } else if (this->general_sort == LastPlayed) {
        option->value = "LastPlayed";
    }

    option = ini->findSection("general")->findFirstOption("theme", false);
    if (this->general_theme == T_Light) {
        option->value = "Light";
    } else if (this->general_theme == T_Dark) {
        option->value = "Dark";
    } else if (this->general_theme == T_Auto) {
        option->value = "Auto";
    }

    option = ini->findSection("hidden")->findFirstOption("deleted", false);
    if (this->hidden_deleted == true) {
        option->value = "true";
    } else if (this->hidden_deleted == false) {
        option->value = "false";
    }

    option = ini->findSection("hidden")->findFirstOption("unplayed", false);
    if (this->hidden_unplayed == true) {
        option->value = "true";
    } else if (this->hidden_unplayed == false) {
        option->value = "false";
    }

    ini->writeToFile(CONFIG_FILE);
    delete ini;
}

SortType Config::getGeneralSort() {
    return this->general_sort;
}

ThemeType Config::getGeneralTheme() {
    return this->general_theme;
}

bool Config::getHiddenDeleted() {
    return this->hidden_deleted;
}

bool Config::getHiddenUnplayed() {
    return this->hidden_unplayed;
}

void Config::setGeneralSort(SortType v) {
    this->general_sort = v;
}

void Config::setGeneralTheme(ThemeType v) {
    this->general_theme = v;
}

void Config::setHiddenDeleted(bool v) {
    this->hidden_deleted = v;
}

void Config::setHiddenUnplayed(bool v) {
    this->hidden_unplayed = v;
}
