#ifndef UI_LIST_ITEM_OPTION_HPP
#define UI_LIST_ITEM_OPTION_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI::ListItem {
    // The Option ListItem represents a option that can be toggled.
    // It stores a function to call when pressed and the string to show.
    class Option : public List_Item {
        private:
            // Textures for text
            SDL_Texture * text;

            // Function pointer
            std::string (*selected_func)(bool);

        public:
            // Constructor takes string and function to call when tapped
            Option(std::string, std::string(*)(bool));

            void draw();

            // Calls stored function pointer
            void pressed();

            // Destructor destroys textures
            ~Option();
    };
};

#endif