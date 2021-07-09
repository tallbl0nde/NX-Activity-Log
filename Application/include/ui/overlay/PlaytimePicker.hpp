#ifndef OVERLAY_PLAYTIMEPICKER_HPP
#define OVERLAY_PLAYTIMEPICKER_HPP

#include "Aether/Aether.hpp"

namespace CustomOvl {
    // Picks a playtime adjustment value using hour, minute and seconds spinners.
    class PlaytimePicker : public Aether::Overlay {
        private:
            // Pointers to elements
            Aether::ControlBar * ctrlBar;
            Aether::Rectangle * rect, * top, * bottom;
            Aether::Text * title;
            Aether::BorderButton * button;
            Aether::Text * tip;

            // Keep pointers to each item
            Aether::Spinner * hour;
            Aether::Spinner * minute;
            Aether::Spinner * second;

            // Callback function
            std::function<void(int)> func;

            // Helper to invoke callback
            void callFunc();

        public:
            // Pass title and callback which is invoked when confirmed
            PlaytimePicker(const std::string &, const int, std::function<void(int)>);

            // Set button labels
            void setBackLabel(const std::string &);
            void setOKLabel(const std::string &);
            void setTipText(const std::string &);

            // Set text for each spinner's hint
            void setHourHint(const std::string &);
            void setMinuteHint(const std::string &);
            void setSecondHint(const std::string &);

            // Set colours
            void setBackgroundColour(const Aether::Colour &);
            void setHighlightColour(const Aether::Colour &);
            void setInactiveColour(const Aether::Colour &);
            void setTextColour(const Aether::Colour &);
    };
};

#endif
