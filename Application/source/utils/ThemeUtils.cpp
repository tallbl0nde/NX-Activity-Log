#include "SimpleIniParser.hpp"
#include "utils/ThemeUtils.hpp"

// Ini file path
#define INI_PATH "/config/NX-Activity-Log/theme.ini"
// Ini var
static simpleIniParser::Ini * ini = nullptr;
// Set true when a value has been changed and thus should write to file
static bool changed;

namespace Utils::Theme {
    void readIni() {
        ini = simpleIniParser::Ini::parseOrCreateFile(INI_PATH);
        changed = false;
    }

    void readValues(std::string s, Aether::Colour & col) {
        if (ini == nullptr) {
            return;
        }

        simpleIniParser::IniSection * sec = ini->findOrCreateSection(s);
        simpleIniParser::IniOption * option = sec->findOrCreateFirstOption("r", std::to_string(col.r()));
        col.setR(std::stoi(option->value));
        option = sec->findOrCreateFirstOption("g", std::to_string(col.g()));
        col.setG(std::stoi(option->value));
        option = sec->findOrCreateFirstOption("b", std::to_string(col.b()));
        col.setB(std::stoi(option->value));
        option = sec->findOrCreateFirstOption("a", std::to_string(col.a()));
        col.setA(std::stoi(option->value));
    }

    void writeValues(std::string s, Aether::Colour col) {
        if (ini == nullptr) {
            return;
        }

        simpleIniParser::IniSection * sec = ini->findOrCreateSection(s);
        simpleIniParser::IniOption * option = sec->findOrCreateFirstOption("r", std::to_string(col.r()));
        option->value = std::to_string(col.r());
        option = sec->findOrCreateFirstOption("g", std::to_string(col.g()));
        option->value = std::to_string(col.g());
        option = sec->findOrCreateFirstOption("b", std::to_string(col.b()));
        option->value = std::to_string(col.b());
        option = sec->findOrCreateFirstOption("a", std::to_string(col.a()));
        option->value = std::to_string(col.a());
        changed = true;
    }

    void deleteIni() {
        if (changed) {
            ini->writeToFile(INI_PATH);
        }
        delete ini;
        ini = nullptr;
    }
};