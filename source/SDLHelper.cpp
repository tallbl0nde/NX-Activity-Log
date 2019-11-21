#include "SDLHelper.hpp"
#include "utils.hpp"

// === SDL RENDERING ===
// SDL Renderer instance
static SDL_Renderer * renderer;
// SDL Window instance
static SDL_Window * window;

// === FONT OBJECTS ===
// Stores data about Extended font (icons)
static PlFontData ext_font_data;
// Stores data about Standard font
static PlFontData std_font_data;
// Map filled with TTF_Font * (standard) - referenced by font size and created if doesn't exist
static std::unordered_map<int, TTF_Font *> std_font;
// Same but for extended
static std::unordered_map<int, TTF_Font *> ext_font;

namespace SDLHelper {
    bool initSDL() {
        // Init main SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
            SDL_Log("Unable to initialize SDL!");
            return false;
        }

        // Initialize SDL_ttf
        if (TTF_Init() == -1) {
            SDL_Log("Unable to initialize SDL_ttf!");
            return false;
        }

        // Create SDL Window
        window = SDL_CreateWindow("window", 0, 0, WIDTH, HEIGHT, 0);
        if (!window) {
            SDL_Log("Unable to create SDL window %s\n", SDL_GetError());
            return false;
        }

        // Create SDL Renderer
        renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            SDL_Log("Unable to create SDL renderer %s\n", SDL_GetError());
            return false;
        }

        // Prepare controller (only railed for now)
        if (SDL_JoystickOpen(0) == NULL) {
            SDL_Log("Unable to open joystick 0 %s\n", SDL_GetError());
            return false;
        }

        // Enable blending (transparency)
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // Enable antialiasing
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        // Load fonts
        plGetSharedFontByType(&std_font_data, PlSharedFontType_Standard);
        plGetSharedFontByType(&ext_font_data, PlSharedFontType_NintendoExt);

        return true;
    }

    void exitSDL() {
        // Delete created fonts
        for (auto it = std_font.begin(); it != std_font.end(); it++) {
            TTF_CloseFont(it->second);
        }
        for (auto it = ext_font.begin(); it != ext_font.end(); it++) {
            TTF_CloseFont(it->second);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    void clearScreen() {
        SDL_RenderClear(renderer);
    }

    void destroyTexture(SDL_Texture * t) {
        SDL_DestroyTexture(t);
    }

    void getDimensions(SDL_Texture * t, int * w, int * h) {
        SDL_QueryTexture(t, nullptr, nullptr, w, h);
    }

    void setColour(SDL_Color c) {
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    }

    // === DRAWING FUNCTIONS ===

    void draw() {
        SDL_RenderPresent(renderer);
    }

    void drawRect(int x, int y, int w, int h) {
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = w;
        r.h = h;
        SDL_RenderFillRect(renderer, &r);
    }

    void drawText(const char * str, SDL_Color c, int x, int y, int font_size, bool ext) {
        // Render text to texture and draw
        SDL_Texture * tex = renderText(str, font_size, ext);
        drawTexture(tex, c, x, y);
        SDL_DestroyTexture(tex);
    }

    void drawTexture(SDL_Texture * tex, int x, int y, int w, int h, int tx, int tw) {
        // Get dimensions
        int width, height;
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
        if (w == -1) {
            w = width;
        }
        if (h == -1) {
            h = height;
        }

        // Scale if necessary + render
        if (tx != -1 && tw != -1) {
            SDL_Rect s = {tx, 0, tw, height};
            SDL_Rect r = {x, y, w, h};
            SDL_RenderCopy(renderer, tex, &s, &r);
        } else {
            SDL_Rect r = {x, y, w, h};
            SDL_RenderCopy(renderer, tex, nullptr, &r);
        }
    }

    void drawTexture(SDL_Texture * tex, SDL_Color c, int x, int y, int w, int h, int tx, int tw) {
        // Set color
        SDL_SetTextureColorMod(tex, c.r, c.g, c.b);
        SDL_SetTextureAlphaMod(tex, c.a);

        // Draw texture
        drawTexture(tex, x, y, w, h, tx, tw);

        // Reset color
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 255);
    }

    // === RENDERING FUNCTIONS ===

    SDL_Texture * renderImage(u8 * ptr, size_t size) {
        SDL_Surface * tmp = IMG_Load_RW(SDL_RWFromMem(ptr, size), 1);
        SDL_Texture * tex = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_FreeSurface(tmp);
        return tex;
    }

    SDL_Texture * renderMergeTextures(SDL_Texture * left, SDL_Texture * right, int gap) {
        // Determine width and height of both
        int leftW, leftH = 0;
        int rightW, rightH = 0;
        uint32_t format;
        SDL_QueryTexture(left, &format, nullptr, &leftW, &leftH);
        SDL_QueryTexture(right, nullptr, nullptr, &rightW, &rightH);
        int width = leftW + rightW + gap;
        int height = std::max(leftH, rightH);

        // "Merge" both into one texture
        SDL_Texture * tex = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, width, height);
        SDL_SetRenderTarget(renderer, tex);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);

        SDL_Rect r1 = {0, 0, leftW, leftH};
        SDL_RenderCopy(renderer, left, nullptr, &r1);

        SDL_Rect r2 = {leftW + gap, (height - rightH)/2, rightW, rightH};
        SDL_RenderCopy(renderer, right, nullptr, &r2);

        // Reset renderer
        SDL_RenderPresent(renderer);
        SDL_SetRenderTarget(renderer, nullptr);

        return tex;
    }

    SDL_Texture * renderText(const char * str, int font_size, bool ext) {
        SDL_Surface * tmp;

        // If font doesn't exist create it
        // Use Extended font
        if (ext) {
            if (ext_font.find(font_size) == ext_font.end()) {
                TTF_Font * f = TTF_OpenFontRW(SDL_RWFromMem(ext_font_data.address, ext_font_data.size), 1, font_size);
                ext_font[font_size] = f;
            }
            tmp = TTF_RenderUTF8_Blended(ext_font[font_size], str, SDL_Color{255, 255, 255, 255});
        // Use standard font
        } else {
            if (std_font.find(font_size) == std_font.end()) {
                TTF_Font * f = TTF_OpenFontRW(SDL_RWFromMem(std_font_data.address, std_font_data.size), 1, font_size);
                std_font[font_size] = f;
            }
            tmp = TTF_RenderUTF8_Blended(std_font[font_size], str, SDL_Color{255, 255, 255, 255});
        }

        // Render text
        SDL_Texture * tex = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_FreeSurface(tmp);

        return tex;
    }
};