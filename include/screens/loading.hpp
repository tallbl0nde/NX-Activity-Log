#ifndef SCREEN_LOADING_HPP
#define SCREEN_LOADING_HPP

#include <string>
#include "ui/screen.hpp"

namespace Screen {
    class Loading : public UI::Screen {
        private:
            // Pointer to status variable
            char * status;

        // See ui/screen.hpp for what these functions do
        public:
            Loading(struct Theme *, bool *, char *);

            void event();

            void update(uint32_t dt);

            void draw();

            ~Loading();
    };
}

#endif