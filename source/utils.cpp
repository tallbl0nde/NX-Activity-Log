#include "utils.h"
#include <string>

void moveCursor(int x, int y){
    std::string s = "\033[";
    s += std::to_string(y);
    s += ";";
    s += std::to_string(x);
    s += "H";
    std::cout << s;
}

void clearConsole(){
    moveCursor(0, 3);
    for (int y = 2; y < CONSOLE_HEIGHT-4; y++){
        for (int x = 0; x < CONSOLE_WIDTH; x++){
            printf(" ");
        }
    }
}