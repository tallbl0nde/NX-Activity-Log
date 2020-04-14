#ifndef THEME_PRESETS_HPP
#define THEME_PRESETS_HPP

#include "Types.hpp"

namespace ThemePreset {
    // Colours for Basic Black
    const ThemeSet Dark {
        Aether::Theme::Dark.accent,
        Aether::Theme::Dark.altBG,
        Aether::Theme::Dark.bg,
        Aether::Theme::Dark.fg,
        Aether::Colour{0, 150, 190, 255},   // highlight1
        Aether::Colour{0, 250, 250, 255},   // highlight2
        Aether::Theme::Dark.highlightBG,
        Aether::Theme::Dark.mutedLine,
        Aether::Theme::Dark.mutedText,
        Aether::Theme::Dark.selected,
        Aether::Theme::Dark.text
    };

    // Colours for Basic White
    const ThemeSet Light {
        Aether::Theme::Light.accent,
        Aether::Theme::Light.altBG,
        Aether::Theme::Light.bg,
        Aether::Theme::Light.fg,
        Aether::Colour{0, 200, 190, 255},   // highlight1
        Aether::Colour{0, 250, 216, 255},   // highlight2
        Aether::Theme::Light.highlightBG,
        Aether::Theme::Light.mutedLine,
        Aether::Theme::Light.mutedText,
        Aether::Theme::Light.selected,
        Aether::Theme::Light.text
    };
};

#endif