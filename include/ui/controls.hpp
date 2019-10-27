#ifndef UI_CONTROLS_HPP
#define UI_CONTROLS_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "utils.h"

// Struct stored in vector which stores button text and position
struct Button {
    SDL_Texture * texture = nullptr;
    char pos = 0;
    bool show = false;
    bool enabled = true;
};

namespace UI {
    // Controls is the element where controls are shown along the bottom of the screen
    // Buttons can easily be added/removed and enabled/disabled
    // Note this is does not handle actual button input, only touch and displays the UI
    class Controls {
        private:
            // Array storing button structs
            struct Button buttons[KEY_MAP_SIZE];
            // Fonts
            TTF_Font * extend;
            TTF_Font * stndrd;
            // Renderer
            SDL_Renderer * renderer;

        public:
            // Constructor initializes vector and creates fonts
            Controls(SDL_Renderer *);

            // Add a button (updates text/position if already added)
            // Passed a key, text and position
            void add(HidControllerKeys, std::string, char);

            // Remove a button (will do nothing if already removed)
            void remove(HidControllerKeys);

            // Enable a button (can be touched)
            void enable(HidControllerKeys);

            // Disable a button (dimmed and can't be touched)
            void disable(HidControllerKeys);

            // Draw controls
            // xPos specifies right most point, yPos specifies
            void draw(int, int);

            ~Controls();
    };
}

#endif