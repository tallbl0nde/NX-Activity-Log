#ifndef UI_LIST_ITEM_SESSION_HPP
#define UI_LIST_ITEM_SESSION_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI::ListItem {
    // The Session ListItem stores 3 textures and is used to
    // portray information about a play session
    class Session : public List_Item {
        private:
            // Textures for text
            SDL_Texture * percentage;
            SDL_Texture * playtime;
            SDL_Texture * time;

        public:
            // Constructor takes relevant information
            // Order: Playtime (s), Total playtime (s), Start time, End time
            Session(u64, u64, u64, u64);

            void draw();

            // Destructor destroys textures
            ~Session();
    };
};

#endif