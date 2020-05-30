#include <filesystem>
#include <fstream>
#include "nlohmann/json.hpp"
#include "utils/Lang.hpp"
#include "utils/NX.hpp"
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

            case Russian:
                path = "romfs:/lang/ru.json";
                break;

            case Spanish:
                path = "romfs:/lang/es.json";
                break;

            case Turkish:
                path = "romfs:/lang/tr.json";
                break;

            case Korean:
                path = "romfs:/lang/ko.json";
                break;

            default:
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

        // If the string is not present return key
        if (t == nullptr || !t.is_string()) {
            return key;
        }

        return t.get<std::string>();
    }
};