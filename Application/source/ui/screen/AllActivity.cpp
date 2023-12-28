#include "ui/screen/AllActivity.hpp"
#include "Application.hpp"
#include "utils/Lang.hpp"
#include "utils/Utils.hpp"
#include "utils/Time.hpp"

namespace Screen {
    AllActivity::AllActivity(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r;
        if (!this->app->config()->tImage() || this->app->config()->gTheme() != ThemeType::Custom) {
            r = new Aether::Rectangle(400, 88, 850, 559);
            r->setColour(this->app->theme()->altBG());
            this->addElement(r);
        }
        r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(this->app->theme()->fg());
        this->addElement(r);
        Aether::ControlBar * c = new Aether::ControlBar();
        c->addControl(Aether::Button::A, "common.buttonHint.ok"_lang);
        c->addControl(Aether::Button::B, "common.buttonHint.back"_lang);
        c->addControl(Aether::Button::X, "common.buttonHint.sort"_lang);
        c->setDisabledColour(this->app->theme()->text());
        c->setEnabledColour(this->app->theme()->text());
        this->addElement(c);

        // Create sort overlay
        this->sortOverlay = new Aether::PopupList("common.sort.heading"_lang);
        this->sortOverlay->setBackLabel("common.buttonHint.back"_lang);
        this->sortOverlay->setOKLabel("common.buttonHint.ok"_lang);
        this->sortOverlay->setBackgroundColour(this->app->theme()->altBG());
        this->sortOverlay->setHighlightColour(this->app->theme()->accent());
        this->sortOverlay->setLineColour(this->app->theme()->fg());
        this->sortOverlay->setListLineColour(this->app->theme()->mutedLine());
        this->sortOverlay->setTextColour(this->app->theme()->text());

        // Add callbacks for buttons
        this->onButtonPress(Aether::Button::X, [this](){
            this->setupOverlay();
        });
        this->onButtonPress(Aether::Button::B, [this](){
            if (this->app->isUserPage()) {
                this->app->exit();
            } else {
                this->app->setScreen(ScreenID::UserSelect);
            }
        });
        this->onButtonPress(Aether::Button::ZR, [this](){
            this->app->setHoldDelay(30);
        });
        this->onButtonRelease(Aether::Button::ZR, [this](){
            this->app->setHoldDelay(100);
        });
        this->onButtonPress(Aether::Button::ZL, [this](){
            this->app->setHoldDelay(30);
        });
        this->onButtonRelease(Aether::Button::ZL, [this](){
            this->app->setHoldDelay(100);
        });
    }

    void AllActivity::setupOverlay() {
        // Empty previous entries
        this->sortOverlay->removeEntries();

        // Add entries and highlight current sort
        SortType sort = this->list->sort();
        for (int i = 0; i < SortType::TotalSorts; i++) {
            SortType s = (SortType)i;
            this->sortOverlay->addEntry(toString(s), [this, s](){
                this->list->setSort(s);
            }, sort == s);
        }

        // List is focussed when overlay is closed
        this->setFocussed(this->list);

        this->app->addOverlay(this->sortOverlay);
    }

