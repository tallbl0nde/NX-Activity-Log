#include "loading.hpp"
#include <switch.h>

namespace Screen {
    Loading::Loading(SDL_Renderer * r, struct Theme * t, bool * b, char * c) : Screen::Screen(r, t, b) {
        // Set Loading message
        this->status = c;
    }

    void Loading::event() {

    }

    void Loading::update(uint32_t dt) {

    }

    void Loading::draw() {
        // Clear screen
        SDL_SetRenderDrawColor(this->renderer, this->theme->background, this->theme->background, this->theme->background, 255);
        SDL_RenderClear(this->renderer);

        // Draw top and bottom lines
        SDL_SetRenderDrawColor(this->renderer, this->theme->line, this->theme->line, this->theme->line, 255);
        SDL_Rect lineTop = {30, 87, 1220, 1};
        SDL_Rect lineBottom = {30, 647, 1220, 1};
        SDL_RenderFillRect(this->renderer, &lineTop);
        SDL_RenderFillRect(this->renderer, &lineBottom);

        // Print loading title
        SDL_Surface * tmp = TTF_RenderUTF8_Blended(this->heading, "Loading...", SDL_Color{0, 0, 0, 255});
        SDL_Texture * tex = SDL_CreateTextureFromSurface(this->renderer, tmp);
        int width, height;
        uint32_t format;
        SDL_QueryTexture(tex, &format, nullptr, &width, &height);
        SDL_Rect pos = {65, 44 - (HEADING_FONT_SIZE/2), width, height};
        SDL_RenderCopy(this->renderer, tex, NULL, &pos);
        SDL_FreeSurface(tmp);
        SDL_DestroyTexture(tex);

        SDL_RenderPresent(renderer);
    }

    Loading::~Loading() {

    }
}