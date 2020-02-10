#ifndef SCREEN_RECENTACTIVITY_HPP
#define SCREEN_RECENTACTIVITY_HPP

#include "Application.hpp"
#include "SortedList.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
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

            // Change the list heading and populate with stats
            void updateList();

        public:
            // Passed main application object
            RecentActivity(Main::Application *);

            // Updates list when time is changed
            void update(uint32_t);

            // Prepare user-specific elements + list
            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();
    };
};

#endif