    void AllActivity::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, Utils::formatHeading(this->app->activeUser()->username()), 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(this->app->theme()->text());
        this->addElement(this->heading);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize(), Aether::Render::Wait);
        this->image->setScaleDimensions(60, 60);
        this->image->renderSync();
        this->addElement(this->image);

        // Create side menu
        this->menu = new Aether::Menu(30, 88, 388, 559);
        this->menu->addElement(new Aether::MenuOption("common.screen.recentActivity"_lang, this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(ScreenID::RecentActivity);
        }));
        Aether::MenuOption * opt = new Aether::MenuOption("common.screen.allActivity"_lang, this->app->theme()->accent(), this->app->theme()->text(), nullptr);
        this->menu->addElement(opt);
        this->menu->setActiveOption(opt);
        this->menu->addElement(new Aether::MenuSeparator(this->app->theme()->mutedLine()));
        this->menu->addElement(new Aether::MenuOption("common.screen.settings"_lang, this->app->theme()->accent(), this->app->theme()->text(), [this](){
            this->app->setScreen(ScreenID::Settings);
        }));
        this->menu->setFocussed(opt);
        this->addElement(this->menu);

        // Create list
        this->list = new CustomElm::SortedList(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setHeadingColour(this->app->theme()->mutedText());
        this->list->setScrollBarColour(this->app->theme()->mutedLine());

        // Populate list + count total time
        std::vector<AdjustmentValue> adjustments = this->app->config()->adjustmentValues();
        std::vector<NX::Title *> t = this->app->titleVector();
        std::vector<uint64_t> hidden = this->app->config()->hiddenTitles();
        uint64_t totalSecs = 0;
        for (size_t i = 0; i < t.size(); i++) {
            // Skip over hidden games
            if (std::find(hidden.begin(), hidden.end(), t[i]->titleID()) != hidden.end()) {
                continue;
            }

            // Get statistics and append adjustment if needed
            NX::PlayStatistics * ps = this->app->playdata()->getStatisticsForUser(t[i]->titleID(), this->app->activeUser()->ID());
            std::vector<AdjustmentValue>::iterator it = std::find_if(adjustments.begin(), adjustments.end(), [this, t, i](AdjustmentValue val) {
                return (val.titleID == t[i]->titleID() && val.userID == this->app->activeUser()->ID());
            });
            if (it != adjustments.end()) {
                ps->playtime += (*it).value;
            }

            // Skip unplayed titles
            totalSecs += ps->playtime;
            if (ps->launches == 0) {
                // Add in dummy data if not launched before (due to adjustment)
                ps->firstPlayed = Utils::Time::posixTimestampToPdm(Utils::Time::getTimeT(Utils::Time::getTmForCurrentTime()));
                ps->lastPlayed = ps->firstPlayed;
                ps->launches = 1;

                if (ps->playtime == 0) {
                    delete ps;
                    continue;
                }
            }

            // "Convert" PlayStatistics to SortInfo
            SortInfo * si = new SortInfo;
            si->name = t[i]->name();
            si->titleID = t[i]->titleID();
            si->firstPlayed = ps->firstPlayed;
            si->lastPlayed = ps->lastPlayed;
            si->playtime = ps->playtime;
            si->launches = ps->launches;

            // Create ListActivity and add to list
            CustomElm::ListActivity * la = new CustomElm::ListActivity();
            la->setImage(t[i]->imgPtr(), t[i]->imgSize());
            la->setTitle(t[i]->name());
            la->setPlaytime(Utils::playtimeToPlayedForString(ps->playtime));
            la->setLeftMuted(Utils::lastPlayedToString(ps->lastPlayed));
            la->setRightMuted(Utils::launchesToPlayedString(ps->launches));
            la->onPress([this, i](){
                this->app->setActiveTitle(i);
                this->app->pushScreen();
                this->app->setScreen(ScreenID::Details);
            });
            la->setTitleColour(this->app->theme()->text());
            la->setPlaytimeColour(this->app->theme()->accent());
            la->setMutedColour(this->app->theme()->mutedText());
            la->setLineColour(this->app->theme()->mutedLine());
            this->list->addElement(la, si);
        }

        // Sort the list
        this->list->setSort(this->app->config()->lSort());
        this->addElement(this->list);

        // Render total hours string
        this->hours = new Aether::Text(1215, 44, Utils::playtimeToTotalPlaytimeString(totalSecs), 20);
        this->hours->setXY(this->hours->x() - this->hours->w(), this->hours->y() - this->hours->h()/2);
        this->hours->setColour(this->app->theme()->mutedText());
        this->addElement(this->hours);
        // Show update icon if needbe
        this->updateElm =nullptr;
        if (this->app->hasUpdate()) {
            this->updateElm = new Aether::Image(50, 669, "romfs:/icon/download.png");
            this->updateElm->setColour(this->app->theme()->text());
            this->addElement(this->updateElm);
        }
    }

    void AllActivity::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->hours);
        this->removeElement(this->image);
        this->removeElement(this->list);
        this->removeElement(this->menu);
        this->removeElement(this->updateElm);
    }

    AllActivity::~AllActivity() {
        delete this->sortOverlay;
    }
};
