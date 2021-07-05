#include "Application.hpp"
#include <fstream>
#include <iomanip>
#include "nlohmann/json.hpp"
#include "utils/Curl.hpp"
#include "utils/Lang.hpp"
#include "utils/UpdateUtils.hpp"
#include "utils/Utils.hpp"

#include "ui/screen/AllActivity.hpp"
#include "ui/screen/CustomTheme.hpp"
#include "ui/screen/Details.hpp"
#include "ui/screen/RecentActivity.hpp"
#include "ui/screen/Settings.hpp"
#include "ui/screen/Update.hpp"
#include "ui/screen/UserSelect.hpp"

// Path to background image
#define BACKGROUND_IMAGE "/config/NX-Activity-Log/background.png"

static std::string toHexString(const uint64_t value) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(16) << std::hex << value;
    return ss.str();
}

namespace Main {
    Application::Application() {
        // Start all required services
        Utils::NX::startServices();
        Utils::Curl::init();

        // Create config object and read in values
        this->config_ = new Config();
        this->config_->readConfig();

        // Set language
        if (!Utils::Lang::setLanguage(this->config_->gLang())) {
            this->window->exit();
        }

        this->playdata_ = new NX::PlayData();
        this->theme_ = new Theme(this->config_->gTheme());

        // Start update thread
        this->hasUpdate_ = false;
        this->updateThread = std::async(std::launch::async, &Application::checkForUpdate, this);

        // Check if launched via user page and if so only use the chosen user
        NX::User * u = Utils::NX::getUserPageUser();
        this->isUserPage_ = false;
        if (u != nullptr) {
            this->isUserPage_ = true;
            this->users.push_back(u);
        }

        // Set view to today and by day
        this->tm = Utils::Time::getTmForCurrentTime();
        this->tm.tm_hour = 0;
        this->tm.tm_min = 0;
        this->tm.tm_sec = 0;
        this->viewType = this->config_->lView();
        switch (this->viewType) {
            case ViewPeriod::Year:
                this->tm.tm_mon = 0;

            case ViewPeriod::Month:
                this->tm.tm_mday = 1;

            default:
                break;
        }
        this->tmCopy = this->tm;
        this->viewTypeCopy = this->viewType;

        // Populate users vector
        if (!this->isUserPage_) {
            this->users = Utils::NX::getUserObjects();
        }
        this->userIdx = 0;

        // Populate titles vector
        this->titles = Utils::NX::getTitleObjects(this->users);
        this->titleIdx = 0;

        // Create Aether instance (ignore log messages for now)
        this->window = new Aether::Window("NX-Activity-Log", 1280, 720, [](const std::string message, const bool important) {

        });
        this->window->showDebugInfo(true);

        // Create overlays
        this->dtpicker = nullptr;
        this->periodpicker = new Aether::PopupList("common.view.heading"_lang);
        this->periodpicker->setBackLabel("common.buttonHint.back"_lang);
        this->periodpicker->setOKLabel("common.buttonHint.ok"_lang);

        // Setup screens
        this->setDisplayTheme();
        this->createReason = ScreenCreate::Normal;
        this->createScreens();
        this->reinitScreens_ = ReinitState::False;

        if (this->isUserPage_) {
            // Skip UserSelect screen if launched via user page
            this->setScreen(this->config_->lScreen());
        } else {
            // Start with UserSelect screen
            this->window->setFadeIn(true);
            this->window->setFadeOut(true);
            this->setScreen(ScreenID::UserSelect);
        }
    }

    void Application::checkForUpdate() {
        if (Utils::Update::needsCheck()) {
            Utils::Update::check();
        }

        this->hasUpdate_ = Utils::Update::available();
    }

    void Application::reinitScreens(ScreenCreate c) {
        this->createReason = c;
        this->reinitScreens_ = ReinitState::Wait;
    }

    void Application::createScreens() {
        this->scAllActivity = new Screen::AllActivity(this);
        this->scCustomTheme = new Screen::CustomTheme(this);
        this->scDetails = new Screen::Details(this);
        this->scRecentActivity = new Screen::RecentActivity(this);
        this->scSettings = new Screen::Settings(this, this->createReason);
        this->scUpdate = new Screen::Update(this);

        // These screens aren't used on the user page so no point wasting memory
        if (!this->isUserPage_) {
            this->scUserSelect = new Screen::UserSelect(this, this->users);
        }

        this->createReason = ScreenCreate::Normal;
    }

