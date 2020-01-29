#ifndef SCREEN_SETTINGS_HPP
#define SCREEN_SETTINGS_HPP

#include "Application.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // Application Settings screen
    class Settings : public Aether::Screen {
        private:
            // Pointer to main app object for config
            Main::Application * app;

            // Pointers to elements
            Aether::Text * heading;
            Aether::Image * image;
            Aether::List * list;

            // Choose sort overlay
            Aether::PopupList * sortOverlay;
            // Choose theme overlay
            Aether::PopupList * themeOverlay;

            // Set elements and highlight one in overlay
            void setupSortOverlay();
            void setupThemeOverlay();

        public:
            // Passed main application object
            Settings(Main::Application *);

            // Prepare user-specific elements
            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();

            // Deletes overlays
            ~Settings();
    };
};

#endif