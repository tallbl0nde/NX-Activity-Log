#ifndef UI_CONTROLS_HPP
#define UI_CONTROLS_HPP

#include "drawable.hpp"
#include "SDLHelper.hpp"
#include <string>
#include "utils.hpp"

// Struct stored in vector which stores button text and position
struct Button {
    SDL_Texture * texture = nullptr;
    int tex_x = 0;
    int tex_w = 0;
    int tex_h = 0;
    char pos = 0;
    bool show = false;
    bool enabled = true;
    bool touched = false;
};

// Dummy controller ID for button presses
#define CONTROLS_ID 99

namespace UI {
    // Controls is the element where controls are shown along the bottom of the screen
    // Buttons can easily be added/removed and enabled/disabled
    // Note this is does not handle actual button input, only touch and displays the UI
    class Controls : public Drawable {
        private:
            // Array storing button structs
            struct Button buttons[KEY_MAP_SIZE];

        public:
            // Constructor initializes vector and creates fonts + rightmost position
            Controls(int, int, int, int);

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
            void draw();

            // Handles touch events
            void touched(uint32_t, float, float);

            ~Controls();
    };
}

#endif