#include "Curl.hpp"
#include "Lang.hpp"
#include "Update.hpp"
#include "Utils.hpp"

namespace Screen {
    Update::Update(Main::Application * a) {
        this->app = a;

        // Create elements
        Aether::Rectangle * r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::Text * t = new Aether::Text(65, 44, "update.heading"_lang, 28);
        t->setY(t->y() - t->h()/2);
        t->setColour(this->app->theme()->text());
        this->addElement(t);
        this->controls = new Aether::Controls();
        this->controls->addItem(new Aether::ControlItem(Aether::Button::A, "common.buttonHint.ok"_lang));
        this->controls->addItem(new Aether::ControlItem(Aether::Button::B, "common.buttonHint.back"_lang));
        this->addElement(this->controls);

        // Exit only once thread is done
        this->onButtonPress(Aether::Button::B, [this](){
            if (this->threadDone) {
                this->app->popScreen();
            }
        });
    }

    void Update::showElements() {
        // This won't block as this is only called after the thread is finished
        UpdateData data = this->data.get();

        if (data.success) {
            if (data.version == ("v" VER_STRING)) {
                // Show up to date message and version
                Aether::Text * t = new Aether::Text(640, 250, "update.upToDate"_lang, 24);
                t->setColour(this->app->theme()->text());
                t->setX(t->x() - t->w()/2);
                this->el->addElement(t);
                t = new Aether::Text(640, t->y() + t->h() + 10, Utils::insertVersionInString("update.version"_lang, VER_STRING), 18);
                t->setColour(this->app->theme()->mutedText());
                t->setX(t->x() - t->w()/2);
                this->el->addElement(t);
            } else {
                // Background behind buttons
                Aether::Rectangle * r = new Aether::Rectangle(30, 88, 810, 559);
                r->setColour(this->app->theme()->altBG());
                this->el->addElement(r);

                // Update button
                Aether::BorderButton * bb = new Aether::BorderButton(915, 270, 260, 80, 3, "update.update"_lang, 24, [this]() {
                    // Update stuff here!
                });
                bb->setBorderColour(this->app->theme()->text());
                bb->setTextColour(this->app->theme()->text());
                this->el->addElement(bb);

                // Cancel button
                bb = new Aether::BorderButton(945, 380, 200, 60, 3, "update.cancel"_lang, 20, [this]() {
                    this->app->popScreen();
                });
                bb->setBorderColour(this->app->theme()->text());
                bb->setTextColour(this->app->theme()->text());
                this->el->addElement(bb);

                // List contains heading + changelog
                Aether::List * l = new Aether::List(30, 88, 800, 559);
                l->setShowScrollBar(true);
                l->setScrollBarColour(this->app->theme()->mutedLine());
                Aether::Text * t = new Aether::Text(0, 0, "", 24);
                t->setColour(this->app->theme()->text());
                l->addElement(t);
                t->setString(Utils::insertVersionInString("update.newAvailable"_lang, data.version.substr(1, data.version.length() - 1)));
                l->addElement(new Aether::ListSeparator(30));
                Aether::TextBlock * tb = new Aether::TextBlock(0, 0, "", 18, 10);
                tb->setColour(this->app->theme()->mutedText());
                l->addElement(tb);
                tb->setWrapWidth(tb->w());
                tb->setString(data.changelog);
                Aether::Element * e = new Aether::Element(0, 0, 0, 10);
                l->addElement(e);
                this->el->addElement(l);
            }
        } else {
            // Show error message
            Aether::Text * t = new Aether::Text(640, 250, "update.error.heading"_lang, 24);
            t->setColour(this->app->theme()->text());
            t->setX(t->x() - t->w()/2);
            this->el->addElement(t);
            t = new Aether::Text(640, 295, "update.error.tip"_lang, 20);
            t->setColour(this->app->theme()->mutedText());
            t->setX(t->x() - t->w()/2);
            this->el->addElement(t);
            t = new Aether::Text(640, t->y() + t->h() + 10, Utils::insertVersionInString("update.version"_lang, VER_STRING), 18);
            t->setColour(this->app->theme()->mutedText());
            t->setX(t->x() - t->w()/2);
            this->el->addElement(t);
        }
        this->addElement(this->el);
    }

    void Update::update(uint32_t dt) {
        if (!this->threadDone) {
            if (this->data.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                this->threadDone = true;
                this->msgbox->close(true);
                this->controls->setColour(this->app->theme()->text());
                this->showElements();
            }
        }
    }

    void Update::onLoad() {
        this->controls->setColour(this->app->theme()->mutedText());
        this->el = new Aether::Container(0, 87, 1280, 558);

        // "Checking updates" overlay
        this->msgbox = new Aether::MessageBox();
        this->msgbox->setRectangleColour(this->app->theme()->altBG());
        this->msgbox->setLineColour(Aether::Colour{255, 255, 255, 0});
        int bw, bh;
        msgbox->getBodySize(&bw, &bh);
        Aether::Element * body = new Aether::Element(0, 0, bw, bh);
        Aether::Text * t = new Aether::Text(50, 40, "update.waiting"_lang, 24);
        if (t->w() > bw) {
            bw = t->w() + 100;
        }
        t->setX(0 + (bw - t->w())/2);
        t->setColour(this->app->theme()->text());
        body->addElement(t);
        this->msgbox->setBodySize(bw, t->y() + t->h() + 40);
        this->msgbox->setBody(body);
        this->app->addOverlay(this->msgbox);
        // Prevent closing the overlay
        this->msgbox->onButtonPress(Aether::Button::B, nullptr);

        // Start update thread
        this->threadDone = false;
        this->data = std::async(std::launch::async, Utils::Curl::getLatestMetadata);
    }

    void Update::onUnload() {
        this->removeElement(this->el);
        delete this->msgbox;
    }
};