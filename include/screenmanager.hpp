#ifndef SCREENMANAGER_HPP
#define SCREENMANAGER_HPP

#include "activity.hpp"
#include "details.hpp"
#include "error.hpp"
#include "loading.hpp"
#include "settings.hpp"
#include <stack>

// The screen manager is a singleton class used to swap between
// screens at any point in the code/program using a "ScreenID".
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

        // Stack storing screens to return to
        std::stack<UI::Screen *> stack;

    public:
        // (Creates and) returns instance pointer
        static ScreenManager * getInstance();

        // "Overwrite" current screen
        void setScreen(UI::Screen *);

        // Push current screen on stack to return to
        void pushScreen();

        // Pop screen from stack and return to it
        void popScreen();

        // Wrappers for current screen functions
        void event();
        void update(uint32_t);
        void draw();

        // Returns running
        bool loop();
        // Set running to false (ie. stop loop)
        void stopLoop();
};

#endif