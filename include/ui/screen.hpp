#ifndef UI_SCREEN_HPP
#define UI_SCREEN_HPP

#include "controls.hpp"
#include "drawable.hpp"

// Default font sizes
#define HEADING_FONT_SIZE 28
#define BODY_FONT_SIZE 20

// Enum for current active element
enum ActiveElement {
    SideMenu,   // Left menu
    List        // List of games
};

namespace UI {
    // Screen represents an application screen and has it's own event handling
    // Functions called in order: event(), update(), draw()
    class Screen : public Drawable {
        protected:
            // Controls element
            Controls * controls;

            // Variables indicating touch/button state
            static bool touch_active;
            unsigned int active_element;

        public:
            // Constructor is used to initialize variables
            Screen();

            // Called before update, handles any events
            virtual void event() = 0;

            // Called before draw, update any relevant variables here
            // dt is the time since last update call
            virtual void update(uint32_t dt);

            // Actually draw/render stuff
            virtual void draw() = 0;

            // Destructor is used to free variables/clean up
            virtual ~Screen();
    };
}

#endif