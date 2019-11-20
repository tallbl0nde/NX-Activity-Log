#ifndef UI_LIST_HPP
#define UI_LIST_HPP

#include "listitem.hpp"
#include "SDLHelper.hpp"
#include <vector>

// Enumerations for sorting methods
enum SortType {
    AlphaAsc,       // Alphabetically (A-Z)
    HoursAsc,       // Play time (ascending)
    HoursDec,       // Play time (descending)
    LaunchAsc,      // Launches (ascending)
    LaunchDec,      // Launches (descending)
    FirstPlayed,    // First played (earliest first)
    LastPlayed,     // Last played (most recent first)
};

namespace UI {
    // A List contains ListItems and handles the positioning and
    // selection stuff. A small navigation bar is also shown
    class List : public Drawable {
        private:
            // Vector containing ListItems
            std::vector<ListItem *> items;
            // Used for scrolling
            unsigned int pos;
            unsigned int max_pos;
            // Current sorting type
            SortType sorting;
            // Texture for sorting text
            SDL_Texture * sort_text;
            // Scrolling vars
            bool is_scrolling;
            bool is_touched;
            float scroll_v;

        public:
            // The constructor does not accept ListItems
            List(int, int, int, int);

            // Add an item to the list
            void addItem(ListItem *);

            void update(uint32_t);

            // Draw at stored coords
            void draw();

            // Handles touches
            void touched(uint32_t, float, float, float = 0, float = 0);

            // Returns if scrolling
            bool isTouched();

            // Handles button presses
            void button(uint8_t, uint8_t);

            // Return pos
            unsigned int getPos();

            // Change pos to parameter
            void setPos(unsigned int);

            // Returns current sort type
            SortType getSorting();

            // Sorts with given type
            void sort(SortType);

            // Destructor frees stored ListItems
            ~List();
    };
};

#endif