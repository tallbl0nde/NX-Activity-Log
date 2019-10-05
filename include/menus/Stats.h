#ifndef MENU_STATS
#define MENU_STATS

#include <vector>

#include "menu.h"
#include "Title.h"

enum SortType {
    HoursAsc,       // Play time (ascending)
    LaunchAsc,      // Launches (ascending)
    FirstPlayed,    // First played (earliest first)
    LastPlayed,     // Last played (most recent first)
    AlphaAsc        // Alphabetically (A-Z)
};

class Menu_Stats : public Menu{
    private:
        std::vector<Title *> titles;
        SortType sort;
        unsigned int page;
        unsigned int pages;
        // Sort titles
        void sortTitles();
    public:
        Menu_Stats(std::vector<Title *>);
        virtual MenuType draw(u64);
};

#endif