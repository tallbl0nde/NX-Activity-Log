#ifndef SCREEN_CUSTOMTHEME_HPP
#define SCREEN_CUSTOMTHEME_HPP

#include "ui/overlay/ColourPicker.hpp"
#include "ui/element/ListColour.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // "Update" page
    class CustomTheme : public Aether::Screen {
        private:
            // Pointer to main app object for user + titles
            Main::Application * app;
            Aether::Image * updateElm;

            // Need pointers to every element in order to update colours
            Aether::Text * heading;
            Aether::Rectangle * topR;
            Aether::Rectangle * bottomR;
            Aether::ControlBar * controls;
            Aether::List * list;

            Aether::ListButton * optionPreset;

            Aether::ListOption * optionImage;
            Aether::ListComment * imageHint;
            CustomElm::ListColour * colourAccent;

            CustomElm::ListColour * colourAltBG;
            CustomElm::ListColour * colourBackground;

            CustomElm::ListColour * colourHighlight1;
            CustomElm::ListColour * colourHighlight2;
            CustomElm::ListColour * colourHighlighted;
            CustomElm::ListColour * colourSelected;

            CustomElm::ListColour * colourLine;
            CustomElm::ListColour * colourMutedLine;

            CustomElm::ListColour * colourText;
            CustomElm::ListColour * colourMutedText;

            // Copy theme on load (highlight1/2 aren't in Theme_T)
            bool oldImage;
            Aether::Theme_T oldTheme;
            Aether::Colour oldHighlight1;
            Aether::Colour oldHighlight2;

            // Error popup
            Aether::MessageBox * msgbox;
            // Create popup
            void showErrorMsg();

            // Colour picker
            CustomOvl::ColourPicker * picker;
            // Setup picker (title, colour, callback)
            void setupPicker(std::string, Aether::Colour, std::function<void(Aether::Colour)>);

            // Popup of presets
            Aether::PopupList * presetList;
            // Fill list with presets and callbacks
            void createPresetList();

            // Recolour everything
            void recolourElements();

        public:
            // Passed main application object
            CustomTheme(Main::Application *);

            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();
    };
};


#endif