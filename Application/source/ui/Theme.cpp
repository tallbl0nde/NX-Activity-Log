#include <numbers>
#include "utils/NX.hpp"
#include "ui/Theme.hpp"
#include "ui/ThemePresets.hpp"
#include "utils/ThemeUtils.hpp"

Theme::Theme(ThemeType t) {
    Utils::Theme::readIni();
    this->setTheme(t);
}

Aether::Colour Theme::accent() {
    return this->theme.accent;
}

Aether::Colour Theme::altBG() {
    return this->theme.altBG;
}

Aether::Colour Theme::bg() {
    return this->theme.bg;
}

Aether::Colour Theme::fg() {
    return this->theme.fg;
}

std::function<Aether::Colour(uint32_t)> Theme::highlightFunc() {
    // Copy colours in case object is destroyed
    Aether::Colour h1 = this->theme.highlight1;
    Aether::Colour h2 = this->theme.highlight2;
    return [h1, h2](uint32_t t) {
        Aether::Colour col;
        col.setR(h1.r() + ((h2.r() - h1.r()) * (0.5 * sin(1.8 * std::numbers::pi * (t/1000.0)) + 0.5)));
        col.setG(h1.g() + ((h2.g() - h1.g()) * (0.5 * sin(1.8 * std::numbers::pi * (t/1000.0)) + 0.5)));
        col.setB(h1.b() + ((h2.b() - h1.b()) * (0.5 * sin(1.8 * std::numbers::pi * (t/1000.0)) + 0.5)));
        col.setA(h1.a() + ((h2.a() - h1.a()) * (0.5 * sin(1.8 * std::numbers::pi * (t/1000.0)) + 0.5)));
        return col;
    };
}

Aether::Colour Theme::highlight1() {
    return this->theme.highlight1;
}

Aether::Colour Theme::highlight2() {
    return this->theme.highlight2;
}

Aether::Colour Theme::highlightBG() {
    return this->theme.highlightBG;
}

Aether::Colour Theme::mutedLine() {
    return this->theme.mutedLine;
}

Aether::Colour Theme::mutedText() {
    return this->theme.mutedText;
}

Aether::Colour Theme::selected() {
    return this->theme.selected;
}

Aether::Colour Theme::text() {
    return this->theme.text;
}

void Theme::setAccent(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.accent = c;
}

void Theme::setAltBG(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.altBG = c;
}

void Theme::setBg(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.bg = c;
}

void Theme::setFg(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.fg = c;
}

void Theme::setHighlight1(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.highlight1 = c;
}

void Theme::setHighlight2(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.highlight2 = c;
}

void Theme::setHighlightBG(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.highlightBG = c;
}

void Theme::setMutedLine(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.mutedLine = c;
}

void Theme::setMutedText(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.mutedText = c;
}

void Theme::setSelected(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.selected = c;
}

void Theme::setText(Aether::Colour c) {
    if (this->type != ThemeType::Custom) {
        return;
    }

    this->theme.text = c;
}


void Theme::setTheme(ThemeType t) {
    switch (t) {
        case ThemeType::Auto:
            this->setThemeAuto();
            break;

        case ThemeType::Custom:
            this->setThemeCustom();
            break;

        case ThemeType::Dark:
            this->setThemeDark();
            break;

        case ThemeType::Light:
            this->setThemeLight();
            break;

        // Never called but I don't like compiler warnings
        default:
            break;
    }
}

void Theme::setThemeAuto() {
    switch (Utils::NX::getHorizonTheme()) {
        case ThemeType::Dark:
            this->setThemeDark();
            break;

        case ThemeType::Light:
            this->setThemeLight();
            break;

        // Never called but I don't like compiler warnings
        default:
            break;
    }
}

void Theme::setThemeCustom() {
    // Set values to default in case values don't exist
    this->setThemeAuto();
    // Get values
    Utils::Theme::readValues("accent", this->theme.accent);
    Utils::Theme::readValues("altBG", this->theme.altBG);
    Utils::Theme::readValues("bg", this->theme.bg);
    Utils::Theme::readValues("fg", this->theme.fg);
    Utils::Theme::readValues("highlight1", this->theme.highlight1);
    Utils::Theme::readValues("highlight2", this->theme.highlight2);
    Utils::Theme::readValues("highlightBG", this->theme.highlightBG);
    Utils::Theme::readValues("mutedLine", this->theme.mutedLine);
    Utils::Theme::readValues("mutedText", this->theme.mutedText);
    Utils::Theme::readValues("selected", this->theme.selected);
    Utils::Theme::readValues("text", this->theme.text);
    this->type = ThemeType::Custom;
}

void Theme::setThemeDark() {
    this->theme = ThemePreset::Dark;
    this->type = ThemeType::Dark;
}

void Theme::setThemeLight() {
    this->theme = ThemePreset::Light;
    this->type = ThemeType::Light;
}

void Theme::saveCustom() {
    if (this->type == ThemeType::Custom) {
        // Write custom values
        Utils::Theme::writeValues("accent", this->theme.accent);
        Utils::Theme::writeValues("altBG", this->theme.altBG);
        Utils::Theme::writeValues("bg", this->theme.bg);
        Utils::Theme::writeValues("fg", this->theme.fg);
        Utils::Theme::writeValues("highlight1", this->theme.highlight1);
        Utils::Theme::writeValues("highlight2", this->theme.highlight2);
        Utils::Theme::writeValues("highlightBG", this->theme.highlightBG);
        Utils::Theme::writeValues("mutedLine", this->theme.mutedLine);
        Utils::Theme::writeValues("mutedText", this->theme.mutedText);
        Utils::Theme::writeValues("selected", this->theme.selected);
        Utils::Theme::writeValues("text", this->theme.text);
        Utils::Theme::deleteIni();
        Utils::Theme::readIni();
    }
}

Theme::~Theme() {
    this->saveCustom();
    Utils::Theme::deleteIni();
}