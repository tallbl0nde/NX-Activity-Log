#ifndef UI_LIST_ITEM_ACTIVITY_HPP
#define UI_LIST_ITEM_ACTIVITY_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"

namespace UI::ListItem {
    // The User ListItem stores an icon and string of a user
    class User : public List_Item {
        private:
            // Rendered textures
            SDL_Texture * icon;
            SDL_Texture * name;

        public:
            // Constructor accepts the texture and name
            User(SDL_Texture *, std::string);

            // Draw function is only one defined
            void draw();

            // Destructor deletes texture
            ~User();
    };
};

#endif