    void Application::deleteScreens() {
        delete this->scAllActivity;
        delete this->scCustomTheme;
        delete this->scDetails;
        delete this->scRecentActivity;
        delete this->scSettings;
        delete this->scUpdate;

        if (!this->isUserPage_) {
            delete this->scUserSelect;
        }
    }

    void Application::setHoldDelay(int i) {
        this->window->setHoldDelay(i);
    }

    void Application::addOverlay(Aether::Overlay * o) {
        this->window->addOverlay(o);
    }

    void Application::setScreen(ScreenID s) {
        switch (s) {
            case AllActivity:
                this->window->showScreen(this->scAllActivity);
                this->screen = AllActivity;
                break;

            case CustomTheme:
                this->window->showScreen(this->scCustomTheme);
                this->screen = CustomTheme;
                break;

            case Details:
                this->window->showScreen(this->scDetails);
                this->screen = Details;
                break;

            case RecentActivity:
                this->window->showScreen(this->scRecentActivity);
                this->screen = RecentActivity;
                break;

            case Settings:
                this->window->showScreen(this->scSettings);
                this->screen = Settings;
                break;

            case Update:
                this->window->showScreen(this->scUpdate);
                this->screen = Update;
                break;

            case UserSelect:
                this->window->showScreen(this->scUserSelect);
                this->screen = UserSelect;
                break;
        }
    }

    void Application::pushScreen() {
        this->window->pushScreen();
        this->screenStack.push(this->screen);

        // I'll add the onPush()/onPop() methods to all screen when I refactor them
        if (this->screen == RecentActivity) {
            this->scRecentActivity->onPush();
        }
    }

    void Application::popScreen() {
        this->window->popScreen();
        if (!this->screenStack.empty()) {
            this->screen = this->screenStack.top();
            this->screenStack.pop();

            // I'll add the onPush()/onPop() methods to all screen when I refactor them
            if (this->screen == RecentActivity) {
                this->scRecentActivity->onPop();
            }
        }
    }

    void Application::decreaseDate() {
        // All prevent going before 2000
        switch (this->viewType) {
            case ViewPeriod::Day:
                if (this->tm.tm_year == 100 && this->tm.tm_mon == 0 && this->tm.tm_mday == 1) {
                    return;
                }
                this->tm = Utils::Time::decreaseTm(this->tm, 'D');
                break;

            case ViewPeriod::Month:
                if (this->tm.tm_year == 100 && this->tm.tm_mon == 0) {
                    return;
                }
                this->tm = Utils::Time::decreaseTm(this->tm, 'M');
                break;

            case ViewPeriod::Year:
                if (this->tm.tm_year == 100) {
                    return;
                }
                this->tm = Utils::Time::decreaseTm(this->tm, 'Y');
                break;

            default:
                break;
        }
    }

    void Application::increaseDate() {
        // All prevent going past 2060 (same as switch)
        switch (this->viewType) {
            case ViewPeriod::Day:
                if (this->tm.tm_year == 160 && this->tm.tm_mon == 11 && this->tm.tm_mday == 31) {
                    return;
                }
                this->tm = Utils::Time::increaseTm(this->tm, 'D');
                break;

            case ViewPeriod::Month:
                if (this->tm.tm_year == 160 && this->tm.tm_mon == 11) {
                    return;
                }
                this->tm = Utils::Time::increaseTm(this->tm, 'M');
                break;

            case ViewPeriod::Year:
                if (this->tm.tm_year == 160) {
                    return;
                }
                this->tm = Utils::Time::increaseTm(this->tm, 'Y');
                break;

            default:
                break;
        }
    }

    void Application::createDatePicker() {
        if (this->dtpicker != nullptr) {
            delete this->dtpicker;
        }
        switch (this->viewType) {
            case ViewPeriod::Day:
                this->dtpicker = new Aether::DateTime("common.datePanel.headingDate"_lang, this->tm, Aether::DTFlag::Date);
                break;

            case ViewPeriod::Month:
                this->dtpicker = new Aether::DateTime("common.datePanel.headingMonth"_lang, this->tm, Aether::DTFlag::Month | Aether::DTFlag::Year);
                break;

            case ViewPeriod::Year:
                this->dtpicker = new Aether::DateTime("common.datePanel.headingYear"_lang, this->tm, Aether::DTFlag::Year);
                break;

            default:
                break;
        }
        this->dtpicker->setDayHint("common.datePanel.day"_lang);
        this->dtpicker->setMonthHint("common.datePanel.month"_lang);
        this->dtpicker->setYearHint("common.datePanel.year"_lang);
        this->dtpicker->setBackLabel("common.buttonHint.back"_lang);
        this->dtpicker->setOKLabel("common.buttonHint.ok"_lang);
        this->dtpicker->setAllColours(this->theme_->altBG(), this->theme_->accent(), this->theme_->mutedText(), this->theme_->mutedText(), this->theme_->text());
        this->addOverlay(this->dtpicker);
    }

