#ifndef ELEMENT_LISTACTIVITY_HPP
#define ELEMENT_LISTACTIVITY_HPP

#include "Aether/Aether.hpp"

namespace CustomElm {
    // List item containing game icon and a few left/right aligned strings
    class ListActivity : public Aether::Element {
        private:
            // Height of item in pixels
            static const int height = 120;

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
            void positionItems();

            // Small helper to return whether the given element is ready to be shown
            bool textReady(Aether::Text *);

        public:
            // Constructor positions elements
            ListActivity();

            // Set icon using raw data
            void setImage(uint8_t *, uint32_t);

            // Set strings
            void setTitle(const std::string &);
            void setPlaytime(const std::string &);
            void setLeftMuted(const std::string &);
            void setRightMuted(const std::string &);
            void setRank(const std::string &);

            // Set colours
            void setTitleColour(const Aether::Colour &);
            void setPlaytimeColour(const Aether::Colour &);
            void setMutedColour(const Aether::Colour &);
            void setLineColour(const Aether::Colour &);

            void update(unsigned int);

            // Adjusting width must move/adjust children
            void setW(int);
    };
};

#endif