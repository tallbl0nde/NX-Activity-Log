#ifndef MENU_ERROR
#define MENU_ERROR

#include "menu.h"

class Menu_Error : public Menu{
    public:
        Menu_Error();
        virtual MenuType draw(u64);
        virtual ~Menu_Error();
};

#endif