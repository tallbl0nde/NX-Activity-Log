#include "Application.ExportJob.hpp"
#include <fstream>
#include "nlohmann/json.hpp"
#include "utils/Utils.hpp"

namespace Main {
    Application::ExportJob::ExportJob(Application * app, std::atomic<double> & percent) : Aether::ThreadPool::Job(), percent(percent) {
        this->app = app;
    }

    void Application::ExportJob::work() {
        // Reset percentage
        this->percent = 0;

        // Create JSON object and fill with metadata
        nlohmann::json json;
        struct tm time = Utils::Time::getTmForCurrentTime();
        json["exportString"] = Utils::Time::tmToString(time, "%B %d %Y, %T");
        json["exportTimestamp"] = Utils::Time::getTimeT(time);
        json["exportVersion"] = std::string(VER_STRING);

        // Iterate over each user
        json["users"] = nlohmann::json::array();
        for (size_t i = 0; i < this->app->users.size(); i++) {
            NX::User * user = this->app->users[i];
            nlohmann::json uJson;

            // Add user metadata
            uJson["name"] = user->username();
            uJson["id"] = Utils::formatHexString(user->ID().uid[0]) + Utils::formatHexString(user->ID().uid[1]);
            uJson["titles"] = nlohmann::json::array();

            // Iterate over user's played titles
            for (size_t j = 0; j < this->app->titles.size(); j++) {
                NX::Title * title = this->app->titles[j];
                nlohmann::json tJson;

                // Check if played, and if not move onto next
                NX::RecentPlayStatistics * stats = this->app->playdata_->getRecentStatisticsForTitleAndUser(title->titleID(), std::numeric_limits<u64>::min(), std::numeric_limits<u64>::max(), user->ID());
                bool recentLaunched = (stats->launches != 0);
                delete stats;

                // Add title metadata
                tJson["name"] = title->name();
                tJson["id"] = Utils::formatHexString(title->titleID());

                // Get all title events
                std::vector<NX::PlayEvent> events = this->app->playdata_->getPlayEvents(std::numeric_limits<u64>::min(), std::numeric_limits<u64>::max(), title->titleID(), user->ID());
                tJson["events"] = nlohmann::json::array();
                if (!events.empty()) {
                    // Iterate over all events
                    nlohmann::json eJson;
                    for (size_t k = 0; k < events.size(); k++) {
                        std::string str;
                        switch (events[k].eventType) {
                            case NX::EventType::Applet_Launch:
                                str = "Launched";
                                break;

                            case NX::EventType::Applet_Exit:
                                str = "Closed";
                                break;

                            case NX::EventType::Applet_InFocus:
                                str = "Gained Focus";
                                break;

                            case NX::EventType::Applet_OutFocus:
                                str = "Lost Focus";
                                break;

                            case NX::EventType::Account_Active:
                                str = "Account Login";
                                break;

                            case NX::EventType::Account_Inactive:
                                str = "Account Logout";
                                break;

                            default:
                                str = "Unknown";
                                break;
                        }

                        eJson["clockTimestamp"] = events[k].clockTimestamp;
                        eJson["steadyTimestamp"] = events[k].steadyTimestamp;
                        eJson["type"] = str;
                        tJson["events"].push_back(eJson);
                    }
                }

                // Get all summary stats
                NX::PlayStatistics * stats2 = this->app->playdata_->getStatisticsForUser(title->titleID(), user->ID());
                bool allLaunched = (stats2->launches != 0);
                tJson["summary"]["firstPlayed"] = stats2->firstPlayed;
                tJson["summary"]["lastPlayed"] = stats2->lastPlayed;
                tJson["summary"]["playtime"] = stats2->playtime;
                tJson["summary"]["launches"] = stats2->launches;
                delete stats2;

                // Append title if played at least once
                if (recentLaunched || allLaunched) {
                    uJson["titles"].push_back(tJson);
                }

                // Update percentage
                size_t current = (i * this->app->titles.size()) + j;
                size_t total = this->app->users.size() * this->app->titles.size();
                this->percent = 99 * (current/static_cast<double>(total));
            }

            // Append user
            json["users"].push_back(uJson);
        }

        // Write to file
        std::ofstream file("/switch/NX-Activity-Log/export.json");
        file << json.dump(4) << std::endl;

        // Pause so those with little data can still see the process completed successfully
        this->percent = 99.9;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        this->percent = 100;
    }
};
