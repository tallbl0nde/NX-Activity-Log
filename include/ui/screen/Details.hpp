#ifndef SCREEN_DETAILS_HPP
#define SCREEN_DETAILS_HPP

#include "Application.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // Detailed activity screen
    class Details : public Aether::Screen {
        private:
            // Pointer to app for theme
            Main::Application * app;

            // Pointers to elements
            Aether::Image * icon;
            Aether::List * list;
            Aether::Text * title;
            Aether::Image * userimage;
            Aether::Text * username;

            // Summary items (to edit value directly)
            Aether::ListOption * loPlayTime;
            Aether::ListOption * loAverageTime;
            Aether::ListOption * loLaunched;
            Aether::ListOption * loFirstTime;
            Aether::ListOption * loLastTime;

            // MessageBox overlay (used for multiple things)
            Aether::MessageBox * msgbox;

            // Prepare msgbox for session help
            void setupSessionHelp();

            // Prepare msgbox for session breakdown
            // Takes PlaySession
            void setupSessionBreakdown(NX::PlaySession);

        public:
            // Sets up elements
            Details(Main::Application *);

            // Create user + title reliant elements
            void onLoad();
            // Free elements created in onLoad()
            void onUnload();
    };
};

#endif