#ifndef MENU_H
#define MENU_H

#include "utils.h"
#include <switch.h>

enum MenuType {
    M_Dummy,        // Dummy menu
    M_Error,        // Error
    M_UserSelect,   // UserSelect
    M_Stats         // Stats
};

class Menu{
    protected:
        bool redraw;
    public:
        virtual MenuType draw(u64) = 0;
        virtual ~Menu();
};

#endif