#ifndef SETTINGLISTITEM_HPP
#define SETTINGLISTITEM_HPP

#include "drawable.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI {
    // A settinglistitem represents the "bar"/option in a list
    // which will toggle a setting when tapped
    class SettingListItem : public Drawable {
        private:
            // Textures for text
            SDL_Texture * text;
            SDL_Texture * value;
            SDL_Texture * tip;
            std::string tip_text;

            // Function pointer
            std::string (*selected_func)(bool);

        public:
            // Constructor takes strings, function to call when tapped
            SettingListItem(std::string, std::string(*)(bool), std::string = "");

            // Called when tapped/pressed (calls func and updates value)
            void pressed();

            // Draw the item
            void draw();

            // Rerender tip texture (with new width)
            // I want to avoid this somehow
            void renderTip();

            // Return box height (used for highlight)
            unsigned int getBoxHeight();

            // Destructor destroys textures
            ~SettingListItem();
    };
};

#endif