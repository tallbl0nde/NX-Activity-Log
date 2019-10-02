#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 45

// Apparently you can't use libnx colour macros with cout...? (most likely my fault)
#define TEXT_CYAN "\033[36;1m"
#define TEXT_RED "\033[31;1m"
#define TEXT_RESET "\033[0m"
#define TEXT_WHITE "\033[37;1m"

// Move the cursor to specified coords
void moveCursor(int, int);

// Clear the 'middle' section of the console
void clearConsole();

#endif