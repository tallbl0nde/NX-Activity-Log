#ifndef ELEMENT_LISTCOLOUR_HPP
#define ELEMENT_LISTCOLOUR_HPP

#include "Aether/Aether.hpp"

namespace CustomElm {
    // A list element storing a string, colour hex value and colour 'box'
    class ListColour : public Aether::Element {
        private:
            Aether::Rectangle * topR;
            Aether::Rectangle * bottomR;
            Aether::Text * text;
            Aether::Text * hex;
            Aether::Rectangle * colour;

        public:
            // Pass text and callback
            ListColour(std::string, std::function<void()>);

            // Set colour of box and hex value
            void setColour(Aether::Colour);

            // Setter for colours
            void setLineColour(Aether::Colour);
            void setTextColour(Aether::Colour);

            // Adjusting width (when added to list)
            // must adjust line width and reposition elements
            void setW(int);
    };
};

#endif