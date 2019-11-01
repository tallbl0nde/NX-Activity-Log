#include "activity.hpp"
#include "ui/list.hpp"
#include "ui/listitem.hpp"
#include "SDLHelper.hpp"
#include <switch.h>

namespace Screen {
    Activity::Activity(struct Theme * t, bool * b, User * u, std::vector<Title *> tls) : Screen::Screen(t, b) {
        this->list = new UI::List();
        for (int i = 0; i < tls.size(); i++) {
            this->list->addItem(new UI::ListItem(tls[i]));
        }

        this->user = u;
        this->controls->add(KEY_PLUS, "Exit", 0);
        this->controls->add(KEY_MINUS, "Sort", 1);
    }

    void Activity::event() {
        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    if (events.jbutton.which == 0) {
                        if (events.jbutton.button == Utils::key_map[KEY_PLUS]) {
                            *(this->loop) = false;
                        } else if (events.jbutton.button == Utils::key_map[KEY_DDOWN]) {
                            this->list->movePos(this->list->getPos() + 1);
                        } else if (events.jbutton.button == Utils::key_map[KEY_DUP]) {
                            this->list->movePos(this->list->getPos() - 1);
                        }
                    }
                    break;
            }
        }
    }

    void Activity::update(uint32_t dt) {

    }

    void Activity::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(this->theme->background, this->theme->background, this->theme->background, 255);
        SDLHelper::clearScreen();

        // Draw list of items
        this->list->draw(30, 100, 750, 550);

        // Draw over list to hide scrolling
        SDLHelper::setColour(this->theme->background, this->theme->background, this->theme->background, 255);
        SDLHelper::drawRect(30, 0, 780, 87);
        SDLHelper::drawRect(30, 648, 1220, 72);

        // Draw top and bottom lines
        SDLHelper::setColour(this->theme->line, this->theme->line, this->theme->line, 255);
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Draw player icon
        SDLHelper::setColour(255, 255, 255, 255);
        SDLHelper::drawTexture(this->user->getImage(), 65, 14, 60, 60);

        // Print heading
        std::string str = this->user->getUsername() + "'s Play Activity";
        SDLHelper::setColour(0, 0, 0, 255);
        SDLHelper::drawText(str.c_str(), 150, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);

        // Draw controls
        SDLHelper::setColour(255, 255, 255, 255);
        this->controls->draw(1215, 670);
    }

    Activity::~Activity() {

    }
}