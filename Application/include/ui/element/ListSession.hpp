#ifndef ELEMENT_LISTSESSION_HPP
#define ELEMENT_LISTSESSION_HPP

#include "Aether/Aether.hpp"

namespace CustomElm {
    class ListSession : public Aether::AsyncItem {
        private:
            // Height of item
            static const int height = 85;

            // Font size of all text
            static const unsigned int fontSize = 20;

            // Pointers to elements
            Aether::Rectangle * top;
            Aether::Rectangle * bottom;
            Aether::Text * time;
            Aether::Text * playtime;
            Aether::Text * percentage;

            // Sets width of text
            void positionElements();

            // Helper for updating text
            void processText(Aether::Text * &, std::function<Aether::Text * ()>);

        public:
            // Constructor sets up elements
            ListSession();

            // Override update() to also change the alpha of lines
            // (they are not monitored by AsyncItem)
            void update(uint32_t);

            // Set colours
            void setLineColour(Aether::Colour);
            void setPercentageColour(Aether::Colour);
            void setPlaytimeColour(Aether::Colour);
            void setTimeColour(Aether::Colour);

            // Set strings
            void setPercentageString(std::string);
            void setPlaytimeString(std::string);
            void setTimeString(std::string);
    };
};

#endif
