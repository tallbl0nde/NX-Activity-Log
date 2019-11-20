#ifndef THEME_HPP
#define THEME_HPP

#include <cstdint>
#include <SDL2/SDL.h>

enum ThemeType {
    T_Light,
    T_Dark,
    T_Custom
};

namespace UI {
    // The theme class literally represents a theme.
    // It is designed to only have one instance.
    class Theme {
        private:
            // Type of current theme
            ThemeType type;

            // Used to compute highlight colour
            float hi_sin_time;

            // Colour variables
            SDL_Color accent;
            SDL_Color alt_background;
            SDL_Color background;
            SDL_Color foreground;
            SDL_Color highlight;
            SDL_Color highlight_bg;
            SDL_Color muted_line;
            SDL_Color muted_text;
            SDL_Color text;
            SDL_Color touch_overlay;

        public:
            // Constructor initializes to light scheme
            Theme();

            // Set to preconfigured colours
            void setLight();
            void setDark();

            // Return type of theme
            ThemeType getType();

            // Adjusts values of highlight colour
            // Takes a dt value
            void animateHighlight(uint32_t);

            ~Theme();

            // Getters for all theme colours (prohibits writing)
            SDL_Color getAccent();
            SDL_Color getAltBG();
            SDL_Color getBG();
            SDL_Color getFG();
            SDL_Color getHighlight();
            SDL_Color getHighlightBG();
            SDL_Color getMutedLine();
            SDL_Color getMutedText();
            SDL_Color getText();
            SDL_Color getTouchOverlay();
    };
};

#endif