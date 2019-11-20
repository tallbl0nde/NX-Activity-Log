#ifndef UI_SIDEMENU_HPP
#define UI_SIDEMENU_HPP

#include <SDL2/SDL.h>
#include "sideitem.hpp"
#include <vector>
#include "utils.hpp"

namespace UI {
    // A List contains SideItems and handles the positioning and
    // selection stuff
    class SideMenu {
        private:
            // Vector containing SideItems
            std::vector<SideItem *> items;
            // Index of current element
            size_t pos;

            // Change pos to parameter
            void setPos(size_t);

        public:
            // The constructor does not accept SideItems
            SideMenu();

            // Add an item to the list
            void addItem(SideItem *);

            // Handles button events
            void button(uint8_t, uint8_t);

            void update(uint32_t);

            // Draw at x, y with w, h
            void draw(int, int, int, int);

            // Destructor frees stored SideItems
            ~SideMenu();
    };
};

#endif