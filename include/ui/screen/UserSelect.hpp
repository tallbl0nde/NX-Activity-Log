#ifndef SCREEN_USERSELECT_HPP
#define SCREEN_USERSELECT_HPP

#include "Application.hpp"
#include "User.hpp"

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

            // Stores copy of user vector
            std::vector<User *> users;

        public:
            // Constructor takes display element and vector of User objects and sets up elements
            UserSelect(Main::Application *, std::vector<User *>);

            // User images + names are handled + elements created on load
            void onLoad();
            // Undoes everything done on onLoad to free memory
            void onUnload();
    };
};

#endif