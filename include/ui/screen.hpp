#ifndef UI_SCREEN_HPP
#define UI_SCREEN_HPP

#include <SDL2/SDL.h>
#include "theme.h"

namespace UI {
    // Screen represents a application screen and has it's own event handling
    // Functions called in order: event(), update(), draw()
    class Screen {
        protected:
            // The active theme
            struct Theme * theme;
            // Pointer to variable to break loop
            bool * loop;

        public:
            // Constructor is used to initialize variables
            Screen(struct Theme *, bool *);

            // Called before update, handles any events
            virtual void event() = 0;

            // Called before draw, update any relevant variables here
            // dt is the time since last update call
            virtual void update(uint32_t dt) = 0;

            // Actually draw/render stuff
            virtual void draw(SDL_Renderer *) = 0;

            // Destructor is used to free variables/clean up
            ~Screen();
    };
}

#endif