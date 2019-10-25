#include "activity.hpp"

namespace Screen {
    Activity::Activity(struct Theme * t, bool * b) : Screen::Screen(t, b){

    }

    void Activity::event() {
        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    if (events.jbutton.which == 0) {
                        if (events.jbutton.button == 10){
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

    void Activity::draw(SDL_Renderer * renderer) {
        // Clear screen
        SDL_SetRenderDrawColor(renderer, this->theme->background, this->theme->background, this->theme->background, 255);
        SDL_RenderClear(renderer);

        // Draw top and bottom lines
        SDL_Rect lineTop = {30, 87, 1220, 1};
        SDL_Rect lineBottom = {30, 647, 1220, 1};

        SDL_SetRenderDrawColor(renderer, this->theme->line, this->theme->line, this->theme->line, 255);
        SDL_RenderFillRect(renderer, &lineTop);
        SDL_RenderFillRect(renderer, &lineBottom);
    }

    Activity::~Activity() {

    }
}