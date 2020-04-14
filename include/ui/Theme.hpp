#ifndef THEME_HPP
#define THEME_HPP

#include "Aether.hpp"
#include "Types.hpp"

class Theme {
    private:
        // Type of theme
        ThemeType type;

        // Theme variables (same name as Aether)
        Aether::Colour accent_;
        Aether::Colour altBG_;
        Aether::Colour bg_;
        Aether::Colour fg_;
        std::function<Aether::Colour(uint32_t)> highlightFunc_;
        Aether::Colour highlight1_;
        Aether::Colour highlight2_;
        Aether::Colour highlightBG_;
        Aether::Colour mutedLine_;
        Aether::Colour mutedText_;
        Aether::Colour selected_;
        Aether::Colour text_;

        // Functions to set colours based on type
        void setThemeAuto();
        void setThemeCustom();
        void setThemeDark();
        void setThemeLight();

        void setHighlightFunc();

    public:
        // Constructor takes type and sets colours
        Theme(ThemeType);

        // Return private variables
        Aether::Colour accent();
        Aether::Colour altBG();
        Aether::Colour bg();
        Aether::Colour fg();
        std::function<Aether::Colour(uint32_t)> highlightFunc();
        Aether::Colour highlight1();
        Aether::Colour highlight2();
        Aether::Colour highlightBG();
        Aether::Colour mutedLine();
        Aether::Colour mutedText();
        Aether::Colour selected();
        Aether::Colour text();

        // Set private variables (only works if custom)
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