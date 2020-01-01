#ifndef SCREEN_DETAILS_HPP
#define SCREEN_DETAILS_HPP

#include "list.hpp"
#include "screen.hpp"
#include "Title.hpp"
#include "User.hpp"

namespace Screen {
    class Details : public UI::Screen {
        private:
            // Left hand side list
            UI::List * list;

            // Textures
            SDL_Texture * icon;
            SDL_Texture * name;
            SDL_Texture * username;
            SDL_Texture * userimage;

        // See ui/screen.hpp for what these functions do
        public:
            // Takes user and title objects
            Details(User *, Title *);

            // See ui/screen.hpp for what these functions do
            void event(SDL_Event);
            void update(uint32_t dt);
            void draw();

            ~Details();
    };
}

#endif