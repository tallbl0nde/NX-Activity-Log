#ifndef UI_SIDEITEM_HPP
#define UI_SIDEITEM_HPP

#include <SDL2/SDL.h>

namespace UI {
    // SideItem represents an entry/item which will be stored in
    // a side menu.
    class SideItem {
        private:
            // Text texture
            SDL_Texture * text;
            // Self-explanatory
            bool selected;

        public:
            // See ui/screen.hpp for similar explanations of these functions
            // Constructor takes string and creates texture
            SideItem(std::string);

            void update(uint32_t);

            void draw(int, int, int, int);

            void setSelected(bool);

            ~SideItem();
    };
};

#endif