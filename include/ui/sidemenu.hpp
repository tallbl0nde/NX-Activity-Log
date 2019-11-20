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

            // Change pos to parameter
            void setPos(size_t);

        public:
            // The constructor does not accept SideItems
            SideMenu(int, int, int, int);

            // Add an item to the list
            void addItem(SideItem *);

            // Handles button events
            void button(uint8_t, uint8_t);

            void draw();

            // Destructor frees stored SideItems
            ~SideMenu();
    };
};

#endif