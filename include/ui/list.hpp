#ifndef UI_LIST_HPP
#define UI_LIST_HPP

#include "listitem.hpp"
#include <SDL2/SDL.h>
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
    class List {
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

        public:
            // The constructor does not accept ListItems
            List();

            // Add an item to the list
            void addItem(ListItem *);

            void update(uint32_t);

            // Draw at x, y with w, h
            void draw(int, int, int, int);

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