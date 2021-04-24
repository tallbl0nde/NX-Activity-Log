#include "Application.hpp"
#include "utils/Lang.hpp"
#include "ui/screen/Update.hpp"
#include "utils/UpdateUtils.hpp"
#include "utils/Utils.hpp"

namespace Screen {
    Update::Update(Main::Application * a) {
        this->app = a;

        // Create elements
        Aether::Rectangle * r;
        r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::Text * t = new Aether::Text(65, 44, "update.heading"_lang, 28);
        t->setY(t->y() - t->h()/2);
        t->setColour(this->app->theme()->text());
        this->addElement(t);
        this->controls = new Aether::ControlBar();
        this->controls->addControl(Aether::Button::A, "common.buttonHint.ok"_lang);
        this->controls->addControl(Aether::Button::B, "common.buttonHint.back"_lang);
        this->controls->setDisabledColour(this->app->theme()->text());
        this->controls->setEnabledColour(this->app->theme()->text());
        this->addElement(this->controls);

        // Exit only once thread is done
        this->onButtonPress(Aether::Button::B, [this](){
            if (this->threadDone) {
                this->app->popScreen();
            }
        });
    }

    void Update::createMsgbox() {
        delete this->msgbox;
        this->msgbox = new Aether::MessageBox();
        this->msgbox->setRectangleColour(this->app->theme()->altBG());
        this->msgbox->setLineColour(Aether::Colour{255, 255, 255, 0});
        // Prevent closing the overlay
        this->msgbox->onButtonPress(Aether::Button::B, nullptr);
    }

    void Update::progressCallback(long long int dl, long long int total) {
        // Only show progress if the file is downloading and not other comms! (size must be greater than 50 kB)
        if (total <= 50000) {
            return;
        }
        this->downloaded = 100 * ((double)dl/(double)total);
    }

    void Update::showDownloadProgress() {
        this->isDownloading = true;
        this->downloaded = 0.0;

        // Show progress bar in overlay
        this->createMsgbox();
        Aether::Element * body = new Aether::Element(0, 0, 560, 210);
        Aether::Text * heading = new Aether::Text(50, 35, "update.download.dl"_lang, 24);
        heading->setColour(this->app->theme()->text());
        body->addElement(heading);
        this->pbar = new Aether::RoundProgressBar(50, heading->y() + heading->h() + 25, 400, 12);
        this->pbar->setBackgroundColour(this->app->theme()->mutedLine());
        this->pbar->setForegroundColour(this->app->theme()->accent());
        body->addElement(this->pbar);
        this->ptext = new Aether::Text(pbar->x() + pbar->w() + 25, 0, "0%", 18);
        this->ptext->setColour(this->app->theme()->text());
        this->ptext->setY(this->pbar->y() + (this->pbar->h() - this->ptext->h())/2);
        body->addElement(this->ptext);
        body->setH(this->pbar->y() + this->pbar->h() + 35);
        this->msgbox->setBodySize(body->w(), body->h());
        this->msgbox->setBody(body);
        this->app->addOverlay(this->msgbox);

        // Start a new thread for download
        this->threadDone = false;
        this->updateThread = std::async(std::launch::async, Utils::Update::download, this->nroURL, [this](long long int dl, long long int total) {
            this->progressCallback(dl, total);
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
                if (!this->app->config()->tImage() || this->app->config()->gTheme() != ThemeType::Custom) {
                    Aether::Rectangle * r = new Aether::Rectangle(30, 88, 810, 559);
                    r->setColour(this->app->theme()->altBG());
                    this->el->addElement(r);
                }

                // Update button
                Aether::BorderButton * bb = new Aether::BorderButton(915, 270, 260, 80, 3, "update.update"_lang, 24, [this]() {
                    this->showDownloadProgress();
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

                this->nroURL = data.url;
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

    void Update::showError() {
        this->createMsgbox();
        int bw, bh;
        this->msgbox->getBodySize(&bw, &bh);
        Aether::Element * e = new Aether::Element(0, 0, 700, bh);
        Aether::TextBlock * t = new Aether::TextBlock(50, 40, "update.download.error"_lang, 24, e->w() - 100);
        t->setColour(this->app->theme()->text());
        e->addElement(t);
        t = new Aether::TextBlock(50, t->y() + t->h() + 20, "update.error.tip"_lang, 20, e->w() - 100);
        t->setColour(this->app->theme()->mutedText());
        e->addElement(t);
        e->setH(t->y() + t->h() + 40);
        this->msgbox->setBodySize(e->w(), e->h());
        this->msgbox->setBody(e);
    }

    void Update::showSuccess() {
        this->createMsgbox();
        int bw, bh;
        this->msgbox->getBodySize(&bw, &bh);
        Aether::Element * e = new Aether::Element(0, 0, 700, bh);
        Aether::TextBlock * t = new Aether::TextBlock(50, 40, "update.download.success"_lang, 24, e->w() - 100);
        t->setColour(this->app->theme()->text());
        e->addElement(t);
        if (this->app->isUserPage()) {
            t = new Aether::TextBlock(50, t->y() + t->h() + 20, "update.download.successHintPage"_lang, 20, e->w() - 100);
        } else {
            t = new Aether::TextBlock(50, t->y() + t->h() + 20, "update.download.successHint"_lang, 20, e->w() - 100);
        }
        t->setColour(this->app->theme()->mutedText());
        e->addElement(t);
        e->setH(t->y() + t->h() + 40);
        this->msgbox->setBodySize(e->w(), e->h());
        this->msgbox->setBody(e);
    }

    void Update::update(uint32_t dt) {
        if (!this->threadDone) {
            if (this->isDownloading) {
                // Download thread
                if (this->updateThread.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    this->msgbox->close();
                    this->threadDone = true;
                } else {
                    // Update progress
                    this->pbar->setValue(this->downloaded);
                    this->ptext->setString(Utils::truncateToDecimalPlace(std::to_string(Utils::roundToDecimalPlace(this->downloaded, 0)), 0) + "%");
                }
            } else {
                // Check thread
                if (this->data.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    this->threadDone = true;
                    this->msgbox->close();
                    this->showElements();
                }
            }
        } else {
            if (this->isDownloading && this->msgbox->shouldClose()) {
                bool b = this->updateThread.get();
                if (b) {
                    // No errors - show success message
                    this->showSuccess();
                } else {
                    // Error occurred - show error message
                    this->showError();
                }
                // Enable close button
                int bw, bh;
                this->msgbox->getBodySize(&bw, &bh);
                this->msgbox->addTopButton("common.buttonHint.ok"_lang, [this, b]() {
                    if (b) {
                        this->app->exit();
                    } else {
                        this->msgbox->close();
                    }
                    this->isDownloading = false;
                });
                this->msgbox->setBodySize(bw, bh);
                this->msgbox->setLineColour(this->app->theme()->mutedLine());
                this->msgbox->setTextColour(this->app->theme()->accent());
                this->app->addOverlay(this->msgbox);
            }
        }
    }

    void Update::onLoad() {
        this->el = new Aether::Container(0, 87, 1280, 558);

        // "Checking updates" overlay
        this->msgbox = nullptr;
        this->createMsgbox();
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

        // Start update thread
        this->isDownloading = false;
        this->threadDone = false;
        this->data = std::async(std::launch::async, Utils::Update::check);
    }

    void Update::onUnload() {
        this->removeElement(this->el);
        delete this->msgbox;
    }
};