#ifndef UI_LIST_ITEM_HEADING_HPP
#define UI_LIST_ITEM_HEADING_HPP

#include <functional>
#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI::ListItem {
    // The Heading ListItem represents a heading for a section in the list
    // Has a small rectangle to the left of the string
    class Heading : public List_Item {
        private:
            // Function to execute when help pressed
            std::function<void()> callback;
            // Texture to render
            SDL_Texture * help;
            SDL_Texture * texture;

        public:
            // Constructor takes string and creates texture and an optional callback
            // Specifying a callback function will show/activate the help icon
            Heading(std::string, std::function<void()> = nullptr);

            void draw();

            // Checks if help button pressed and if so executes callback function
            void pressed();

            // Destructor destroys texture
            ~Heading();
    };
};

#endif