#include "controls.hpp"

#include <algorithm>
#include <string>
#include <vector>

// Parameters to adjust appearance
#define BUTTON_GAP 40
#define ICON_FONT_SIZE 25
#define STR_FONT_SIZE 22
#define TEX_GAP 13

// "Map" of keys to unicode characters
const std::string key_char[] = {
    "\uE0E0", // KEY_A
    "\uE0E1", // KEY_B
    "\uE0E2", // KEY_X
    "\uE0E3", // KEY_Y
    "\uE0F7", // KEY_LSTICK
    "\uE0F8", // KEY_RSTICK
    "\uE0E4", // KEY_L
    "\uE0E5", // KEY_R
    "\uE0E6", // KEY_ZL
    "\uE0E7", // KEY_ZR
    "\uE0EF", // KEY_PLUS
    "\uE0F0", // KEY_MINUS
    "\uE0ED", // KEY_DLEFT
    "\uE0EB", // KEY_DUP
    "\uE0EE", // KEY_DRIGHT
    "\uE0EC", // KEY_DDOWN
    "", // KEY_LSTICK_LEFT
    "", // KEY_LSTICK_UP
    "", // KEY_LSTICK_RIGHT
    "", // KEY_LSTICK_DOWN
    "", // KEY_RSTICK_LEFT
    "", // KEY_RSTICK_UP
    "", // KEY_RSTICK_RIGHT
    "", // KEY_RSTICK_DOWN
    "\uE0E8", // KEY_SL_LEFT
    "\uE0E9", // KEY_SR_LEFT
    "\uE0E8", // KEY_SL_RIGHT
    "\uE0E9"  // KEY_SL_RIGHT
};

namespace UI {
    Controls::Controls(SDL_Renderer * r) {
        this->renderer = r;

        // Fill array with empty structs
        for (int i = 0; i < KEY_MAP_SIZE; i++) {
            struct Button b;
            this->buttons[i] = b;
        }

        // Create fonts
        PlFontData fontData;
        // Standard font
        plGetSharedFontByType(&fontData, PlSharedFontType_Standard);
        this->stndrd = TTF_OpenFontRW(SDL_RWFromMem(fontData.address, fontData.size), 1, STR_FONT_SIZE);
        if (!this->stndrd) {
            // Handle error
        }
        // NintendoExtended font
        plGetSharedFontByType(&fontData, PlSharedFontType_NintendoExt);
        this->extend = TTF_OpenFontRW(SDL_RWFromMem(fontData.address, fontData.size), 1, ICON_FONT_SIZE);
        if (!this->extend) {
            // Handle error
        }
    }

    void Controls::add(HidControllerKeys k, std::string str, char pos) {
        int key = key_map[k];

        // Create button texture
        SDL_Surface * tmp = TTF_RenderUTF8_Blended(this->extend, key_char[key].c_str(), SDL_Color{0, 0, 0, 255});
        SDL_Texture * icon = SDL_CreateTextureFromSurface(this->renderer, tmp);
        SDL_FreeSurface(tmp);

        // Create text texture
        tmp = TTF_RenderUTF8_Blended(this->stndrd, str.c_str(), SDL_Color{0, 0, 0, 255});
        SDL_Texture * text = SDL_CreateTextureFromSurface(this->renderer, tmp);
        SDL_FreeSurface(tmp);

        // Determine width and height
        int iconW, iconH = 0;
        int textW, textH = 0;
        uint32_t format;
        SDL_QueryTexture(icon, &format, nullptr, &iconW, &iconH);
        SDL_QueryTexture(text, nullptr, nullptr, &textW, &textH);
        int width = iconW + textW + TEX_GAP;
        int height = std::max(iconH, textH);

        // "Merge" both into one texture
        SDL_Texture * tex = SDL_CreateTexture(this->renderer, format, SDL_TEXTUREACCESS_TARGET, width, height);
        SDL_SetRenderTarget(this->renderer, tex);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);

        SDL_Rect r1 = {0, 0, iconW, iconH};
        SDL_RenderCopy(this->renderer, icon, nullptr, &r1);
        SDL_DestroyTexture(icon);

        SDL_Rect r2 = {iconW + TEX_GAP, (height - textH)/2, textW, textH};
        SDL_RenderCopy(this->renderer, text, nullptr, &r2);
        SDL_DestroyTexture(text);

        // Reset renderer
        SDL_RenderPresent(this->renderer);
        SDL_SetRenderTarget(this->renderer, nullptr);

        // Create and store struct
        this->remove(k);
        struct Button b;
        b.texture = tex;
        b.pos = pos;
        b.show = true;
        b.enabled = true;
        this->buttons[key] = b;
    }

    void Controls::remove(HidControllerKeys k) {
        int key = key_map[k];

        // Delete texture and hide the specified key
        if (this->buttons[key].texture != nullptr) {
            SDL_DestroyTexture(this->buttons[key].texture);
        }
        this->buttons[key].show = false;
    }

    void Controls::enable(HidControllerKeys k) {
        // Set enabled bool
        this->buttons[key_map[k]].enabled = true;
    }

    void Controls::disable(HidControllerKeys k) {
        // Unset enabled bool
        this->buttons[key_map[k]].enabled = false;
    }

    void Controls::draw(int xPos, int yPos) {
        // Coordinates of where to print next
        SDL_Rect r = {xPos, yPos, 0, 0};

        // Vector stores a sorted copy of structs
        std::vector<struct Button> vec(std::begin(this->buttons), std::end(this->buttons));
        std::sort(vec.begin(), vec.end(), [](struct Button lhs, struct Button rhs){
                return lhs.pos < rhs.pos;
        });

        // Loop over vector
        for (unsigned int i = 0; i < vec.size(); i++) {
            if (vec[i].show) {
                // Shift coordinates
                int w, h = 0;
                SDL_QueryTexture(vec[i].texture, nullptr, nullptr, &w, &h);
                r.x -= w;

                // Copy texture
                r.w = w;
                r.h = h;
                SDL_RenderCopy(this->renderer, vec[i].texture, NULL, &r);

                // Prepare for next button
                r.x -= BUTTON_GAP;
            }
        }
    }

    Controls::~Controls() {
        // Destroy any remaining textures
        for (int i = 0; i < KEY_MAP_SIZE; i++) {
            if (this->buttons[i].texture != nullptr) {
                SDL_DestroyTexture(this->buttons[i].texture);
            }
        }

        // Free fonts
        TTF_CloseFont(this->extend);
        TTF_CloseFont(this->stndrd);
    }
}