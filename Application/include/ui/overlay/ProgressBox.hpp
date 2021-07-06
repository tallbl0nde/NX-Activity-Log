#ifndef OVERLAY_PROGRESSBOX_HPP
#define OVERLAY_PROGRESSBOX_HPP

#include "Aether/Aether.hpp"

namespace CustomOvl {
    // Overlay which shows a brief message along with a progress bar
    class ProgressBox : public Aether::Overlay {
        private:
            // Background rectangle
            Aether::Rectangle * rect;

            // Message
            Aether::TextBlock * heading;

            // Progress bar
            Aether::RoundProgressBar * bar;

            // Progress bar value
            Aether::Text * value;

        public:
            // Constructs a new (blank) ProgressBox
            ProgressBox();

            void setHeading(const std::string &);
            void setValue(const double);

            void setBackgroundColour(const Aether::Colour);
            void setBarBackgroundColour(const Aether::Colour);
            void setBarForegroundColour(const Aether::Colour);
            void setTextColour(const Aether::Colour);
    };
};

#endif
