#include "activity.hpp"
#include "ui/list.hpp"
#include "ui/listitem.hpp"
#include <switch.h>

namespace Screen {
    Activity::Activity(SDL_Renderer * r, struct Theme * t, bool * b, User * u, std::vector<Title *> tls) : Screen::Screen(r, t, b) {
        this->list = new UI::List();
        for (int i = 0; i < tls.size(); i++) {
            this->list->addItem(new UI::ListItem(tls[i]));
        }

        this->user = u;
        this->controls->add(KEY_PLUS, "Exit", 0);
    }

    void Activity::event() {
        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    if (events.jbutton.which == 0) {
                        if (events.jbutton.button == key_map[KEY_PLUS]) {
                            *(this->loop) = false;
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
        SDL_SetRenderDrawColor(this->renderer, this->theme->line, this->theme->line, this->theme->line, 255);
        SDL_Rect lineTop = {30, 87, 1220, 1};
        SDL_Rect lineBottom = {30, 647, 1220, 1};
        SDL_RenderFillRect(this->renderer, &lineTop);
        SDL_RenderFillRect(this->renderer, &lineBottom);

        // Draw player icon
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        uint32_t format;
        SDL_QueryTexture(this->user->getImage(), &format, nullptr, nullptr, nullptr);
        SDL_Rect pos = {65, 14, 60, 60};
        SDL_RenderCopy(this->renderer, this->user->getImage(), NULL, &pos);

        // Print heading text
        std::string text = this->user->getUsername() + "'s Play Activity";
        SDL_Surface * tmp = TTF_RenderUTF8_Blended(this->heading, text.c_str(), SDL_Color{0, 0, 0, 255});
        SDL_Texture * tex = SDL_CreateTextureFromSurface(this->renderer, tmp);
        int width, height;
        SDL_QueryTexture(tex, &format, nullptr, &width, &height);
        SDL_Rect pos2 = {150, 44 - (HEADING_FONT_SIZE/2), width, height};
        SDL_RenderCopy(this->renderer, tex, NULL, &pos2);
        SDL_FreeSurface(tmp);
        SDL_DestroyTexture(tex);

        // Draw list of items
        this->list->draw(this->renderer, 100, 100, 700, 400);

        // Draw controls
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        this->controls->draw(1215, 670);
        SDL_RenderPresent(renderer);
    }

    Activity::~Activity() {

    }
}