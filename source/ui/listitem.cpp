#include <algorithm>
#include <ctime>
#include <iomanip>
#include "listitem.hpp"
#include <sstream>
#include "utils.h"

// Font sizes
#define TITLE_FONT_SIZE 25
#define SUB_FONT_SIZE 20

namespace UI {
    ListItem::ListItem(Title * t) {
        // Format timestamp for printing
        std::stringstream ss;
        std::time_t timestamp = pdmPlayTimestampToPosix(t->getLastPlayed());
        ss << std::put_time(std::localtime(&timestamp), "%d %B %Y %I:%M %p");
        this->last_played = ss.str();

        this->title = t;
        this->selected = false;

        // Create fonts
        PlFontData fontData;
        plGetSharedFontByType(&fontData, PlSharedFontType_Standard);
        this->title_font = TTF_OpenFontRW(SDL_RWFromMem(fontData.address, fontData.size), 1, TITLE_FONT_SIZE);
        if (!this->title_font) {
            // Handle error
        }
        this->sub_font = TTF_OpenFontRW(SDL_RWFromMem(fontData.address, fontData.size), 1, SUB_FONT_SIZE);
        if (!this->sub_font) {
            // Handle error
        }
    }

    void ListItem::update(uint32_t dt) {

    }

    void ListItem::draw(SDL_Renderer * renderer, int x, int y, int w, int h) {
        // Draw background
        SDL_Rect rect = {x, y, w, h};
        if (this->selected) {
            SDL_RenderFillRect(renderer, &rect);
        }

        // Draw outlines
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_Rect lineTop = {x, y, w, 1};
        SDL_Rect lineBottom = {x, y + h, w, 1};
        SDL_RenderFillRect(renderer, &lineTop);
        SDL_RenderFillRect(renderer, &lineBottom);

        // Draw icon
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        rect.x = x + 10;
        rect.y = y + 10;
        rect.w = h - 20;
        rect.h = rect.w;
        SDL_RenderCopy(renderer, this->title->getIcon(), NULL, &rect);

        // Print name
        SDL_Surface * tmp = TTF_RenderUTF8_Blended(this->title_font, this->title->getName().c_str(), SDL_Color{0, 0, 0, 255});
        SDL_Texture * tex = SDL_CreateTextureFromSurface(renderer, tmp);
        int width, height;
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
        rect.x += rect.w + 20;
        rect.y += 15;
        rect.w = width;
        rect.h = height;
        SDL_RenderCopy(renderer, tex, nullptr, &rect);
        SDL_FreeSurface(tmp);
        SDL_DestroyTexture(tex);

        // Print play time
        std::string play_time = Utils::formatPlaytime(this->title->getPlaytime());
        tmp = TTF_RenderUTF8_Blended(this->sub_font, play_time.c_str(), SDL_Color{120, 120, 120, 255});
        tex = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
        rect.y += 30;
        rect.w = width;
        rect.h = height;
        SDL_RenderCopy(renderer, tex, nullptr, &rect);
        SDL_FreeSurface(tmp);
        SDL_DestroyTexture(tex);

        // Print last played
        tmp = TTF_RenderUTF8_Blended(this->sub_font, this->last_played.c_str(), SDL_Color{120, 120, 120, 255});
        tex = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
        rect.y += 25;
        rect.w = width;
        rect.h = height;
        SDL_RenderCopy(renderer, tex, nullptr, &rect);
        SDL_FreeSurface(tmp);
        SDL_DestroyTexture(tex);
    }

    ListItem::~ListItem() {
        TTF_CloseFont(this->title_font);
        TTF_CloseFont(this->sub_font);
    }
};