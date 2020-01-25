#include "Application.hpp"
#include "Utils.hpp"

namespace Main {
    Application::Application() {
        // Start all required services
        Utils::startServices();

        // Populate users vector
        this->users = Utils::getUserObjects();
        this->userIdx = 0;

        // Create Aether instance
        this->display = new Aether::Display();
        this->display->setBackgroundColour(Aether::Theme::Dark.bg.r, Aether::Theme::Dark.bg.g, Aether::Theme::Dark.bg.b);
        this->display->setHighlightColours(Aether::Theme::Dark.highlightBG, Aether::Theme::Dark.selected);
        this->display->setHighlightAnimation(Aether::Theme::Dark.highlightFunc);

        // Setup screens
        this->scUserSelect = new Screen::UserSelect(this, this->users);

        // Start with UserSelect screen
        this->setScreen(ScreenID::UserSelect);
    }

    void Application::setScreen(ScreenID s) {
        switch (s) {
            case UserSelect:
                this->display->setScreen(this->scUserSelect);
                break;
        }
    }

    User * Application::activeUser() {
        return this->users[this->userIdx];
    }

    void Application::setActiveUser(unsigned short i) {
        this->userIdx = i;
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

        // Delete screens
        delete this->scUserSelect;

        // Cleanup Aether
        delete this->display;

        // Stop all services
        Utils::stopServices();
    }
};