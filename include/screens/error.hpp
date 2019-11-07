#ifndef SCREEN_ERROR_HPP
#define SCREEN_ERROR_HPP

#include <string>
#include "ui/screen.hpp"

namespace Screen {
    class Error : public UI::Screen {
        private:
            // Error message to display
            std::string message;

        // See ui/screen.hpp for what these functions do
        public:
            Error(bool *, std::string);

            void event();

            void update(uint32_t dt);

            void draw();

            ~Error();
    };
}

#endif