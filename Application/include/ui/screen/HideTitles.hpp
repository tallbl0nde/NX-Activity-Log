#ifndef SCREEN_HIDETITLES_HPP
#define SCREEN_HIDETITLES_HPP

#include "Aether/Aether.hpp"

// Forward declare due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    class HideTitles : public Aether::Screen {
        private:
            // Pointer to application object
            Main::Application * app;

            // Main list element
            Aether::List * list;

            // Titles hidden in sidebar
            Aether::Text * hiddenCountText;
            Aether::TextBlock * hiddenSubText;
            std::vector<uint64_t> hiddenIDs;

            // Update the titles hidden counter
            void updateHiddenCounter();

        public:
            // Constructor takes application object
            HideTitles(Main::Application *);

            // Prepare and show list
            void onLoad();

            // Undo onLoad();
            void onUnload();
    };
};

#endif
