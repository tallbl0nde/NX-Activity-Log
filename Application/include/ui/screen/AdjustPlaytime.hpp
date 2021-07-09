#ifndef SCREEN_ADJUSTPLAYTIME_HPP
#define SCREEN_ADJUSTPLAYTIME_HPP

#include "Aether/Aether.hpp"

namespace Main {
    class Application;
};

namespace Screen {
    // Screen allowing user to adjust the playtime displayed in All Activity.
    class AdjustPlaytime : public Aether::Screen {
        private:
            // Pointer to application object
            Main::Application * app;

            // List object
            Aether::List * list;

            // User
            Aether::Image * userimage;
            Aether::Text * username;

        public:
            // Constructs the screen
            AdjustPlaytime(Main::Application *);

            // Create relevant elements on load
            void onLoad();

            // Undo onLoad();
            void onUnload();
    };
};

#endif
