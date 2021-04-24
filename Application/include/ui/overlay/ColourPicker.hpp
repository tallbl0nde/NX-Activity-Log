#ifndef OVERLAY_COLOURPICKER_HPP
#define OVERLAY_COLOURPICKER_HPP

#include "Aether/Aether.hpp"

namespace CustomOvl {
    // Select a colour using a spinner for r, g, b, a. Shows preview, hex value
    // and requires a callback for when OK is pressed
    class ColourPicker : public Aether::Overlay {
        private:
            // Pointers to elements
            Aether::ControlBar * ctrlBar;
            Aether::Rectangle * rect, * top, * bottom;
            Aether::Text * title;
            Aether::BorderButton * button;
            Aether::Text * tip;

            // Keep pointers to each item
            Aether::Spinner * red;
            Aether::Spinner * green;
            Aether::Spinner * blue;
            Aether::Spinner * alpha;

            // Preview elements
            Aether::Rectangle * colourRect;
            Aether::Text * colourHex;

            // Callback func
            std::function<void(Aether::Colour)> func;
            // Wrapper for callback that passes colour
            void callFunc();

        public:
            // Pass title, colour and callback which is given colour (not called if closed by cancelling!)
            ColourPicker(std::string, Aether::Colour, std::function<void(Aether::Colour)>);

            // Update colour every frame
            void update(uint32_t);

            // Set button labels
            void setBackLabel(std::string);
            void setOKLabel(std::string);
            void setTipText(std::string);

            // Set text for each spinner's hint
            void setRedHint(std::string);
            void setGreenHint(std::string);
            void setBlueHint(std::string);
            void setAlphaHint(std::string);

            // Set colours
            void setBackgroundColour(Aether::Colour);
            void setHighlightColour(Aether::Colour);
            void setInactiveColour(Aether::Colour);
            void setTextColour(Aether::Colour);
    };
};

#endif