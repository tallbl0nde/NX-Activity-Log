#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <atomic>
#include "Config.hpp"
#include <future>
#include "utils/NX.hpp"
#include "nx/PlayData.hpp"
#include <stack>
#include "ui/Theme.hpp"
#include "utils/Time.hpp"

namespace Screen {
    class AllActivity;
    class CustomTheme;
    class Details;
    class RecentActivity;
    class Settings;
    class Update;
    class UserSelect;
};

enum class ReinitState {
    False,
    True,
    Wait
};

namespace Main {
    // The Application class represents the "root" object of the app. It stores/handles all states
    // and objects used through the app
    class Application {
        private:
            // Main aether object used for rendering
            Aether::Window * window;

            // Screens of the app
            Screen::AllActivity * scAllActivity;
            Screen::CustomTheme * scCustomTheme;
            Screen::Details * scDetails;
            Screen::RecentActivity * scRecentActivity;
            Screen::Settings * scSettings;
            Screen::Update * scUpdate;
            Screen::UserSelect * scUserSelect;

            // ID of current screen
            ScreenID screen;
            // Reason for screen creation (set to Normal after reinit)
            ScreenCreate createReason;
            // Set true by reinitScreens() in order to recreate screens
            // before next loop
            ReinitState reinitScreens_;
            // Create screens
            void createScreens();
            // Delete screens
            void deleteScreens();
            // Stack of ScreenIDs matching pushed screens
            std::stack<ScreenID> screenStack;

            // Config object allows interfacing with config file
            Config * config_;
            // PlayData object used for all play stats
            NX::PlayData * playdata_;
            // Stores current theme colours
            Theme * theme_;

            // Set true when an update is available
            std::atomic<bool> hasUpdate_;
            // Future for update check thread
            std::future<void> updateThread;
            // Function to check for update
            void checkForUpdate();

            // Time to view recent activity, etc...
            struct tm tm;
            // Copy of above struct used to determine if changed
            struct tm tmCopy;
            // Period of time to view
            ViewPeriod viewType;
            // Copy of view period to determine if changed
            ViewPeriod viewTypeCopy;
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
            void reinitScreens(ScreenCreate);

            // Wrapper for window function
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
            // (Re)set the display theme variables for highlight/select/etc... using values in theme object
            void setDisplayTheme();

            // True if an update is available
            bool hasUpdate();

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

            // Imports previously exported play data, returning if successful or not
            // TODO: implement
            // Exports stored play data, returning if successful or not
            bool exportToJSON();

            // Handles window loop
            void run();
            // Call to stop window loop
            void exit();

            // Destructor frees memory and quits Aether
            ~Application();
    };
};

#endif
