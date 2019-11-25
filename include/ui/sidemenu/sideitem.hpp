#ifndef UI_SIDEITEM_HPP
#define UI_SIDEITEM_HPP

#include "drawable.hpp"
#include "SDLHelper.hpp"

namespace UI {
    // SideItem represents an entry/item which will be stored in
    // a side menu.
    class SideItem : public Drawable {
        private:
            // Text texture
            SDL_Texture * text;
            // Self-explanatory
            bool selected;

        public:
            // See ui/screen.hpp for similar explanations of these functions
            // Constructor takes string and creates texture
            SideItem(std::string);

            void draw();

            bool getSelected();

            void setSelected(bool);

            ~SideItem();
    };
};

#endif