#ifndef SCREEN_LOADING_HPP
#define SCREEN_LOADING_HPP

#include "screen.hpp"
#include <string>

namespace Screen {
    class Loading : public UI::Screen {
        private:
            // Fading value
            int fade;

        // See ui/screen.hpp for what these functions do
        public:
            Loading();

            void event();

            void update(uint32_t dt);

            void draw();

            // Returns true when fading animation done
            bool animDone();

            // Used to set theme
            void setTheme(bool);

            ~Loading();
    };
}

#endif