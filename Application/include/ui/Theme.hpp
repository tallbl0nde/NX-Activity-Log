#ifndef THEME_HPP
#define THEME_HPP

#include "Aether/Aether.hpp"
#include "Types.hpp"

class Theme {
    private:
        // Type of theme
        ThemeType type;

        // Theme colours
        ThemeSet theme;

        // Functions to set colours based on type
        void setThemeAuto();
        void setThemeCustom();
        void setThemeDark();
        void setThemeLight();

    public:
        // Constructor takes type and sets colours
        Theme(ThemeType);

        // Return colours
        Aether::Colour accent();
        Aether::Colour altBG();
        Aether::Colour bg();
        Aether::Colour fg();
        std::function<Aether::Colour(uint32_t)> highlightFunc();    // Actually constructs function when called
        Aether::Colour highlight1();
        Aether::Colour highlight2();
        Aether::Colour highlightBG();
        Aether::Colour mutedLine();
        Aether::Colour mutedText();
        Aether::Colour selected();
        Aether::Colour text();

        // Set private variables (only works if type is custom)
        void setAccent(Aether::Colour);
        void setAltBG(Aether::Colour);
        void setBg(Aether::Colour);
        void setFg(Aether::Colour);
        void setHighlight1(Aether::Colour);
        void setHighlight2(Aether::Colour);
        void setHighlightBG(Aether::Colour);
        void setMutedLine(Aether::Colour);
        void setMutedText(Aether::Colour);
        void setSelected(Aether::Colour);
        void setText(Aether::Colour);

        // Set theme to type
        void setTheme(ThemeType);

        // Ensure changes to custom colours are saved
        void saveCustom();

        // Destructor saves ini if theme type is custom
        ~Theme();
};

#endif