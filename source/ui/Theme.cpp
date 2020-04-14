#include "NX.hpp"
#include "Theme.hpp"
#include "ThemeUtils.hpp"

Theme::Theme(ThemeType t) {
    Utils::Theme::readIni();
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

std::function<Aether::Colour(uint32_t)> Theme::highlightFunc() {
    return this->highlightFunc_;
}

Aether::Colour Theme::highlight1() {
    return this->highlight1_;
}

Aether::Colour Theme::highlight2() {
    return this->highlight2_;
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

void Theme::setAccent(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->accent_ = c;
}

void Theme::setAltBG(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->altBG_ = c;
}

void Theme::setBg(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->bg_ = c;
}

void Theme::setFg(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->fg_ = c;
}

void Theme::setHighlight1(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->highlight1_ = c;
    this->setHighlightFunc();
}

void Theme::setHighlight2(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->highlight2_ = c;
    this->setHighlightFunc();
}

void Theme::setHighlightBG(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->highlightBG_ = c;
}

void Theme::setMutedLine(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->mutedLine_ = c;
}

void Theme::setMutedText(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->mutedText_ = c;
}

void Theme::setSelected(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->selected_ = c;
}

void Theme::setText(Aether::Colour c) {
    if (this->type != Custom) {
        return;
    }

    this->text_ = c;
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
    Utils::Theme::readValues("accent", this->accent_);
    Utils::Theme::readValues("altBG", this->altBG_);
    Utils::Theme::readValues("bg", this->bg_);
    Utils::Theme::readValues("fg", this->fg_);
    Utils::Theme::readValues("highlight1", this->highlight1_);
    Utils::Theme::readValues("highlight2", this->highlight2_);
    Utils::Theme::readValues("highlightBG", this->highlightBG_);
    Utils::Theme::readValues("mutedLine", this->mutedLine_);
    Utils::Theme::readValues("mutedText", this->mutedText_);
    Utils::Theme::readValues("selected", this->selected_);
    Utils::Theme::readValues("text", this->text_);
    this->setHighlightFunc();

    this->type = Custom;
}

void Theme::setThemeDark() {
    this->accent_ = Aether::Theme::Dark.accent;
    this->altBG_ = Aether::Theme::Dark.altBG;
    this->bg_ = Aether::Theme::Dark.bg;
    this->fg_ = Aether::Theme::Dark.fg;
    this->highlight1_ = Aether::Colour{0, 150, 190, 255};
    this->highlight2_ = Aether::Colour{0, 250, 250, 255};
    this->highlightFunc_ = Aether::Theme::Dark.highlightFunc;
    this->highlightBG_ = Aether::Theme::Dark.highlightBG;
    this->mutedLine_ = Aether::Theme::Dark.mutedLine;
    this->mutedText_ = Aether::Theme::Dark.mutedText;
    this->selected_ = Aether::Theme::Dark.selected;
    this->text_ = Aether::Theme::Dark.text;
    this->type = Dark;
}

void Theme::setThemeLight() {
    this->accent_ = Aether::Theme::Light.accent;
    this->altBG_ = Aether::Theme::Light.altBG;
    this->bg_ = Aether::Theme::Light.bg;
    this->fg_ = Aether::Theme::Light.fg;
    this->highlight1_ = Aether::Colour{0, 200, 190, 255};
    this->highlight2_ = Aether::Colour{0, 250, 216, 255};
    this->highlightFunc_ = Aether::Theme::Light.highlightFunc;
    this->highlightBG_ = Aether::Theme::Light.highlightBG;
    this->mutedLine_ = Aether::Theme::Light.mutedLine;
    this->mutedText_ = Aether::Theme::Light.mutedText;
    this->selected_ = Aether::Theme::Light.selected;
    this->text_ = Aether::Theme::Light.text;
    this->type = Light;
}

void Theme::setHighlightFunc() {
    this->highlightFunc_ = [this](uint32_t t) {
        Aether::Colour col = {0, 0, 0, 0};
        col.r = this->highlight1_.r + ((this->highlight2_.r - this->highlight1_.r) * (0.5 * sin(1.8 * M_PI * (t/1000.0)) + 0.5));
        col.g = this->highlight1_.g + ((this->highlight2_.g - this->highlight1_.g) * (0.5 * sin(1.8 * M_PI * (t/1000.0)) + 0.5));
        col.b = this->highlight1_.b + ((this->highlight2_.b - this->highlight1_.b) * (0.5 * sin(1.8 * M_PI * (t/1000.0)) + 0.5));
        col.a = this->highlight1_.a + ((this->highlight2_.a - this->highlight1_.a) * (0.5 * sin(1.8 * M_PI * (t/1000.0)) + 0.5));
        return col;
    };
}

void Theme::saveCustom() {
    if (this->type == Custom) {
        // Write custom values
        Utils::Theme::writeValues("accent", this->accent_);
        Utils::Theme::writeValues("altBG", this->altBG_);
        Utils::Theme::writeValues("bg", this->bg_);
        Utils::Theme::writeValues("fg", this->fg_);
        Utils::Theme::writeValues("highlight1", this->highlight1_);
        Utils::Theme::writeValues("highlight2", this->highlight2_);
        Utils::Theme::writeValues("highlightBG", this->highlightBG_);
        Utils::Theme::writeValues("mutedLine", this->mutedLine_);
        Utils::Theme::writeValues("mutedText", this->mutedText_);
        Utils::Theme::writeValues("selected", this->selected_);
        Utils::Theme::writeValues("text", this->text_);
        Utils::Theme::deleteIni();
        Utils::Theme::readIni();
    }
}

Theme::~Theme() {
    this->saveCustom();
    Utils::Theme::deleteIni();
}