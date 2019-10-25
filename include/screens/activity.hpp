#ifndef SCREEN_ACTIVITY_HPP
#define SCREEN_ACTIVITY_HPP

#include "ui/screen.hpp"

namespace Screen {
    class Activity : public UI::Screen {
        private:

        // See ui/screen.hpp for what these functions do
        public:
            Activity(struct Theme *, bool *);

            void event();

            void update(uint32_t dt);

            void draw(SDL_Renderer *);

            ~Activity();
    };
}

#endif