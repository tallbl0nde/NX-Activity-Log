#ifndef SCREEN_SETTINGS_HPP
#define SCREEN_SETTINGS_HPP

#include "list.hpp"
#include "screen.hpp"
#include "sidemenu.hpp"
#include "User.hpp"

namespace Screen {
    class Settings : public UI::Screen {
        private:
            // List object
            UI::List * list;
            // Left menu object
            UI::SideMenu * menu;
            // Used to print/render user name and image
            User * user;

        // See ui/screen.hpp for what these functions do
        public:
            // Additionally takes a user object
            Settings(bool *, User *);

            void event();

            void update(uint32_t dt);

            void draw();

            ScreenID change();

            ~Settings();
    };
}

#endif