#include "controls.hpp"

#include <algorithm>
#include "SDLHelper.hpp"
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
    Controls::Controls() {
        // Fill array with empty structs
        for (int i = 0; i < KEY_MAP_SIZE; i++) {
            struct Button b;
            this->buttons[i] = b;
        }
    }

    void Controls::add(HidControllerKeys k, std::string str, char pos) {
        int key = Utils::key_map[k];

        // Create button texture
        SDLHelper::setColour(0, 0, 0, 255);
        SDL_Texture * icon = SDLHelper::renderText(key_char[key].c_str(), ICON_FONT_SIZE);

        // Create text texture
        SDL_Texture * text = SDLHelper::renderText(str.c_str(), STR_FONT_SIZE);

        // Merge the two together
        SDL_Texture * tex = SDLHelper::renderMergeTextures(icon, text, TEX_GAP);
        SDLHelper::destroyTexture(icon);
        SDLHelper::destroyTexture(text);

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
        int key = Utils::key_map[k];

        // Delete texture and hide the specified key
        if (this->buttons[key].texture != nullptr) {
            SDLHelper::destroyTexture(this->buttons[key].texture);
        }
        this->buttons[key].show = false;
    }

    void Controls::enable(HidControllerKeys k) {
        // Set enabled bool
        this->buttons[Utils::key_map[k]].enabled = true;
    }

    void Controls::disable(HidControllerKeys k) {
        // Unset enabled bool
        this->buttons[Utils::key_map[k]].enabled = false;
    }

    void Controls::draw(int x, int y) {
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
                SDLHelper::getDimensions(vec[i].texture, &w, &h);
                x -= w;

                // Copy texture
                w = w;
                h = h;
                SDLHelper::drawTexture(vec[i].texture, x, y, w, h);

                // Prepare for next button
                x -= BUTTON_GAP;
            }
        }
    }

    Controls::~Controls() {
        // Destroy any remaining textures
        for (int i = 0; i < KEY_MAP_SIZE; i++) {
            if (this->buttons[i].texture != nullptr) {
                SDLHelper::destroyTexture(this->buttons[i].texture);
            }
        }
    }
}