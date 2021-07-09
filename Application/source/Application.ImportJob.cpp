#include "Application.ImportJob.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include "nlohmann/json.hpp"
#include "utils/Utils.hpp"
#include "utils/Time.hpp"

namespace Main {
    Application::ImportJob::ImportJob(Application * app, std::atomic<double> & percent) : Aether::ThreadPool::Job(), percent(percent) {
        this->app = app;
    }

    void Application::ImportJob::work() {
        // Reset percentage
        this->percent = 0;

        // Read in import JSON
        if (!std::filesystem::exists("/switch/NX-Activity-Log/import.json")) {
            this->percent = 100;
            return;
        }
        std::ifstream importFile("/switch/NX-Activity-Log/import.json");
        nlohmann::json importJson = nlohmann::json::parse(importFile);

        // Create imported JSON
        nlohmann::json json;
        struct tm time = Utils::Time::getTmForCurrentTime();
        json["aWarning"] = "This file was generated automatically - editing it may cause crashes!";
        json["importString"] = Utils::Time::tmToString(time, "%B %d %Y, %T");
        json["importTimestamp"] = Utils::Time::getTimeT(time);
        json["importVersion"] = std::string(VER_STRING);

        // Abort if no users array in JSON
        if (importJson["users"] == nullptr) {
            this->percent = 100;
            return;
        }

        // Count number of "iterations" that need to be performed in order to
        // monitor the progress
        size_t current = 0;
        size_t total = 0;
        for (nlohmann::json user : importJson["users"]) {
            if (user["titles"] != nullptr) {
                total += user["titles"].size();
            }
        }

        // Iterate over each user in the import JSON, looking for
        // matching usernames
        for (nlohmann::json user : importJson["users"]) {
            // Skip over users with no name, id or titles
            if (user["name"] == nullptr || user["id"] == nullptr || user["titles"] == nullptr) {
                continue;
            }

            // Check if we have a user with matching name
            std::vector<NX::User *>::iterator it = std::find_if(this->app->users.begin(), this->app->users.end(), [user](NX::User * u) {
                return (user["name"].get<std::string>() == u->username());
            });
            if (it == this->app->users.end()) {
                continue;
            }

            // We have a matching user, so now create an entry for them
            NX::User * u = (*it);
            nlohmann::json uJson;
            std::string id = user["id"].get<std::string>();
            uJson["id"] = nlohmann::json::array();
            uJson["id"].push_back(u->ID().uid[0]);
            uJson["id"].push_back(u->ID().uid[1]);
            uJson["titles"] = nlohmann::json::array();

            // Iterate over each title, copying all metadata + events
            // (but reformat to an easier to use format)
            for (nlohmann::json title : user["titles"]) {
                current++;

                // Skip title if no name or id present
                if (title["id"] == nullptr || title["name"] == nullptr) {
                    continue;
                }

                nlohmann::json tJson;
                tJson["id"] = Utils::stringToU64(title["id"].get<std::string>());
                tJson["name"] = title["name"];
                tJson["events"] = nlohmann::json::array();

                // Iterate over each event
                nlohmann::json eJson;
                for (nlohmann::json event : title["events"]) {
                    // Skip event if no timestamp or type prsent
                    if (event["type"] == nullptr || event["clockTimestamp"] == nullptr || event["steadyTimestamp"] == nullptr) {
                        continue;
                    }

                    // Convert event type to enum
                    std::string str = event["type"].get<std::string>();
                    int val;
                    if (str == "Launched") {
                        val = static_cast<int>(NX::EventType::Applet_Launch);

                    } else if (str == "Closed") {
                        val = static_cast<int>(NX::EventType::Applet_Exit);

                    } else if (str == "Gained Focus") {
                        val = static_cast<int>(NX::EventType::Applet_InFocus);

                    } else if (str == "Lost Focus") {
                        val = static_cast<int>(NX::EventType::Applet_OutFocus);

                    } else if (str == "Account Login") {
                        val = static_cast<int>(NX::EventType::Account_Active);

                    } else if (str == "Account Logout") {
                        val = static_cast<int>(NX::EventType::Account_Inactive);

                    } else {
                        // Skip event if invalid type
                        continue;
                    }

                    eJson["clockTimestamp"] = event["clockTimestamp"];
                    eJson["steadyTimestamp"] = event["steadyTimestamp"];
                    eJson["type"] = val;
                    tJson["events"].push_back(eJson);
                }

                // Copy over summary stats
                if (title["summary"] != nullptr) {
                    nlohmann::json s = title["summary"];
                    if (s["firstPlayed"] != nullptr && s["lastPlayed"] != nullptr && s["playtime"] != nullptr && s["launches"] != nullptr) {
                        tJson["summary"] = nlohmann::json();
                        tJson["summary"]["firstPlayed"] = Utils::Time::posixTimestampToPdm(s["firstPlayed"].get<uint64_t>());
                        tJson["summary"]["lastPlayed"] = Utils::Time::posixTimestampToPdm(s["lastPlayed"].get<uint64_t>());
                        tJson["summary"]["playtime"] = s["playtime"];
                        tJson["summary"]["launches"] = s["launches"];
                    }
                }

                // Append title if at least one event or a summary is present
                if (tJson["events"].size() > 0 || tJson["summary"] != nullptr) {
                    uJson["titles"].push_back(tJson);
                }

                // Update percentage
                this->percent = 99 * (current/static_cast<double>(total));
            }

            // Append user if they have some titles
            if (uJson["titles"].size() > 0) {
                json["users"].push_back(uJson);
            }
        }

        // Write imported data to file
        std::ofstream importedFile("/switch/NX-Activity-Log/importedData.json");
        importedFile << json.dump(4) << std::endl;

        // Pause so those with little data can still see the process completed successfully
        this->percent = 99.9;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        this->percent = 100;
    }
};
