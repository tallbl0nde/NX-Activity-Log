#ifndef SCREEN_DETAILS_HPP
#define SCREEN_DETAILS_HPP

#include "screen.hpp"
#include "Title.hpp"
#include "User.hpp"

namespace Screen {
    class Details : public UI::Screen {
        private:
            // Textures
            SDL_Texture * av_heading;
            SDL_Texture * fp_heading;
            SDL_Texture * lp_heading;
            SDL_Texture * l_heading;
            SDL_Texture * p_heading;
            SDL_Texture * index;

            SDL_Texture * average_time;
            SDL_Texture * first_played;
            SDL_Texture * last_played;
            SDL_Texture * launches;
            SDL_Texture * playtime;

            SDL_Texture * name;
            SDL_Texture * username;
            // Used to print/render user name and image
            User * user;
            // List of titles
            std::vector<Title *> titles;
            unsigned int title;

        // See ui/screen.hpp for what these functions do
        public:
            // Takes user and titles with index
            Details(User *, std::vector<Title *>, unsigned int);

            // Renders relevant textures for current title object
            void renderTextures();

            void event();

            void update(uint32_t dt);

            void draw();

            ~Details();
    };
}

#endif