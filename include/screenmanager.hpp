#ifndef SCREENMANAGER_HPP
#define SCREENMANAGER_HPP

#include "allactivity.hpp"
#include "details.hpp"
#include "error.hpp"
#include <functional>
#include "recentactivity.hpp"
#include "selection.hpp"
#include "settings.hpp"
#include <stack>
#include "userselect.hpp"

// The screen manager is a singleton class used to swap between
// screens at any point in the code/program and handling panels.
class ScreenManager {
    private:
        // Single instance of this class
        static ScreenManager * instance;

        // Constructor is private!
        ScreenManager();

        // True when screen has changed (reset after "cycle")
        bool changed;

        // Bool used to control main loop
        bool looping;

        // Current screen
        UI::Screen * screen_ptr;

        // Pointer to "selection" panel
        // Can be invoked and "covers" screen
        UI::Selection * selection;
        bool selection_active;
        std::function<void(int)> selection_callback;

        // Stack storing screens to return to
        std::stack<UI::Screen *> stack;

    public:
        // THIS WON'T BE PUBLIC FOREVER
        // Bool representing if touch is active
        bool touch_active;

        // (Creates and) returns instance pointer
        static ScreenManager * getInstance();

        // "Overwrite" current screen
        void setScreen(UI::Screen *);
        // Push current screen on stack to return to
        void pushScreen();
        // Pop screen from stack and return to it
        void popScreen();

        // Activates selection panel
        void createSelection(std::string, std::vector<std::string>, int, std::function<void(int)>);

        // Wrappers for current screen functions
        void event();
        void update(uint32_t);
        void draw();

        // Returns running
        bool loop();
        // Set running to false (ie. stop loop)
        void stopLoop();

        // Frees any screens on the stack and current screen
        // Call upon exiting!
        void free();
};

#endif