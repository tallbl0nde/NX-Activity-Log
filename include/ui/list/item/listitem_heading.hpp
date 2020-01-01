#ifndef UI_LIST_ITEM_HEADING_HPP
#define UI_LIST_ITEM_HEADING_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI::ListItem {
    // The Heading ListItem represents a heading for a section in the list
    // Has a small rectangle to the left of the string
    class Heading : public List_Item {
        private:
            // Texture to render
            SDL_Texture * texture;

        public:
            // Constructor takes string and creates texture
            Heading(std::string);

            void draw();

            // Destructor destroys texture
            ~Heading();
    };
};

#endif