    void Application::createPeriodPicker() {
        this->periodpicker->removeEntries();
        this->periodpicker->addEntry(toString(ViewPeriod::Day), [this](){
            if (this->viewType != ViewPeriod::Day) {
                // Set to current day if within range
                struct tm now = Utils::Time::getTmForCurrentTime();
                if ((this->viewType == ViewPeriod::Year && this->tm.tm_year == now.tm_year) || (this->viewType == ViewPeriod::Month && this->tm.tm_mon == now.tm_mon && this->tm.tm_year == now.tm_year)) {
                    this->tm.tm_mon = now.tm_mon;
                    this->tm.tm_mday = now.tm_mday;
                }

                this->viewType = ViewPeriod::Day;
            }
        }, this->viewType == ViewPeriod::Day);
        this->periodpicker->addEntry(toString(ViewPeriod::Month), [this](){
            if (this->viewType != ViewPeriod::Month) {
                // Set to current month if within range
                struct tm now = Utils::Time::getTmForCurrentTime();
                if (this->viewType == ViewPeriod::Year && this->tm.tm_year == now.tm_year) {
                    this->tm.tm_mon = now.tm_mon;
                }

                this->tm.tm_mday = 1;
                this->viewType = ViewPeriod::Month;
            }
        }, this->viewType == ViewPeriod::Month);
        this->periodpicker->addEntry(toString(ViewPeriod::Year), [this](){
            if (this->viewType != ViewPeriod::Year) {
                this->tm.tm_mon = 0;
                this->tm.tm_mday = 1;
                this->viewType = ViewPeriod::Year;
            }
        }, this->viewType == ViewPeriod::Year);
        this->addOverlay(this->periodpicker);
    }

    Config * Application::config() {
        return this->config_;
    }

    NX::PlayData * Application::playdata() {
        return this->playdata_;
    }

    Theme * Application::theme() {
        return this->theme_;
    }

    void Application::setDisplayTheme() {
        this->periodpicker->setBackgroundColour(this->theme_->altBG());
        this->periodpicker->setTextColour(this->theme_->text());
        this->periodpicker->setLineColour(this->theme_->fg());
        this->periodpicker->setHighlightColour(this->theme_->accent());
        this->periodpicker->setListLineColour(this->theme_->mutedLine());
        this->window->setHighlightAnimation(this->theme_->highlightFunc());
        this->window->setHighlightBackground(this->theme_->highlightBG());
        this->window->setHighlightOverlay(this->theme_->selected());
        if (this->config_->tImage() && this->config_->gTheme() == Custom) {
            if (this->window->setBackgroundImage(BACKGROUND_IMAGE)) {
                return;
            } else {
                // Turn off background image
                this->config_->setTImage(false);
            }
        }
        this->window->setBackgroundColour(this->theme_->bg());
    }

    bool Application::hasUpdate() {
        return this->hasUpdate_;
    }

    struct tm Application::time() {
        return this->tm;
    }

    ViewPeriod Application::viewPeriod() {
        return this->viewType;
    }

    bool Application::timeChanged() {
        bool b = Utils::Time::areDifferentDates(this->tm, this->tmCopy);
        if (!b) {
            b = (this->viewType != this->viewTypeCopy);
        }
        this->tmCopy = this->tm;
        this->viewTypeCopy = this->viewType;

        return b;
    }

    NX::User * Application::activeUser() {
        return this->users[this->userIdx];
    }

    bool Application::isUserPage() {
        return this->isUserPage_;
    }

    void Application::setActiveUser(unsigned short i) {
        this->userIdx = i;
    }

    std::vector<NX::Title *> Application::titleVector() {
        return this->titles;
    }

    NX::Title * Application::activeTitle() {
        return this->titles[this->titleIdx];
    }

