#ifndef ELEMENT_LISTSESSION_HPP
#define ELEMENT_LISTSESSION_HPP

#include "Aether/Aether.hpp"

namespace CustomElm {
    class ListSession : public Aether::Element {
        private:
            // Pointers to elements
            Aether::Rectangle * top;
            Aether::Rectangle * bottom;
            Aether::Text * time;
            Aether::Text * playtime;
            Aether::Text * percentage;

            void positionElements();

        public:
            // Constructor sets up elements
            ListSession();

            // Set colours
            void setLineColour(Aether::Colour);
            void setPercentageColour(Aether::Colour);
            void setPlaytimeColour(Aether::Colour);
            void setTimeColour(Aether::Colour);

            // Set strings
            void setPercentageString(std::string);
            void setPlaytimeString(std::string);
            void setTimeString(std::string);

            // Adjusting width repositions elements
            void setW(int);
    };
};

#endif