#ifndef SCREEN_ADJUSTPLAYTIME_HPP
#define SCREEN_ADJUSTPLAYTIME_HPP

#include "Aether/Aether.hpp"

namespace Main {
    class Application;
};

namespace CustomElm {
    class ListAdjust;
};

namespace CustomOvl {
    class PlaytimePicker;
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

            // Adjustment picker
            CustomOvl::PlaytimePicker * picker;

            // Helper to get value string
            std::string getValueString(int);

            // Create and show the adjustment overlay
            void setupPlaytimePicker(const std::string &, size_t, CustomElm::ListAdjust *);

            // Vector of title IDs and their adjustment value
            std::vector<AdjustmentValue> adjustments;

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
