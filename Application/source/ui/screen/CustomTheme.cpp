#include "Application.hpp"
#include "ui/screen/CustomTheme.hpp"
#include "utils/Lang.hpp"
#include "ui/ThemePresets.hpp"

namespace Screen {
    CustomTheme::CustomTheme(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        this->topR = new Aether::Rectangle(30, 87, 1220, 1);
        this->addElement(this->topR);
        this->bottomR = new Aether::Rectangle(30, 647, 1220, 1);
        this->addElement(this->bottomR);
        this->heading = new Aether::Text(65, 44, "customTheme.heading"_lang, 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->addElement(this->heading);
        this->controls = new Aether::ControlBar();
        this->controls->addControl(Aether::Button::A, "common.buttonHint.ok"_lang);
        this->controls->addControl(Aether::Button::B, "common.buttonHint.back"_lang);
        this->controls->addControl(Aether::Button::X, "customTheme.apply"_lang);
        this->addElement(this->controls);

        this->onButtonPress(Aether::Button::B, [this](){
            // Restore backed up theme
            this->app->config()->setTImage(this->oldImage);
            this->app->theme()->setAccent(this->oldTheme.accent);
            this->app->theme()->setAltBG(this->oldTheme.altBG);
            this->app->theme()->setBg(this->oldTheme.bg);
            this->app->theme()->setFg(this->oldTheme.fg);
            this->app->theme()->setHighlightBG(this->oldTheme.highlightBG);
            this->app->theme()->setMutedLine(this->oldTheme.mutedLine);
            this->app->theme()->setMutedText(this->oldTheme.mutedText);
            this->app->theme()->setSelected(this->oldTheme.selected);
            this->app->theme()->setText(this->oldTheme.text);
            this->app->theme()->setHighlight1(this->oldHighlight1);
            this->app->theme()->setHighlight2(this->oldHighlight2);
            this->app->setDisplayTheme();
            this->app->popScreen();
        });
        this->onButtonPress(Aether::Button::X, [this](){
            // Set new theme and recreate all screens
            this->app->theme()->saveCustom();
            this->app->popScreen();
            this->app->reinitScreens(ScreenCreate::ThemeEdit);
        });
    }

    void CustomTheme::showErrorMsg() {
        delete this->msgbox;
        this->msgbox = new Aether::MessageBox();
        this->msgbox->setLineColour(this->app->theme()->mutedLine());
        this->msgbox->setRectangleColour(this->app->theme()->altBG());
        this->msgbox->addTopButton("common.buttonHint.ok"_lang, [this]() {
            this->msgbox->close();
        });
        this->msgbox->setTextColour(this->app->theme()->accent());
        int bw, bh;
        this->msgbox->getBodySize(&bw, &bh);
        Aether::Element * e = new Aether::Element(0, 0, 700, bh);
        Aether::TextBlock * t = new Aether::TextBlock(40, 30, "customTheme.imageError.heading"_lang, 24, e->w() - 80);
        t->setColour(this->app->theme()->text());
        e->addElement(t);
        t = new Aether::TextBlock(40, t->y() + t->h() + 20, "customTheme.imageError.body"_lang, 20, e->w() - 80);
        t->setColour(this->app->theme()->mutedText());
        e->addElement(t);
        e->setH(t->y() + t->h() + 30);
        this->msgbox->setBodySize(e->w(), e->h());
        this->msgbox->setBody(e);
        this->app->addOverlay(this->msgbox);
    }

    void CustomTheme::setupPicker(std::string title, Aether::Colour col, std::function<void(Aether::Colour)> func) {
        delete this->picker;
        this->picker = new CustomOvl::ColourPicker(title, col, func);
        this->picker->setBackLabel("common.buttonHint.back"_lang);
        this->picker->setOKLabel("common.buttonHint.ok"_lang);
        this->picker->setTipText("customTheme.picker.tip"_lang);
        this->picker->setRedHint("customTheme.picker.red"_lang);
        this->picker->setGreenHint("customTheme.picker.green"_lang);
        this->picker->setBlueHint("customTheme.picker.blue"_lang);
        this->picker->setAlphaHint("customTheme.picker.alpha"_lang);
        this->picker->setBackgroundColour(this->app->theme()->altBG());
        this->picker->setHighlightColour(this->app->theme()->accent());
        this->picker->setInactiveColour(this->app->theme()->mutedText());
        this->picker->setTextColour(this->app->theme()->text());
        this->app->addOverlay(this->picker);
    }

    void CustomTheme::createPresetList() {
        this->presetList = new Aether::PopupList("customTheme.preset"_lang);
        this->presetList->setBackLabel("common.buttonHint.back"_lang);
        this->presetList->setOKLabel("common.buttonHint.ok"_lang);
        // Basic Black
        this->presetList->addEntry("settings.appearance.theme.dark"_lang, [this]() {
            this->app->theme()->setAccent(ThemePreset::Dark.accent);
            this->app->theme()->setAltBG(ThemePreset::Dark.altBG);
            this->app->theme()->setBg(ThemePreset::Dark.bg);
            this->app->theme()->setFg(ThemePreset::Dark.fg);
            this->app->theme()->setHighlight1(ThemePreset::Dark.highlight1);
            this->app->theme()->setHighlight2(ThemePreset::Dark.highlight2);
            this->app->theme()->setHighlightBG(ThemePreset::Dark.highlightBG);
            this->app->theme()->setMutedLine(ThemePreset::Dark.mutedLine);
            this->app->theme()->setMutedText(ThemePreset::Dark.mutedText);
            this->app->theme()->setSelected(ThemePreset::Dark.selected);
            this->app->theme()->setText(ThemePreset::Dark.text);
            this->recolourElements();
        });
        // Basic White
        this->presetList->addEntry("settings.appearance.theme.light"_lang, [this]() {
            this->app->theme()->setAccent(ThemePreset::Light.accent);
            this->app->theme()->setAltBG(ThemePreset::Light.altBG);
            this->app->theme()->setBg(ThemePreset::Light.bg);
            this->app->theme()->setFg(ThemePreset::Light.fg);
            this->app->theme()->setHighlight1(ThemePreset::Light.highlight1);
            this->app->theme()->setHighlight2(ThemePreset::Light.highlight2);
            this->app->theme()->setHighlightBG(ThemePreset::Light.highlightBG);
            this->app->theme()->setMutedLine(ThemePreset::Light.mutedLine);
            this->app->theme()->setMutedText(ThemePreset::Light.mutedText);
            this->app->theme()->setSelected(ThemePreset::Light.selected);
            this->app->theme()->setText(ThemePreset::Light.text);
            this->recolourElements();
        });
    }

    void CustomTheme::recolourElements() {
        this->topR->setColour(this->app->theme()->fg());
        this->bottomR->setColour(this->app->theme()->fg());
        this->heading->setColour(this->app->theme()->text());
        this->controls->setDisabledColour(this->app->theme()->text());
        this->controls->setEnabledColour(this->app->theme()->text());
        this->optionPreset->setLineColour(this->app->theme()->mutedLine());
        this->optionPreset->setTextColour(this->app->theme()->text());
        this->optionImage->setHintColour(this->app->theme()->text());
        this->optionImage->setLineColour(this->app->theme()->mutedLine());
        this->optionImage->setValueColour((this->app->config()->tImage() ? this->app->theme()->accent() : this->app->theme()->mutedText()));
        this->imageHint->setTextColour(this->app->theme()->mutedText());
        this->colourAccent->setLineColour(this->app->theme()->mutedLine());
        this->colourAccent->setTextColour(this->app->theme()->text());
        this->colourAccent->setColour(this->app->theme()->accent());
        this->colourAltBG->setLineColour(this->app->theme()->mutedLine());
        this->colourAltBG->setTextColour(this->app->theme()->text());
        this->colourAltBG->setColour(this->app->theme()->altBG());
        this->colourBackground->setLineColour(this->app->theme()->mutedLine());
        this->colourBackground->setTextColour(this->app->theme()->text());
        this->colourBackground->setColour(this->app->theme()->bg());
        this->colourHighlight1->setLineColour(this->app->theme()->mutedLine());
        this->colourHighlight1->setTextColour(this->app->theme()->text());
        this->colourHighlight1->setColour(this->app->theme()->highlight1());
        this->colourHighlight2->setLineColour(this->app->theme()->mutedLine());
        this->colourHighlight2->setTextColour(this->app->theme()->text());
        this->colourHighlight2->setColour(this->app->theme()->highlight2());
        this->colourHighlighted->setLineColour(this->app->theme()->mutedLine());
        this->colourHighlighted->setTextColour(this->app->theme()->text());
        this->colourHighlighted->setColour(this->app->theme()->highlightBG());
        this->colourSelected->setLineColour(this->app->theme()->mutedLine());
        this->colourSelected->setTextColour(this->app->theme()->text());
        this->colourSelected->setColour(this->app->theme()->selected());
        this->colourLine->setLineColour(this->app->theme()->mutedLine());
        this->colourLine->setTextColour(this->app->theme()->text());
        this->colourLine->setColour(this->app->theme()->fg());
        this->colourMutedLine->setLineColour(this->app->theme()->mutedLine());
        this->colourMutedLine->setTextColour(this->app->theme()->text());
        this->colourMutedLine->setColour(this->app->theme()->mutedLine());
        this->colourText->setLineColour(this->app->theme()->mutedLine());
        this->colourText->setTextColour(this->app->theme()->text());
        this->colourText->setColour(this->app->theme()->text());
        this->colourMutedText->setLineColour(this->app->theme()->mutedLine());
        this->colourMutedText->setTextColour(this->app->theme()->text());
        this->colourMutedText->setColour(this->app->theme()->mutedText());
        this->app->setDisplayTheme();
        if (this->updateElm != nullptr) {
            this->updateElm->setColour(this->app->theme()->text());
        }
        this->presetList->setAllColours(this->app->theme()->altBG(), this->app->theme()->accent(), this->app->theme()->fg(), this->app->theme()->mutedLine(), this->app->theme()->text());
    }

    void CustomTheme::onLoad() {
        // Backup current theme (restored if not applied)
        this->oldImage = this->app->config()->tImage();
        this->oldTheme.accent = this->app->theme()->accent();
        this->oldTheme.altBG = this->app->theme()->altBG();
        this->oldTheme.bg = this->app->theme()->bg();
        this->oldTheme.fg = this->app->theme()->fg();
        this->oldTheme.highlightBG = this->app->theme()->highlightBG();
        this->oldTheme.mutedLine = this->app->theme()->mutedLine();
        this->oldTheme.mutedText = this->app->theme()->mutedText();
        this->oldTheme.selected = this->app->theme()->selected();
        this->oldTheme.text = this->app->theme()->text();
        this->oldHighlight1 = this->app->theme()->highlight1();
        this->oldHighlight2 = this->app->theme()->highlight2();

        // Create list
        this->list = new Aether::List(200, 88, 880, 559);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // Image button
        this->optionImage = new Aether::ListOption("customTheme.image"_lang, (this->app->config()->tImage() ? "common.yes"_lang : "common.no"_lang), [this](){
            bool b = !(this->app->config()->tImage());
            this->app->config()->setTImage(b);
            this->app->setDisplayTheme();

            // tImage is set false if an error occurred
            if (this->app->config()->tImage()) {
                this->optionImage->setValue("common.yes"_lang);
                this->optionImage->setValueColour(this->app->theme()->accent());
            } else {
                this->optionImage->setValue("common.no"_lang);
                this->optionImage->setValueColour(this->app->theme()->mutedText());
            }

            // Show message if error occurred
            if (b && !this->app->config()->tImage()) {
                this->showErrorMsg();
            }
        });
        this->list->addElement(this->optionImage);
        this->imageHint = new Aether::ListComment("customTheme.imageHint"_lang);
        this->list->addElement(this->imageHint);

        // Choose from preset
        this->optionPreset = new Aether::ListButton("customTheme.preset"_lang, [this]() {
            this->app->addOverlay(this->presetList);
        });
        this->list->addElement(this->optionPreset);

        this->list->addElement(new Aether::ListSeparator());

        // Accent
        this->colourAccent = new CustomElm::ListColour("customTheme.accent"_lang, [this](){
            this->setupPicker("customTheme.accent"_lang, this->app->theme()->accent(), [this](Aether::Colour c){
                this->colourAccent->setColour(c);
                this->app->theme()->setAccent(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourAccent);
        this->list->addElement(new Aether::ListSeparator());

        // Background
        this->colourAltBG = new CustomElm::ListColour("customTheme.altBackground"_lang, [this](){
            this->setupPicker("customTheme.altBackground"_lang, this->app->theme()->altBG(), [this](Aether::Colour c){
                this->colourAltBG->setColour(c);
                this->app->theme()->setAltBG(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourAltBG);
        this->colourBackground = new CustomElm::ListColour("customTheme.background"_lang, [this](){
            this->setupPicker("customTheme.background"_lang, this->app->theme()->bg(), [this](Aether::Colour c){
                this->colourBackground->setColour(c);
                this->app->theme()->setBg(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourBackground);
        this->list->addElement(new Aether::ListSeparator());

        // Highlight
        this->colourHighlight1 = new CustomElm::ListColour("customTheme.highlight1"_lang, [this](){
            this->setupPicker("customTheme.highlight1"_lang, this->app->theme()->highlight1(), [this](Aether::Colour c){
                this->colourHighlight1->setColour(c);
                this->app->theme()->setHighlight1(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourHighlight1);
        this->colourHighlight2 = new CustomElm::ListColour("customTheme.highlight2"_lang, [this](){
            this->setupPicker("customTheme.highlight2"_lang, this->app->theme()->highlight2(), [this](Aether::Colour c){
                this->colourHighlight2->setColour(c);
                this->app->theme()->setHighlight2(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourHighlight2);
        this->colourHighlighted = new CustomElm::ListColour("customTheme.highlightBG"_lang, [this](){
            this->setupPicker("customTheme.highlightBG"_lang, this->app->theme()->highlightBG(), [this](Aether::Colour c){
                this->colourHighlighted->setColour(c);
                this->app->theme()->setHighlightBG(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourHighlighted);
        this->colourSelected = new CustomElm::ListColour("customTheme.selected"_lang, [this](){
            this->setupPicker("customTheme.selected"_lang, this->app->theme()->selected(), [this](Aether::Colour c){
                this->colourSelected->setColour(c);
                this->app->theme()->setSelected(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourSelected);
        this->list->addElement(new Aether::ListSeparator());

        // Lines
        this->colourMutedLine = new CustomElm::ListColour("customTheme.mutedLine"_lang, [this](){
            this->setupPicker("customTheme.mutedLine"_lang, this->app->theme()->mutedLine(), [this](Aether::Colour c){
                this->colourMutedLine->setColour(c);
                this->app->theme()->setMutedLine(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourMutedLine);
        this->colourLine = new CustomElm::ListColour("customTheme.line"_lang, [this](){
            this->setupPicker("customTheme.line"_lang, this->app->theme()->fg(), [this](Aether::Colour c){
                this->colourLine->setColour(c);
                this->app->theme()->setFg(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourLine);
        this->list->addElement(new Aether::ListSeparator());

        // Text
        this->colourMutedText = new CustomElm::ListColour("customTheme.mutedText"_lang, [this](){
            this->setupPicker("customTheme.mutedText"_lang, this->app->theme()->mutedText(), [this](Aether::Colour c){
                this->colourMutedText->setColour(c);
                this->app->theme()->setMutedText(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourMutedText);
        this->colourText = new CustomElm::ListColour("customTheme.text"_lang, [this](){
            this->setupPicker("customTheme.text"_lang, this->app->theme()->text(), [this](Aether::Colour c){
                this->colourText->setColour(c);
                this->app->theme()->setText(c);
                this->recolourElements();
            });
        });
        this->list->addElement(this->colourText);

        this->addElement(this->list);

        // Show update icon if needbe
        this->updateElm = nullptr;
        if (this->app->hasUpdate()) {
            this->updateElm = new Aether::Image(50, 669, "romfs:/icon/download.png");
            this->addElement(this->updateElm);
        }

        this->createPresetList();
        this->picker = nullptr;

        // Set colours!
        this->recolourElements();

        this->msgbox = nullptr;
    }

    void CustomTheme::onUnload() {
        this->removeElement(this->list);
        this->removeElement(this->updateElm);
        delete this->msgbox;
        delete this->picker;
        delete this->presetList;
    }
};