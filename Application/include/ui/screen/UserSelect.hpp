#ifndef SCREEN_USERSELECT_HPP
#define SCREEN_USERSELECT_HPP

#include "nx/User.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // User select screen shown on launch (if not user page)
    class UserSelect : public Aether::Screen {
        private:
            Main::Application * app;

            // Need pointer to list in order to update
            Aether::List * list;
            Aether::Image * updateElm;

            // Stores copy of user vector
            std::vector<NX::User *> users;

        public:
            // Constructor takes window element and vector of User objects and sets up elements
            UserSelect(Main::Application *, std::vector<NX::User *>);

            // User images + names are handled + elements created on load
            void onLoad();
            // Undoes everything done on onLoad to free memory
            void onUnload();
    };
};

#endif