    void Application::setActiveTitle(unsigned int i) {
        this->titleIdx = i;
    }

    bool Application::exportToJSON() {
        // Create JSON object and fill with metadata
        nlohmann::json json;
        struct tm time = Utils::Time::getTmForCurrentTime();
        json["exportString"] = Utils::Time::tmToString(time, "%B %d %Y, %T");
        json["exportTimestamp"] = Utils::Time::getTimeT(time);
        json["exportVersion"] = std::string(VER_STRING);

        // Iterate over each user
        json["users"] = nlohmann::json::array();
        for (size_t i = 0; i < this->users.size(); i++) {
            NX::User * user = this->users[i];
            nlohmann::json uJson;

            // Add user metadata
            uJson["name"] = user->username();
            uJson["id"] = toHexString(user->ID().uid[0]) + toHexString(user->ID().uid[1]);
            uJson["titles"] = nlohmann::json::array();

            // Iterate over user's played titles
            for (size_t j = 0; j < this->titles.size(); j++) {
                NX::Title * title = this->titles[j];
                nlohmann::json tJson;

                // Check if played, and if not move onto next
                NX::RecentPlayStatistics * stats = this->playdata_->getRecentStatisticsForTitleAndUser(title->titleID(), std::numeric_limits<u64>::min(), std::numeric_limits<u64>::max(), user->ID());
                bool recentLaunched = (stats->launches != 0);
                delete stats;

                // Add title metadata
                tJson["name"] = title->name();
                tJson["id"] = toHexString(title->titleID());

                // Get all title events
                std::vector<NX::PlayEvent> events = this->playdata_->getPlayEvents(std::numeric_limits<u64>::min(), std::numeric_limits<u64>::max(), title->titleID(), user->ID());
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

                            default:
                                str = "Unknown";
                                break;
                        }

                        eJson["timestamp"] = events[k].clockTimestamp;
                        eJson["type"] = str;
                        tJson["events"].push_back(eJson);
                    }
                }

                // Get all summary stats
                NX::PlayStatistics * stats2 = this->playdata_->getStatisticsForUser(title->titleID(), user->ID());
                bool allLaunched = (stats2->launches != 0);
                tJson["summary"]["firstPlayed"] = pdmPlayTimestampToPosix(stats2->firstPlayed);
                tJson["summary"]["lastPlayed"] = pdmPlayTimestampToPosix(stats2->lastPlayed);
                tJson["summary"]["playtime"] = stats2->playtime;
                tJson["summary"]["launches"] = stats2->launches;
                delete stats2;

                // Append title if played at least once
                if (recentLaunched || allLaunched) {
                    uJson["titles"].push_back(tJson);
                }
            }

            // Append user
            json["users"].push_back(uJson);
        }

        // Write to file
        std::ofstream file("/switch/NX-Activity-Log/export.json");
        file << json.dump(4) << std::endl;
        return true;
    }

    void Application::run() {
        // Do main loop
        while (this->window->loop()) {
            // Check if screens should be recreated
            if (this->reinitScreens_ == ReinitState::Wait) {
                this->reinitScreens_ = ReinitState::True;
            } else if (this->reinitScreens_ == ReinitState::True) {
                this->reinitScreens_ = ReinitState::False;
                this->window->removeScreen();
                this->deleteScreens();
                this->setDisplayTheme();
                this->createScreens();
                this->setScreen(this->screen);
            }
        }
    }

    void Application::exit() {
        this->window->exit();
    }

    Application::~Application() {
        // Delete user objects
        while (users.size() > 0) {
            delete users[0];
            users.erase(users.begin());
        }

        // Delete title objects
        while (titles.size() > 0) {
            delete titles[0];
            titles.erase(titles.begin());
        }

        // Delete objects
        delete this->config_;
        delete this->playdata_;
        delete this->theme_;

        // Delete overlays
        if (this->dtpicker != nullptr) {
            delete this->dtpicker;
        }
        delete this->periodpicker;

        // Cleanup Aether
        this->deleteScreens();
        if (!this->isUserPage_) {
            // Don't delete window as it flickers black (hopefully there's a better way around this)
            delete this->window;
        }

        // Stop all services
        Utils::Curl::exit();
        Utils::NX::stopServices();

        // Install update if present
        Utils::Update::install();

        if (this->isUserPage_) {
            appletRequestExitToSelf();
        }
    }
};
