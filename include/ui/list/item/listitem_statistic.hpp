#ifndef UI_LIST_ITEM_STATISTIC_HPP
#define UI_LIST_ITEM_STATISTIC_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI::ListItem {
    // The Statistic ListItem shows two strings: one for a name in white
    // and another for a value in the accent colour
    class Statistic : public List_Item {
        private:
            // Textures for text
            SDL_Texture * name;
            SDL_Texture * value;

        public:
            // Constructor takes strings and creates textures
            Statistic(std::string, std::string);

            void draw();

            // Destructor destroys textures
            ~Statistic();
    };
};

#endif