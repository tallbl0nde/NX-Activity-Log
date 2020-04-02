#include <filesystem>
#include <fstream>
#include "JSON.hpp"
#include "Lang.hpp"
#include "NX.hpp"
#include <sstream>

namespace Utils::Lang {
    // json object which stores strings
    nlohmann::json j = nullptr;

    bool setFile(std::string path) {
        // Check it exists
        if (!std::filesystem::exists(path)) {
            return false;
        }

        // Open and copy into object
        std::ifstream in(path);
        j = nlohmann::json::parse(in);

        return true;
    }

    bool setLanguage(Language l) {
        std::string path = "";
        if (l == Default) {
            l = Utils::NX::getSystemLanguage();
        }

        switch (l) {
            case Default:
            case English:
                path = "romfs:/lang/en.json";
                break;

            case French:
                path = "romfs:/lang/fr.json";
                break;

            case German:
                path = "romfs:/lang/de.json";
                break;

            case Italian:
                path = "romfs:/lang/it.json";
                break;

            case Portugese:
                path = "romfs:/lang/pt-BR.json";
                break;
        }

        return setFile(path);
    }

    std::string string(std::string key) {
        // First 'navigate' to nested object
        nlohmann::json t = j;
        std::istringstream ss(key);
        std::string k;
        while (std::getline(ss, k, '.') && t != nullptr) {
            t = t[k];
        }

        // If the string is not present return ?
        if (t == nullptr || !t.is_string()) {
            return "?";
        }

        return t.get<std::string>();
    }
};