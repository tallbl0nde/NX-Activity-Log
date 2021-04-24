#ifndef OVERLAY_PLAYSESSION_HPP
#define OVERLAY_PLAYSESSION_HPP

#include "Aether/Aether.hpp"

namespace CustomOvl {
    // Overlay to show a breakdown of a PlaySession
    class PlaySession : public Aether::Overlay {
        private:
            // Static elements
            Aether::Text * title;
            Aether::Rectangle * rect;
            Aether::Rectangle * top;
            Aether::Rectangle * bottom;
            Aether::Rectangle * sep;
            Aether::ControlBar * ctrlBar;
            Aether::List * list;

            // Summary at bottom
            Aether::Text * length;
            Aether::Text * lengthSub;
            Aether::Text * playtime;
            Aether::Text * playtimeSub;

            // Scroll bar colour
            Aether::Colour col;

        public:
            PlaySession();

            // Add element to list
            void addListItem(Aether::Element *);

            // Set bottom strings
            void setLength(std::string);
            void setPlaytime(std::string);

            // Set colours
            void setAccentColour(Aether::Colour);
            void setBackgroundColour(Aether::Colour);
            void setLineColour(Aether::Colour);
            void setMutedLineColour(Aether::Colour);
            void setTextColour(Aether::Colour);
    };
};

#endif