#include "Theme.hpp"
#include "Utils.hpp"

Theme::Theme(ThemeType t) {
    this->setTheme(t);
}

Aether::Colour Theme::accent() {
    return this->accent_;
}

Aether::Colour Theme::altBG() {
    return this->altBG_;
}

Aether::Colour Theme::bg() {
    return this->bg_;
}

Aether::Colour Theme::fg() {
    return this->fg_;
}

Aether::Colour Theme::highlightBG() {
    return this->highlightBG_;
}

Aether::Colour Theme::mutedLine() {
    return this->mutedLine_;
}

Aether::Colour Theme::mutedText() {
    return this->mutedText_;
}

Aether::Colour Theme::selected() {
    return this->selected_;
}

Aether::Colour Theme::text() {
    return this->text_;
}

void Theme::setTheme(ThemeType t) {
    switch (t) {
        case ThemeType::T_Auto:
            this->setThemeAuto();
            break;

        case ThemeType::T_Custom:
            this->setThemeCustom();
            break;

        case ThemeType::T_Dark:
            this->setThemeDark();
            break;

        case ThemeType::T_Light:
            this->setThemeLight();
            break;
    }
}

void Theme::setThemeAuto() {
    switch (Utils::getHorizonTheme()) {
        case ThemeType::T_Dark:
            this->setThemeDark();
            break;

        case ThemeType::T_Light:
            this->setThemeLight();
            break;
    }
}

void Theme::setThemeCustom() {

}

void Theme::setThemeDark() {
    this->accent_ = Aether::Theme::Dark.accent;
    this->altBG_ = Aether::Theme::Dark.altBG;
    this->bg_ = Aether::Theme::Dark.bg;
    this->fg_ = Aether::Theme::Dark.fg;
    this->highlightBG_ = Aether::Theme::Dark.highlightBG;
    this->mutedLine_ = Aether::Theme::Dark.mutedLine;
    this->mutedText_ = Aether::Theme::Dark.mutedText;
    this->selected_ = Aether::Theme::Dark.selected;
    this->text_ = Aether::Theme::Dark.text;
}

void Theme::setThemeLight() {
    this->accent_ = Aether::Theme::Light.accent;
    this->altBG_ = Aether::Theme::Light.altBG;
    this->bg_ = Aether::Theme::Light.bg;
    this->fg_ = Aether::Theme::Light.fg;
    this->highlightBG_ = Aether::Theme::Light.highlightBG;
    this->mutedLine_ = Aether::Theme::Light.mutedLine;
    this->mutedText_ = Aether::Theme::Light.mutedText;
    this->selected_ = Aether::Theme::Light.selected;
    this->text_ = Aether::Theme::Light.text;
}
