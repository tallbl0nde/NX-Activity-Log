#include "Application.hpp"

namespace Main {
    Application::Application() {
        // Start all required services
        Utils::NX::startServices();

        // Create config object and read in values
        this->config_ = new Config();
        this->config_->readConfig();

        this->playdata_ = new NX::PlayData();
        this->theme_ = new Theme(this->config_->gTheme());

        // Check if launched via user page and if so only use the chosen user
        NX::User * u = Utils::NX::getUserPageUser();
        this->isUserPage_ = false;
        if (u != nullptr) {
            this->isUserPage_ = true;
            this->users.push_back(u);
        }

        // Populate users vector
        if (!this->isUserPage_) {
            this->users = Utils::NX::getUserObjects();
        }
        this->userIdx = 0;

        // Populate titles vector
        this->titles = Utils::NX::getTitleObjects(this->users);
        this->titleIdx = 0;

        // Create Aether instance
        this->display = new Aether::Display();
        this->display->setBackgroundColour(this->theme_->bg().r, this->theme_->bg().g, this->theme_->bg().b);
        this->display->setHighlightColours(this->theme_->highlightBG(), this->theme_->selected());
        this->display->setHighlightAnimation(Aether::Theme::Dark.highlightFunc);
        this->display->setShowFPS(true);

        // Setup screens
        this->scAllActivity = new Screen::AllActivity(this);
        this->scDetails = new Screen::Details(this);
        this->scRecentActivity = new Screen::RecentActivity(this);
        this->scSettings = new Screen::Settings(this);

        if (this->isUserPage_) {
            // Skip UserSelect screen if launched via user page
            this->setScreen(ScreenID::RecentActivity);
        } else {
            // No need for user select if user page
            this->scUserSelect = new Screen::UserSelect(this, this->users);
            // Start with UserSelect screen
            this->setScreen(ScreenID::UserSelect);
        }
    }

    void Application::setHoldDelay(int i) {
        this->display->setHoldDelay(i);
    }

    void Application::addOverlay(Aether::Overlay * o) {
        this->display->addOverlay(o);
    }

    void Application::setScreen(ScreenID s) {
        switch (s) {
            case AllActivity:
                this->display->setScreen(this->scAllActivity);
                break;

            case Details:
                this->display->setScreen(this->scDetails);
                break;

            case RecentActivity:
                this->display->setScreen(this->scRecentActivity);
                break;

            case Settings:
                this->display->setScreen(this->scSettings);
                break;

            case UserSelect:
                this->display->setScreen(this->scUserSelect);
                break;
        }
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

    void Application::run() {
        while (this->display->loop()) {
        }
    }

    void Application::exit() {
        this->display->exit();
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

        // Delete screens
        delete this->scAllActivity;
        delete this->scDetails;
        delete this->scRecentActivity;
        delete this->scSettings;
        delete this->scUserSelect;

        // Cleanup Aether
        delete this->display;

        // Stop all services
        Utils::NX::stopServices();
    }
};