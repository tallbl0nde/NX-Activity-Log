#ifndef ELEMENT_LISTACTIVITY_HPP
#define ELEMENT_LISTACTIVITY_HPP

#include "Aether/Aether.hpp"

namespace CustomElm {
    // List item containing game icon and a few left/right aligned strings
    class ListActivity : public Aether::Element {
        private:
            // Pointers to elements
            Aether::Rectangle * topR;
            Aether::Rectangle * bottomR;
            Aether::Image * icon;
            Aether::Text * title;
            Aether::Text * playtime;
            Aether::Text * mutedLeft;
            Aether::Text * rank;
            Aether::Text * mutedRight;

            // Sets width of applicable elements
            void setTextWidth();

        public:
            // Constructor positions elements
            ListActivity();

            // Set icon (deleted internally)
            void setImage(Aether::Image *);

            // Set strings
            void setTitle(std::string);
            void setPlaytime(std::string);
            void setLeftMuted(std::string);
            void setRightMuted(std::string);
            void setRank(std::string);

            // Set colours
            void setTitleColour(Aether::Colour);
            void setPlaytimeColour(Aether::Colour);
            void setMutedColour(Aether::Colour);
            void setLineColour(Aether::Colour);

            // Adjusting width must move/adjust children
            void setW(int);
    };
};

#endif