#ifndef SCREEN_RECENTACTIVITY_HPP
#define SCREEN_RECENTACTIVITY_HPP

#include "Application.hpp"
#include "SortedList.hpp"
#include "Time.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

// Period to view stats for
enum ViewType {
    Day,
    Month,
    Year
};

namespace Screen {
    class RecentActivity : public Aether::Screen {
        private:
            // Pointer to app for theme
            Main::Application * app;

            // Pointers to elements
            Aether::Text * heading;
            Aether::Text * hours;
            Aether::Image * image;
            Aether::List * list;
            Aether::Text * noStats;

            // Overlay to choose viewType
            Aether::PopupList * viewOverlay;

            // Timestamp to start showing from
            struct tm startTime;
            // Current period of time to view
            ViewType viewType;

            // Change the list heading and populate with stats
            void updateList();

            // Increase/decrease time based on current period type
            // Returns true if changed
            bool updateStartTime(int);

            // Populate overlay and show
            void setupOverlay();

        public:
            // Passed main application object
            RecentActivity(Main::Application *);

            // Prepare user-specific elements + list
            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();

            // Delete overlay
            ~RecentActivity();
    };
};

#endif