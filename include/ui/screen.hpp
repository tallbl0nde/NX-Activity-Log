#ifndef UI_SCREEN_HPP
#define UI_SCREEN_HPP

#include "theme.hpp"
#include "ui/controls.hpp"

// Default font sizes
#define HEADING_FONT_SIZE 28
#define BODY_FONT_SIZE 20

namespace UI {
    // Screen represents a application screen and has it's own event handling
    // Functions called in order: event(), update(), draw()
    class Screen {
        protected:
            // Pointer to variable to break loop
            bool * loop;
            // Controls element
            Controls * controls;

        public:
            // Constructor is used to initialize variables
            Screen(bool *);

            // Called before update, handles any events
            virtual void event() = 0;

            // Called before draw, update any relevant variables here
            // dt is the time since last update call
            virtual void update(uint32_t dt) = 0;

            // Actually draw/render stuff
            virtual void draw() = 0;

            // Destructor is used to free variables/clean up
            virtual ~Screen();
    };
}

#endif