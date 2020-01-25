#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Aether.hpp"
#include "User.hpp"

#include "AllActivity.hpp"
#include "UserSelect.hpp"

// I should really fix this cyclic dependency /shrug
namespace Screen {
    class UserSelect;
    class AllActivity;
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
            Screen::UserSelect * scUserSelect;

            // Vector of users
            std::vector<User *> users;
            // Index of selected user
            unsigned short userIdx;

        public:
            // Constructor inits Aether, screens + other objects
            Application();

            // Pass screen enum to change to it
            void setScreen(ScreenID);

            // Returns user object of active user
            User * activeUser();
            // Sets active user given index
            void setActiveUser(unsigned short);

            // Handles display loop
            void run();

            // Call to stop display loop
            void exit();

            // Destructor frees memory and quits Aether
            ~Application();
    };
};

#endif