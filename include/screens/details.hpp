#ifndef SCREEN_DETAILS_HPP
#define SCREEN_DETAILS_HPP

#include "screen.hpp"
#include "Title.hpp"
#include "User.hpp"

namespace Screen {
    class Details : public UI::Screen {
        private:
            // Textures
            SDL_Texture * first_played;
            SDL_Texture * last_played;
            SDL_Texture * launches;
            SDL_Texture * playtime;

            SDL_Texture * name;
            SDL_Texture * username;
            // Used to print/render user name and image
            User * user;
            // Title whose information is displayed
            Title * title;

        // See ui/screen.hpp for what these functions do
        public:
            // Takes user and title object
            Details(User *, Title *);

            void event();

            void update(uint32_t dt);

            void draw();

            ~Details();
    };
}

#endif