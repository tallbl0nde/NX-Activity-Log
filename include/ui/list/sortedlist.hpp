#ifndef UI_SORTEDLIST_HPP
#define UI_SORTEDLIST_HPP

#include "list.hpp"
#include "SDLHelper.hpp"

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
    // A sorted list essentially is just a list but can sort the items
    class SortedList : public List {
        private:
            // Current sorting type
            SortType sorting;
            // Texture for sorting text
            SDL_Texture * sort_text;

            // Sorts with given type
            void sort(SortType);

        public:
            // The constructor does not accept ListItems
            SortedList(bool *, int, int, int, int);

            void addItem(List_Item *);

            // Draw at stored coords
            void draw();

            // Handles button presses
            void handleButton(uint8_t, uint8_t);

            // Destructor frees stored ListItems
            ~SortedList();
    };
};

#endif