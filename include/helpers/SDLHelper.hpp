#ifndef SDLHELPER_HPP
#define SDLHELPER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <switch.h>
#include <unordered_map>

// These are a bunch of functions to turn long repetitive actions in SDL
// into one-liners.
namespace SDLHelper {
    // Initialize all required/used parts of SDL
    // Returns true if all succeeded, false otherwise
    bool initSDL();

    // Clean up all initialized parts of SDL
    void exitSDL();

    // Clears the screen (renderer) with the specified colour
    void clearScreen();

    // Wrapper for DestroyTexture
    void destroyTexture(SDL_Texture *);

    // Wrapper for QueryTexture
    void getDimensions(SDL_Texture *, int *, int *);

    // Sets the draw colour
    void setColour(SDL_Color);

    // === DRAWING FUNCTIONS ===
    // -> Draw directly to the screen/renderer
    // Update screen
    void draw();

    // Draw a rectangle with the specified position and size
    void drawRect(int, int, int, int);

    // Draw text at specified x and y coordinate with given font size
    void drawText(const char *, SDL_Color, int, int, int, bool = false);

    // Draw provided texture at specified coordinates
    // width and height are optional
    // If a colour is provided the texture is tinted
    void drawTexture(SDL_Texture *, int, int, int = -1, int = -1, int = -1, int = -1);
    void drawTexture(SDL_Texture *, SDL_Color, int, int, int = -1, int = -1, int = -1, int = -1);

    // === RENDERING FUNCTIONS ===
    // -> Draw to a texture and return it
    // -> The caller must destroy the texture

    // Merges two textures to one side by side (with optional spacing)
    SDL_Texture * renderMergeTextures(SDL_Texture *, SDL_Texture *, int = 0);

    // Reads an image from a pointer to it and returns a texture containing it
    SDL_Texture * renderImage(u8 *, size_t);

    // Returns a texture with the specified text drawn at the specified font size
    // Always drawn in white!
    SDL_Texture * renderText(const char *, int, bool = false);

    // Same as renderText but wraps text at given width
    SDL_Texture * renderTextWrapped(const char *, int, uint32_t, bool = false);
};

#endif