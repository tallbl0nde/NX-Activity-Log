#include "Error.h"

Menu_Error::Menu_Error(){
    redraw = true;
}

MenuType Menu_Error::draw(u64 kDown){
    if (redraw){
        moveCursor(2, 4);
        printf("An error has occurred :(");
        moveCursor(2, 5);
        printf("See below for details, press + to exit");
        redraw = false;
    }

    return M_Error;
}

Menu_Error::~Menu_Error(){

}