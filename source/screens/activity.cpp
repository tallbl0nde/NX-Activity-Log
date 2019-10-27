#include "activity.hpp"
#include <SDL2/SDL_ttf.h>
#include <switch.h>

namespace Screen {
    Activity::Activity(SDL_Renderer * r, struct Theme * t, bool * b) : Screen::Screen(r, t, b) {
        this->controls->add(KEY_A, "Confirm Something", 0);
        this->controls->add(KEY_B, "Back", 1);
        this->controls->add(KEY_PLUS, "Exit", 2);
    }

    void Activity::event() {
        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    if (events.jbutton.which == 0) {
                        if (events.jbutton.button == 10) {
                            *(this->loop) = false;
                        } else if (events.jbutton.button == 11){

                        }
                    }
                    break;
            }
        }
    }

    void Activity::update(uint32_t dt) {

    }

    void Activity::draw() {
        // Clear screen
        SDL_SetRenderDrawColor(this->renderer, this->theme->background, this->theme->background, this->theme->background, 255);
        SDL_RenderClear(this->renderer);

        // Draw top and bottom lines
        SDL_Rect lineTop = {30, 87, 1220, 1};
        SDL_Rect lineBottom = {30, 647, 1220, 1};

        SDL_SetRenderDrawColor(this->renderer, this->theme->line, this->theme->line, this->theme->line, 255);
        SDL_RenderFillRect(this->renderer, &lineTop);
        SDL_RenderFillRect(this->renderer, &lineBottom);

        // Draw controls
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        this->controls->draw(1215, 670);
    }

    Activity::~Activity() {

    }
}