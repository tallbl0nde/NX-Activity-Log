#ifndef SCREEN_ACTIVITY_HPP
#define SCREEN_ACTIVITY_HPP

#include "ui/screen.hpp"
#include "User.hpp"

namespace Screen {
    class Activity : public UI::Screen {
        private:
            // Used to print/render user name and image
            User * user;

        // See ui/screen.hpp for what these functions do
        public:
            // Additionally takes a user object
            Activity(SDL_Renderer *, struct Theme *, bool *, User *);

            void event();

            void update(uint32_t dt);

            void draw();

            ~Activity();
    };
}

#endif