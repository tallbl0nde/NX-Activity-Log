#ifndef MENU_USERSELECT
#define MENU_USERSELECT

#include <vector>

#include "menu.h"
#include "User.h"

class Menu_UserSelect : public Menu{
    private:
        std::vector<User *> users;
        int * selectedUser;
        unsigned int yPos;
    public:
        Menu_UserSelect(std::vector<User *>, int *);
        virtual MenuType draw(u64);
        int getSelectedUser();
        virtual ~Menu_UserSelect();
};

#endif