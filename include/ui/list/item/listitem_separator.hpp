#ifndef UI_LIST_ITEM_SEPARATOR_HPP
#define UI_LIST_ITEM_SEPARATOR_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI::ListItem {
    // The Separator ListItem is used to separate entries in a list.
    class Separator : public List_Item {
        public:
            // Constructor accepts height or defaults to 45px
            Separator(int = 45);

            // Draw does nothing
            void draw();

            // Destructor destroys texture
            ~Separator();
    };
}

#endif