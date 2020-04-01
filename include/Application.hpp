#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Config.hpp"
#include "NX.hpp"
#include "PlayData.hpp"
#include "Theme.hpp"
#include "Time.hpp"

#include "AllActivity.hpp"
#include "Details.hpp"
#include "RecentActivity.hpp"
#include "Settings.hpp"
#include "Update.hpp"
#include "UserSelect.hpp"

// I should really fix this cyclic dependency /shrug
namespace Screen {
    class AllActivity;
    class Details;
    class RecentActivity;
    class Settings;
    class Update;
    class UserSelect;
};

namespace Main {
    // The Application class represents the "root" object of the app. It stores/handles all states
    // and objects used through the app
    class Application {
        private:
            // Display object used for rendering
            Aether::Display * display;

            // Screens of the app
            Screen::AllActivity * scAllActivity;
            Screen::Details * scDetails;
            Screen::RecentActivity * scRecentActivity;
            Screen::Settings * scSettings;
            Screen::Update * scUpdate;
            Screen::UserSelect * scUserSelect;

            // ID of current screen
            ScreenID screen;
            // Set true by reinitScreens() in order to recreate screens
            // before next loop
            bool reinitScreens_;
            // Create screens
            void createScreens();
            // Delete screens
            void deleteScreens();

            // Config object allows interfacing with config file
            Config * config_;
            // PlayData object used for all play stats
            NX::PlayData * playdata_;
            // Stores current theme colours
            Theme * theme_;

            // Time to view recent activity, etc...
            struct tm tm;
            // Copy of above struct used to determine if changed
            struct tm tmCopy;
            // Period of time to view
            ViewPeriod viewType;
            // Set true when either have been changed
            bool timeChanged_;

            // Set true if launched via user page
            bool isUserPage_;
            // Vector of users
            std::vector<NX::User *> users;
            // Index of selected user
            unsigned short userIdx;

            // Vector of titles
            std::vector<NX::Title *> titles;
            // Index of "active" title (used for specific screens)
            unsigned int titleIdx;

            // Date picker overlay
            Aether::DateTime * dtpicker;
            // ViewPeriod picker
            Aether::PopupList * periodpicker;

        public:
            // Constructor inits Aether, screens + other objects
            Application();

            // Destroys and recreates screens (effectively a restart without restarting in terms of UI?)
            void reinitScreens();

            // Wrapper for display function
            void setHoldDelay(int);

            // Pass an overlay element in order to render
            // Element is not deleted when closed!
            void addOverlay(Aether::Overlay *);

            // Pass screen enum to change to it
            void setScreen(ScreenID);
            // Push current screen on stack (i.e. keep in memory)
            void pushScreen();
            // Pop screen from stack
            void popScreen();

            // Decrease/increase date tm by one d/m/y
            void decreaseDate();
            void increaseDate();
            // Create date picker overlay
            void createDatePicker();
            // Create view period picker overlay
            void createPeriodPicker();

            // Returns Config object
            Config * config();
            // Returns PlayData object
            NX::PlayData * playdata();
            // Returns theme object
            Theme * theme();

            // Returns copy of tm struct (can only be set by this class)
            struct tm time();
            // Return current viewperiod (also only set by this class)
            ViewPeriod viewPeriod();
            // Has the time struct or period been changed?
            bool timeChanged();

            // Returns isUserPage_
            bool isUserPage();

            // Returns user object of active user
            NX::User * activeUser();
            // Sets active user given index
            void setActiveUser(unsigned short);

            // Returns reference to titles vector
            std::vector<NX::Title *> titleVector();
            // Returns currently selected title
            NX::Title * activeTitle();
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