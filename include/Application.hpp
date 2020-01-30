#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Config.hpp"
#include "PlayData.hpp"
#include "Theme.hpp"
#include "Title.hpp"
#include "User.hpp"

#include "AllActivity.hpp"
#include "Settings.hpp"
#include "UserSelect.hpp"

// I should really fix this cyclic dependency /shrug
namespace Screen {
    class AllActivity;
    class Settings;
    class UserSelect;
};

namespace Main {
    // Enumeration for screens (allows for easy switching)
    enum ScreenID {
        UserSelect,
        AllActivity,
        RecentActivity,
        Settings,
        Details
    };

    // The Application class represents the "root" object of the app. It stores/handles all states
    // and objects used through the app
    class Application {
        private:
            // Display object used for rendering
            Aether::Display * display;

            // Screens of the app
            Screen::AllActivity * scAllActivity;
            Screen::Settings * scSettings;
            Screen::UserSelect * scUserSelect;

            // Config object allows interfacing with config file
            Config * config_;
            // PlayData object used for all play stats
            PlayData * playdata_;
            // Stores current theme colours
            Theme * theme_;

            // Vector of users
            std::vector<User *> users;
            // Index of selected user
            unsigned short userIdx;

            // Vector of titles
            std::vector<Title *> titles;
            // Index of "active" title (used for specific screens)
            unsigned int titleIdx;

        public:
            // Constructor inits Aether, screens + other objects
            Application();

            // Wrapper for display function
            void setHoldDelay(int);

            // Pass an overlay element in order to render
            // Element is not deleted when closed!
            void addOverlay(Aether::Overlay *);

            // Pass screen enum to change to it
            void setScreen(ScreenID);

            // Returns Config object
            Config * config();
            // Returns PlayData object
            PlayData * playdata();
            // Returns theme object
            Theme * theme();

            // Returns user object of active user
            User * activeUser();
            // Sets active user given index
            void setActiveUser(unsigned short);

            // Returns reference to titles vector
            std::vector<Title *> titleVector();
            // Returns currently selected title
            Title * activeTitle();
            // Set active title given index
            void setActiveTitle(unsigned int);

            // Handles display loop
            void run();
            // Call to stop display loop
            void exit();

            // Destructor frees memory and quits Aether
            ~Application();
    };
};

#endif