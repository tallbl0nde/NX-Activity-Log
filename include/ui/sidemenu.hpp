#ifndef UI_SIDEMENU_HPP
#define UI_SIDEMENU_HPP

#include "drawable.hpp"
#include "SDLHelper.hpp"
#include "sideitem.hpp"
#include <vector>
#include "utils.hpp"

namespace UI {
    // A List contains SideItems and handles the positioning and
    // selection stuff
    class SideMenu : public Drawable {
        private:
            // Vector containing SideItems
            std::vector<SideItem *> items;
            // Index of current element
            size_t pos;
            // Current held button
            uint8_t held_button;
            uint32_t last_tap;

            // Position of element touched
            int t_pos;

            // Address of touch status variable
            bool * t_active;

            // Is this the active element?
            bool is_active;

            // Change pos to parameter
            void setPos(size_t);

        public:
            // The constructor does not accept SideItems
            SideMenu(bool *, int, int, int, int);

            // Add an item to the list
            void addItem(SideItem *);

            // Handles touch events
            void touched(uint32_t, float, float);

            // Handles button events
            void button(uint8_t, uint8_t);

            // Set active state
            void setActive(bool);

            void draw();

            void update(uint32_t);

            // Destructor frees stored SideItems
            ~SideMenu();
    };
};

#endif