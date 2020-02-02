#ifndef THEME_HPP
#define THEME_HPP

#include "Aether.hpp"
#include "Types.hpp"

class Theme {
    private:
        // Theme variables (same name as Aether)
        Aether::Colour accent_;
        Aether::Colour altBG_;
        Aether::Colour bg_;
        Aether::Colour fg_;
        Aether::Colour highlightBG_;
        Aether::Colour mutedLine_;
        Aether::Colour mutedText_;
        Aether::Colour selected_;
        Aether::Colour text_;

        // Functions to set colours base on type
        void setThemeAuto();
        void setThemeCustom();
        void setThemeDark();
        void setThemeLight();

    public:
        // Constructor takes type and sets colours
        Theme(ThemeType);

        // Return private variables
        Aether::Colour accent();
        Aether::Colour altBG();
        Aether::Colour bg();
        Aether::Colour fg();
        Aether::Colour highlightBG();
        Aether::Colour mutedLine();
        Aether::Colour mutedText();
        Aether::Colour selected();
        Aether::Colour text();

        // Set theme to type
        void setTheme(ThemeType);
};

#endif