#ifndef UI_LIST_HPP
#define UI_LIST_HPP

#include "listitem.hpp"
#include "navigable.hpp"
#include "SDLHelper.hpp"
#include <vector>

namespace UI {
    // A List contains ListItems and handles the positioning and
    // selection. A small navigation bar is also shown if scrolling
    // is required.
    class List : public Navigable {
        protected:
            // Vector of items
            std::vector<List_Item *> items;
            // Index of top drawn element
            unsigned int top_item;
            // Offset of list from y (used for headings)
            unsigned int offset;

            // Scroll variables
            bool is_scrolling;
            unsigned int scroll_pos;
            unsigned int max_scroll_pos;
            float scroll_velocity;

            // Other navigation variables
            bool is_touched;
            unsigned int highlight_item;
            int touched_item;

            // Position of pressed touch
            int start_touch_y;

            // Chosen element
            bool reset_chosen;
            int chosen;

        public:
            // The constructor does not accept items
            List(bool *, int, int, int, int, int = 0);

            // Add an item to the list
            virtual void addItem(List_Item *);

            // See drawable.hpp
            void update(uint32_t);
            void draw();

            // Handles touches
            void touched(uint32_t, float, float, float = 0, float = 0);

            // Returns if scrolling
            bool isTouched();

            // Handles button presses
            void handleButton(uint8_t, uint8_t);

            // Set scroll_pos (prevents overflow)
            void setScrollPos(unsigned int);

            // Returns index of chosen element or -1 if nothing is pressed
            int getChosen();

            // Destructor frees stored ListItems
            ~List();
    };
};

#endif