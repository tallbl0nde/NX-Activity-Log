#ifndef UI_LIST_ITEM_TOOLTIP_HPP
#define UI_LIST_ITEM_TOOLTIP_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI::ListItem {
    // The Tooltip ListItem represents a text "tip" which is usually
    // positioned underneath a setting. It is not selectable.
    class ToolTip : public List_Item {
        private:
            // Text texture
            SDL_Texture * texture;
            std::string str;

            // Previous width (used for rendering)
            int prev_w;

        public:
            // Constructor accepts string
            ToolTip(std::string);

            // Draw texture
            void draw();

            // Texture is rerendered when the width is changed
            void setW(int);

            // Destructor destroys texture
            ~ToolTip();
    };
}

#endif