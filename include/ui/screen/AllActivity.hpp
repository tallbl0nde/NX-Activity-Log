#ifndef SCREEN_ALLACTIVITY_HPP
#define SCREEN_ALLACTIVITY_HPP

#include "Application.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // "All Activity" page
    class AllActivity : public Aether::Screen {
        private:
            // Pointer to main app object for user + titles
            Main::Application * app;

            // Pointers to elements
            Aether::Text * heading;
            Aether::Text * hours;
            Aether::Image * image;
            Aether::List * list;

        public:
            // Passed main application object
            AllActivity(Main::Application *);

            // Prepare user-specific elements
            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();
    };
};

#endif