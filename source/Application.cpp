#include "Application.hpp"
#include "Utils.hpp"

namespace Main {
    Application::Application() {
        // Start all required services
        Utils::startServices();

        // Create config object and read in values
        this->config_ = new Config();
        this->config_->readConfig();

        // Populate users vector
        this->users = Utils::getUserObjects();
        this->userIdx = 0;

        // Populate titles vector
        this->titles = Utils::getTitleObjects(this->users);
        this->titleIdx = 0;

        // Create PlayData object
        this->playdata_ = new PlayData();

        // Create Aether instance
        this->display = new Aether::Display();
        this->display->setBackgroundColour(Aether::Theme::Dark.bg.r, Aether::Theme::Dark.bg.g, Aether::Theme::Dark.bg.b);
        this->display->setHighlightColours(Aether::Theme::Dark.highlightBG, Aether::Theme::Dark.selected);
        this->display->setHighlightAnimation(Aether::Theme::Dark.highlightFunc);
        this->display->setShowFPS(true);

        // Setup screens
        this->scAllActivity = new Screen::AllActivity(this);
        this->scSettings = new Screen::Settings(this);
        this->scUserSelect = new Screen::UserSelect(this, this->users);

        // Start with UserSelect screen
        this->setScreen(ScreenID::UserSelect);
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

    PlayData * Application::playdata() {
        return this->playdata_;
    }

    User * Application::activeUser() {
        return this->users[this->userIdx];
    }

    void Application::setActiveUser(unsigned short i) {
        this->userIdx = i;
    }

    std::vector<Title *> Application::titleVector() {
        return this->titles;
    }

    Title * Application::activeTitle() {
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

        // Delete screens
        delete this->scAllActivity;
        delete this->scSettings;
        delete this->scUserSelect;

        // Cleanup Aether
        delete this->display;

        // Stop all services
        Utils::stopServices();
    }
};