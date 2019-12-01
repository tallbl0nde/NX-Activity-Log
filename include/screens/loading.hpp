#ifndef SCREEN_LOADING_HPP
#define SCREEN_LOADING_HPP

#include "screen.hpp"
#include <string>

namespace Screen {
    class Loading : public UI::Screen {
        public:
            Loading();

            // See ui/screen.hpp for what these functions do
            void event();
            void update(uint32_t dt);
            void draw();

            ~Loading();
    };
}

#endif