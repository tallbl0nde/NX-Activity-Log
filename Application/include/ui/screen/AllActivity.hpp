#ifndef SCREEN_ALLACTIVITY_HPP
#define SCREEN_ALLACTIVITY_HPP

#include "ui/element/SortedList.hpp"

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
            CustomElm::SortedList * list;
            Aether::Menu * menu;
            Aether::Image * updateElm;

            // Choose sort overlay
            Aether::PopupList * sortOverlay;

            // Set elements and highlight one in overlay
            void setupOverlay();

        public:
            // Passed main application object
            AllActivity(Main::Application *);

            // Prepare user-specific elements
            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();

            // Deletes overlay
            ~AllActivity();
    };
};

#endif