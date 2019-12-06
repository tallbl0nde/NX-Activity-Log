#ifndef UI_SELECTION_HPP
#define UI_SELECTION_HPP

#include "controls.hpp"
#include "list.hpp"
#include "navigable.hpp"
#include "SDLHelper.hpp"
#include <string>

namespace UI {
    // The Selection class is a panel that comes up at the bottom of the screen
    // and is used to choose an item out of a list.
    class Selection : public Drawable {
        private:
            // Controls object
            Controls * controls;
            // List object
            List * list;
            // Title texture
            SDL_Texture * title;
            // Is this active?
            bool is_active;

        public:
            // Constructor takes title string and vector of strings for items
            Selection(bool *, std::string, std::vector<std::string>);

            void event(SDL_Event);
            void update(uint32_t);
            void draw();

            // Returns when closed (by pressing b)
            bool closed();

            // Return the index of chosen item
            int getChosen();

            // Destructor deletes list
            ~Selection();
    };
};

#endif