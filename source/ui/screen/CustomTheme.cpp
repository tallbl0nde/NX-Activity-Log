#include "CustomTheme.hpp"
#include "Lang.hpp"

namespace Screen {
    CustomTheme::CustomTheme(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::Text * t = new Aether::Text(65, 44, "customTheme.heading"_lang, 28);
        t->setY(t->y() - t->h()/2);
        t->setColour(this->app->theme()->text());
        this->addElement(t);
        this->controls = new Aether::Controls();
        this->controls->addItem(new Aether::ControlItem(Aether::Button::A, "common.buttonHint.ok"_lang));
        this->controls->addItem(new Aether::ControlItem(Aether::Button::B, "common.buttonHint.back"_lang));
        this->controls->addItem(new Aether::ControlItem(Aether::Button::X, "customTheme.apply"_lang));
        this->addElement(this->controls);

        this->onButtonPress(Aether::Button::B, [this](){
            this->app->popScreen();
        });
        this->onButtonPress(Aether::Button::X, [this](){
            // Save beforehand
            this->app->popScreen();
        });
    }

    void CustomTheme::recolourElements() {
        this->controls->setColour(this->app->theme()->text());
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
        this->colourHighlight1->setColour(Aether::Colour{0, 0, 0, 200});
        this->colourHighlight2->setLineColour(this->app->theme()->mutedLine());
        this->colourHighlight2->setTextColour(this->app->theme()->text());
        this->colourHighlight2->setColour(Aether::Colour{0, 0, 0, 200});
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
    }

    void CustomTheme::onLoad() {
        // Create list
        this->list = new Aether::List(200, 88, 880, 559);
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // Image button
        this->optionImage = new Aether::ListOption("customTheme.image"_lang, (this->app->config()->tImage() ? "Yes" : "No"), [this](){
            bool b = !(this->app->config()->tImage());
            this->app->config()->setTImage(b);
            if (b) {
                this->optionImage->setValue("Yes");
                this->optionImage->setValueColour(this->app->theme()->accent());
            } else {
                this->optionImage->setValue("No");
                this->optionImage->setValueColour(this->app->theme()->mutedText());
            }
            this->app->setDisplayTheme();
        });
        this->list->addElement(this->optionImage);
        this->imageHint = new Aether::ListComment("customTheme.imageHint"_lang);
        this->list->addElement(this->imageHint);
        this->list->addElement(new Aether::ListSeparator());

        // Accent
        this->colourAccent = new CustomElm::ListColour("customTheme.accent"_lang, [this](){

        });
        this->list->addElement(this->colourAccent);
        this->list->addElement(new Aether::ListSeparator());

        // Background
        this->colourAltBG = new CustomElm::ListColour("customTheme.altBackground"_lang, [this](){

        });
        this->list->addElement(this->colourAltBG);
        this->colourBackground = new CustomElm::ListColour("customTheme.background"_lang, [this](){

        });
        this->list->addElement(this->colourBackground);
        this->list->addElement(new Aether::ListSeparator());

        // Highlight
        this->colourHighlight1 = new CustomElm::ListColour("customTheme.highlight1"_lang, [this](){

        });
        this->list->addElement(this->colourHighlight1);
        this->colourHighlight2 = new CustomElm::ListColour("customTheme.highlight2"_lang, [this](){

        });
        this->list->addElement(this->colourHighlight2);
        this->colourHighlighted = new CustomElm::ListColour("customTheme.highlightBG"_lang, [this](){

        });
        this->list->addElement(this->colourHighlighted);
        this->colourSelected = new CustomElm::ListColour("customTheme.selected"_lang, [this](){

        });
        this->list->addElement(this->colourSelected);
        this->list->addElement(new Aether::ListSeparator());

        // Lines
        this->colourMutedLine = new CustomElm::ListColour("customTheme.mutedLine"_lang, [this](){

        });
        this->list->addElement(this->colourMutedLine);
        this->colourLine = new CustomElm::ListColour("customTheme.line"_lang, [this](){

        });
        this->list->addElement(this->colourLine);
        this->list->addElement(new Aether::ListSeparator());

        // Text
        this->colourMutedText = new CustomElm::ListColour("customTheme.mutedText"_lang, [this](){

        });
        this->list->addElement(this->colourMutedText);
        this->colourText = new CustomElm::ListColour("customTheme.text"_lang, [this](){

        });
        this->list->addElement(this->colourText);

        // Set colours!
        this->addElement(this->list);
        this->recolourElements();

        // Show update icon if needbe
        this->updateElm = nullptr;
        if (this->app->hasUpdate()) {
            this->updateElm = new Aether::Image(50, 669, "romfs:/icon/download.png");
            this->updateElm->setColour(this->app->theme()->text());
            this->addElement(this->updateElm);
        }
    }

    void CustomTheme::onUnload() {
        this->removeElement(this->list);
        this->removeElement(this->updateElm);
    }
};