#ifndef SCREEN_ACTIVITY_HPP
#define SCREEN_ACTIVITY_HPP

#include "list.hpp"
#include "screen.hpp"
#include "sidemenu.hpp"
#include "Title.hpp"
#include "User.hpp"
#include <vector>

// Enum for current active element
enum ActiveElement {
    SideMenu,   // Left menu
    List        // List of games
};

namespace Screen {
    class Activity : public UI::Screen {
        private:
            // List object
            UI::List * list;
            // List scrolling stuff
            bool list_scroll;
            // Left menu object
            UI::SideMenu * menu;
            // Top right time texture
            SDL_Texture * total_hours;
            // Used to print/render user name and image
            User * user;

        // See ui/screen.hpp for what these functions do
        public:
            // Additionally takes a user object
            Activity(bool *, User *, std::vector<Title *>);

            void event();

            void update(uint32_t dt);

            void draw();

            ~Activity();
    };
}

#endif