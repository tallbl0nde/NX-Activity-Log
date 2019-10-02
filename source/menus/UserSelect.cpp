#include "UserSelect.h"

Menu_UserSelect::Menu_UserSelect(std::vector<User *> users, int * sel){
    this->users = users;
    this->selectedUser = sel;
    yPos = 0;
    redraw = true;
}

MenuType Menu_UserSelect::draw(u64 kDown){
    // Move up
    if (kDown & KEY_UP){
        if (yPos > 0){
            yPos--;
        }
        redraw = true;
    }

    // Move down
    if (kDown & KEY_DOWN){
        if (yPos < users.size()-1){
            yPos++;
        }
        redraw = true;
    }

    // 'A' pressed (select user)
    if (kDown & KEY_A){
        *selectedUser = yPos;
        return M_Stats;
    }

    // Redraw if necessary
    if (redraw){
        moveCursor(2, 4);
        std::cout << "Select which user's data to view.";
        for (unsigned int i = 0; i < users.size(); i++){
            moveCursor(2, 6 + i);
            std::cout << "  " << users[i]->getUsername();
        }
        moveCursor(2, 6 + yPos);
        std::cout << ">";
        redraw = false;
    }

    return M_UserSelect;
}

Menu_UserSelect::~Menu_UserSelect(){

}