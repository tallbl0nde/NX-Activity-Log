#ifndef NAVIGABLE_HPP
#define NAVIGABLE_HPP

#include "drawable.hpp"
#include "utils.hpp"

// Struct stores information about a button
struct ButtonInfo {
    bool is_pressed;        // True if button is pressed
    uint32_t time_held;     // Time held down in ms
};

namespace UI {
    // A "Navigable" class extends a Drawable by adding
    // variables and methods regarding controller navigation
    class Navigable : public Drawable {
        protected:
            // Address of variable indicating if touch is active
            bool * touch_active;

            // Is this element active/selected?
            bool is_active;

            // Variables for held buttons
            struct ButtonInfo button[KEY_MAP_SIZE];

        public:
            // Constructor initializes these variables
            Navigable(bool *, int = 0, int = 0, int = -100, int = -100);

            // Update increments counters if button is held
            virtual void update(uint32_t);

            // Button function handles setting held button(s)
            // Takes button SDL value and button SDL state
            virtual void handleButton(uint8_t, uint8_t);

            // Set active variable
            // Resets buttoninfo when set to false
            void setActive(bool);

            // Destructor does nothing
            ~Navigable();
    };
};

#endif