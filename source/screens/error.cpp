#include "error.hpp"
#include <switch.h>

namespace Screen {
    Error::Error(SDL_Renderer * r, struct Theme * t, bool * b, std::string msg) : Screen::Screen(r, t, b) {
        // Set error message
        this->message = msg;

        // Add buttons
        this->controls->add(KEY_PLUS, "Exit", 0);
    }

    void Error::event() {
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed down
                case SDL_JOYBUTTONDOWN:
                    if (events.jbutton.which == 0) {
                        // + button
                        if (events.jbutton.button == key_map[KEY_PLUS]) {
                            *(this->loop) = false;
                        }
                    }
                    break;
            }
        }
    }

    void Error::update(uint32_t dt) {

    }

    void Error::draw() {
        // Clear screen
        SDL_SetRenderDrawColor(this->renderer, this->theme->background, this->theme->background, this->theme->background, 255);
        SDL_RenderClear(this->renderer);

        // Draw top and bottom lines
        SDL_SetRenderDrawColor(this->renderer, this->theme->line, this->theme->line, this->theme->line, 255);
        SDL_Rect lineTop = {30, 87, 1220, 1};
        SDL_Rect lineBottom = {30, 647, 1220, 1};
        SDL_RenderFillRect(this->renderer, &lineTop);
        SDL_RenderFillRect(this->renderer, &lineBottom);

        // Print error title
        SDL_Surface * tmp = TTF_RenderUTF8_Blended(this->heading, "Error", SDL_Color{0, 0, 0, 255});
        SDL_Texture * tex = SDL_CreateTextureFromSurface(this->renderer, tmp);
        int width, height;
        uint32_t format;
        SDL_QueryTexture(tex, &format, nullptr, &width, &height);
        SDL_Rect pos = {65, 44 - (HEADING_FONT_SIZE/2), width, height};
        SDL_RenderCopy(this->renderer, tex, NULL, &pos);
        SDL_FreeSurface(tmp);
        SDL_DestroyTexture(tex);

        // Print error message
        tmp = TTF_RenderUTF8_Blended(this->body, this->message.c_str(), SDL_Color{0, 0, 0, 255});
        tex = SDL_CreateTextureFromSurface(this->renderer, tmp);
        SDL_QueryTexture(tex, &format, nullptr, &width, &height);
        pos = {65, 128, width, height};
        SDL_RenderCopy(this->renderer, tex, NULL, &pos);
        SDL_FreeSurface(tmp);
        SDL_DestroyTexture(tex);

        // Draw controls
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        this->controls->draw(1215, 670);

        SDL_RenderPresent(renderer);
    }

    Error::~Error() {
        // Free fonts
        TTF_CloseFont(this->heading);
        TTF_CloseFont(this->body);
    }
}