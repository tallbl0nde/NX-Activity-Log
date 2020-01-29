#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Aether.hpp"
#include "PlayData.hpp"
#include "Title.hpp"
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

            // PlayData object used for all play stats
            PlayData * playdata_;

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

            // Returns PlayData object
            PlayData * playdata();

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