#ifndef UI_SIDEMENU_HPP
#define UI_SIDEMENU_HPP

#include "navigable.hpp"
#include "SDLHelper.hpp"
#include "sideitem.hpp"
#include "utils.hpp"
#include <vector>

namespace UI {
    // A List contains SideItems and handles the positioning and
    // selection stuff
    class SideMenu : public Navigable {
        private:
            // Vector containing SideItems
            std::vector<SideItem *> items;
            // Index of current element
            size_t pos;

            // Position of element touched
            int t_pos;

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
            void handleButton(uint8_t, uint8_t);

            void draw();

            void update(uint32_t);

            // Destructor frees stored SideItems
            ~SideMenu();
    };
};

#endif