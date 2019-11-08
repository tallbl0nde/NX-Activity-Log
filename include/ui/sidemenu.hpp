#ifndef UI_SIDEMENU_HPP
#define UI_SIDEMENU_HPP

#include <SDL2/SDL.h>
#include "sideitem.hpp"
#include <vector>

namespace UI {
    // A List contains SideItems and handles the positioning and
    // selection stuff
    class SideMenu {
        private:
            // Vector containing SideItems
            std::vector<SideItem *> items;
            // Index of current element
            size_t pos;

        public:
            // The constructor does not accept ListItems
            SideMenu();

            // Add an item to the list
            void addItem(SideItem *);

            void update(uint32_t);

            // Draw at x, y with w, h
            void draw(int, int, int, int);

            // Return pos
            size_t getPos();

            // Change pos to parameter
            void movePos(size_t);

            // Destructor frees stored ListItems
            ~SideMenu();
    };
};

#endif