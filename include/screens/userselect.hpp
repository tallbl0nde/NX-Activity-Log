#ifndef SCREEN_USERSELECT_HPP
#define SCREEN_USERSELECT_HPP

#include "screen.hpp"
#include <string>
#include "User.hpp"
#include <vector>

namespace Screen {
    class UserSelect : public UI::Screen {
        private:
            // Fading value
            int fade;

            // List object
            UI::List * list;

            // Copy of user vector
            std::vector<User *> users;

        public:
            UserSelect(std::vector<User *>);

            // See ui/screen.hpp for what these functions do
            void event();
            void update(uint32_t dt);
            void draw();

            // Used to set theme
            void setTheme(bool);

            ~UserSelect();
    };
}

#endif