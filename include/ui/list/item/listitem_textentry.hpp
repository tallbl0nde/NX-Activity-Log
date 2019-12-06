#ifndef UI_LIST_ITEM_TEXTENTRY_HPP
#define UI_LIST_ITEM_TEXTENTRY_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI::ListItem {
    // A text entry list item simply stores text and nothing else
    class TextEntry : public List_Item {
        private:
            // Texture for text
            SDL_Texture * tex;

        public:
            // Creates texture using string
            TextEntry(std::string);

            void draw();

            // Frees texture
            ~TextEntry();
    };
};

#endif