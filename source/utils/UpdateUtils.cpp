#include "utils/Curl.hpp"
#include <filesystem>
#include <fstream>
#include "nlohmann/json.hpp"
#include "utils/UpdateUtils.hpp"
#include "utils/Time.hpp"

// Flag indicating an update is available
#define AVAILABLE_FILE "/config/NX-Activity-Log/update.flag"
// URL to query release data
#define GITHUB_API_URL "https://api.github.com/repos/tallbl0nde/NX-Activity-Log/releases/latest"
// File storing timestamp of last check
#define TIMESTAMP_FILE "/config/NX-Activity-Log/update.time"
// Path of downloaded .nro
#define TMP_DONE_PATH "/switch/NX-Activity-Log/update.nro"
// Path of downloading .nro (not finished)
#define TMP_PATH "/switch/NX-Activity-Log/tmp.nro"

namespace Utils::Update {
    bool available() {
        return std::filesystem::exists(AVAILABLE_FILE);
    }

    UpdateData check() {
        UpdateData d;
        d.success = false;
        std::string data = Utils::Curl::downloadToString(GITHUB_API_URL);
        if (data.length() > 0) {
            nlohmann::json j = nlohmann::json::parse(data);
            if (j["tag_name"] != nullptr && j["body"] != nullptr) {
                d.version = j["tag_name"].get<std::string>();
                d.changelog = j["body"].get<std::string>();
                if (j["assets"] != nullptr) {
                    if (j["assets"][0] != nullptr) {
                        if (j["assets"][0]["browser_download_url"] != nullptr) {
                            d.url = j["assets"][0]["browser_download_url"].get<std::string>();
                            d.success = true;
                        }
                    }
                }
            }
        }

        if (!available()) {
            if (d.success) {
                if (d.version != "v" VER_STRING) {
                    std::ofstream file(AVAILABLE_FILE);
                    file << d.version;
                }
            }
        }

        // Get timestamp and write to file
        if (d.success) {
            std::ofstream file(TIMESTAMP_FILE, std::ios::trunc);
            file << Utils::Time::getTimeT(Utils::Time::getTmForCurrentTime());
        }

        return d;
    }

    bool download(std::string url, std::function<void(long long int, long long int)> func) {
        // Create folders if necessary
        if (!std::filesystem::exists("/switch/NX-Activity-Log/")) {
            std::filesystem::create_directory("/switch");
            std::filesystem::create_directory("/switch/NX-Activity-Log");
        }

        bool b = Utils::Curl::downloadToFile(url, TMP_PATH, func);
        if (!b) {
            // If failed remove file
            std::filesystem::remove(TMP_PATH);
        } else {
            // If it succeeded rename
            std::filesystem::rename(TMP_PATH, TMP_DONE_PATH);
        }

        return b;
    }

    void install() {
        if (std::filesystem::exists(TMP_DONE_PATH)) {
            std::filesystem::remove("/switch/NX-Activity-Log/NX-Activity-Log.nro");
            std::filesystem::rename(TMP_DONE_PATH, "/switch/NX-Activity-Log/NX-Activity-Log.nro");
            if (std::filesystem::exists(AVAILABLE_FILE)) {
                std::filesystem::remove(AVAILABLE_FILE);
            }
        }
    }

    bool needsCheck() {
        // Don't check if an update is available
        if (std::filesystem::exists(AVAILABLE_FILE)) {
            return false;
        }

        // If no timestamp file check
        if (!std::filesystem::exists(TIMESTAMP_FILE)) {
            return true;
        }

        // Read timestamp from file
        std::ifstream file(TIMESTAMP_FILE);
        unsigned int ts;
        file >> ts;
        return Utils::Time::areDifferentDates(Utils::Time::getTm(ts), Utils::Time::getTmForCurrentTime());
    }
};