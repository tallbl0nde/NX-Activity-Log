#include <algorithm>
#include "controls.hpp"
#include <vector>

// Parameters to adjust appearance
#define BUTTON_GAP 40
#define BUTTON_LIGHT 20
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
    Controls::Controls(int x, int y, int w, int h) : Drawable(x, y, w, h) {
        // Fill array with empty structs
        for (int i = 0; i < KEY_MAP_SIZE; i++) {
            struct Button b;
            this->buttons[i] = b;
        }
    }

    void Controls::add(HidControllerKeys k, std::string str, char pos) {
        int key = Utils::key_map[k];

        // Create button texture
        SDLHelper::setColour(this->theme->getText());
        SDL_Texture * icon = SDLHelper::renderText(key_char[key].c_str(), ICON_FONT_SIZE, true);

        // Create text texture
        SDL_Texture * text = SDLHelper::renderText(str.c_str(), STR_FONT_SIZE);

        // Merge the two together
        SDL_Texture * tex = SDLHelper::renderMergeTextures(icon, text, TEX_GAP);
        SDLHelper::destroyTexture(icon);
        SDLHelper::destroyTexture(text);

        // Create struct
        int w, h;
        SDLHelper::getDimensions(tex, &w, &h);
        this->remove(k);
        struct Button b;
        b.texture = tex;
        b.tex_w = w;
        b.tex_h = h;
        b.pos = pos;
        b.show = true;
        b.enabled = true;
        b.touched = false;
        this->buttons[key] = b;

        // Update all buttons positions
        // Vector stores a sorted copy of structs
        std::vector<struct Button> vec(std::begin(this->buttons), std::end(this->buttons));
        std::sort(vec.begin(), vec.end(), [](struct Button lhs, struct Button rhs){
            return lhs.pos < rhs.pos;
        });

        // Loop over vector and update x values
        int tmp_x = this->x + this->w;
        for (unsigned int i = 0; i < vec.size(); i++) {
            if (vec[i].show) {
                // Shift coordinates
                tmp_x -= vec[i].tex_w;

                // Set xPos
                for (unsigned int j = 0; j < KEY_MAP_SIZE; j++) {
                    if (this->buttons[j].show && this->buttons[j].pos == vec[i].pos) {
                        this->buttons[j].tex_x = tmp_x;
                        break;
                    }
                }

                // Prepare for next button
                tmp_x -= BUTTON_GAP;
            }
        }
    }

    void Controls::remove(HidControllerKeys k) {
        int key = Utils::key_map[k];

        // Delete texture and hide the specified key
        if (this->buttons[key].texture != nullptr) {
            SDLHelper::destroyTexture(this->buttons[key].texture);
            this->buttons[key].texture = nullptr;
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

    void Controls::draw() {
        for (unsigned int i = 0; i < KEY_MAP_SIZE; i++) {
            if (!this->buttons[i].show) {
                continue;
            }

            // Dim if disabled
            if (this->buttons[i].enabled) {
                SDLHelper::drawTexture(this->buttons[i].texture, this->theme->getText(), this->buttons[i].tex_x, this->y, this->buttons[i].tex_w, this->buttons[i].tex_h);
            } else {
                SDLHelper::drawTexture(this->buttons[i].texture, this->theme->getMutedText(), this->buttons[i].tex_x, this->y, this->buttons[i].tex_w, this->buttons[i].tex_h);
            }

            // Draw rectangle on top if touched
            if (this->buttons[i].touched) {
                SDLHelper::setColour(this->theme->getTouchOverlay());
                SDLHelper::drawRect(this->buttons[i].tex_x - BUTTON_LIGHT, this->y - BUTTON_LIGHT, this->buttons[i].tex_w + BUTTON_LIGHT*2, this->buttons[i].tex_h + BUTTON_LIGHT*2);
            }
        }
    }

    void Controls::touched(uint32_t type, float tx, float ty) {
        switch (type) {
            // Pressed
            case SDL_FINGERDOWN:
                // Loop over shown controls and set touched if so
                for (unsigned int i = 0; i < KEY_MAP_SIZE; i++) {
                    if (this->buttons[i].show && this->buttons[i].enabled) {
                        if (tx >= this->buttons[i].tex_x - BUTTON_LIGHT && tx <= this->buttons[i].tex_x + this->buttons[i].tex_w + BUTTON_LIGHT && ty >= this->y - BUTTON_LIGHT && ty <= this->y + this->buttons[i].tex_h + BUTTON_LIGHT) {
                            this->buttons[i].touched = true;
                            break;
                        }
                    }
                }
                break;

            // Moved
            case SDL_FINGERMOTION:
                // If moved above controls treat as lifted finger
                if (ty < (this->y - BUTTON_LIGHT)) {
                    this->touched(SDL_FINGERUP, tx, ty);
                } else {
                    // Otherwise check if moved outside of active button
                    for (unsigned int i = 0; i < KEY_MAP_SIZE; i++) {
                        if (this->buttons[i].touched && (tx < this->buttons[i].tex_x - BUTTON_LIGHT || tx > this->buttons[i].tex_x + this->buttons[i].tex_w + BUTTON_LIGHT || ty < this->y - BUTTON_LIGHT || ty > this->y + this->buttons[i].tex_h + BUTTON_LIGHT)) {
                            this->touched(SDL_FINGERUP, tx, ty);
                            break;
                        }
                    }
                }
                break;

            // Released
            case SDL_FINGERUP:
                // Set all touched to false
                for (unsigned int i = 0; i < KEY_MAP_SIZE; i++) {
                    // Create a button pressed event for the button that was pressed
                    if (this->buttons[i].touched) {
                        if (tx >= this->buttons[i].tex_x - BUTTON_LIGHT && tx <= this->buttons[i].tex_x + this->buttons[i].tex_w + BUTTON_LIGHT && ty >= this->y - BUTTON_LIGHT && ty <= this->y + this->buttons[i].tex_h + BUTTON_LIGHT) {
                            // Send pushed event
                            SDL_Event event;
                            event.type = SDL_JOYBUTTONDOWN;
                            event.jbutton.which = 99;
                            event.jbutton.button = i;
                            event.jbutton.state = SDL_PRESSED;
                            SDL_PushEvent(&event);
                            // Send released event (so basically a verrry fast button press)
                            SDL_Event event2;
                            event2.type = SDL_JOYBUTTONUP;
                            event2.jbutton.which = 99;
                            event2.jbutton.button = i;
                            event2.jbutton.state = SDL_RELEASED;
                            SDL_PushEvent(&event2);
                        }
                    }

                    this->buttons[i].touched = false;
                }
                break;
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