#include <algorithm>
#include <iostream>
#include <string>
#include <switch.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Title.hpp"
#include "User.hpp"
#include "utils.h"
#include <vector>

#include "screens/activity.hpp"
#include "screens/error.hpp"
#include "screens/loading.hpp"

// Forward declarations of functions because I like them at the end :)
u128 getUserID();
std::string getUserUsername(u128);
SDL_Texture * getUserImage(u128, SDL_Renderer *);
std::vector<Title *> getTitleObjects(u128, SDL_Renderer *);

int main(int argc, char * argv[]){
    // Status variables
    bool appRunning = true;
    bool accInit, fsInit, nsInit, pdmInit, plInit = false;
    Result rc = 0;

    // Current user ID
    u128 userID;

    // Vector contains all title objects
    std::vector<Title *> titles;

    // Start required services
    rc = accountInitialize();
    if (R_SUCCEEDED(rc)) {
        accInit = true;
    }
    rc = fsInitialize();
    if (R_SUCCEEDED(rc)) {
        fsInit = true;
    }
    rc = nsInitialize();
    if (R_SUCCEEDED(rc)) {
        nsInit = true;
    }
    rc = pdmqryInitialize();
    if (R_SUCCEEDED(rc)) {
        pdmInit = true;
    }
    rc = plInitialize();
    if (R_SUCCEEDED(rc)) {
        plInit = true;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        SDL_Log("Unable to initialize SDL!");
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Log("Unable to initialize SDL_ttf!");
        return -1;
    }

    // Create SDL Window
    SDL_Window * window = SDL_CreateWindow("window", 0, 0, 1280, 720, 0);
    if (!window) {
        SDL_Log("Unable to create SDL window %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create SDL Renderer
    SDL_Renderer * renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Unable to create SDL renderer %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Enable antialiasing
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    // Prepare controller (only railed for now)
    if (SDL_JoystickOpen(0) == NULL) {
        SDL_Log("Unable to open joystick 0 %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Clock to measure time between draw
    struct Clock clock;

    // Theme struct
    struct Theme theme = theme_light;

    UI::Screen * screen = new Screen::Loading(renderer, &theme, &appRunning, nullptr);
    User * user = nullptr;

    // Only proceed if no errors
    if (accInit && fsInit && nsInit && pdmInit && plInit) {
        // Loading is kinda dodge
        bool error = false;
        screen->draw();

        // Stage 0: Get User ID
        userID = getUserID();
        if (userID == 0) {
            // Unable to get user ID - raise error
            screen = new Screen::Error(renderer, &theme, &appRunning, "Unable to get a User ID... Did you select a user?");
            error = true;
        }

        // Stage 1: Get user's data
        std::string username;
        SDL_Texture * image;
        if (!error) {
            username = getUserUsername(userID);
            image = getUserImage(userID, renderer);

            if (username == "" || image == nullptr) {
                screen = new Screen::Error(renderer, &theme, &appRunning, "An unexpected error occurred while parsing user data");
                error = true;
            }
        }

        // Stage 1.5: Create user object
        if (!error) {
            user = new User(userID, username, image);
        }

        // Stage 2: Get installed titles and create Title objects
        if (!error) {
            titles = getTitleObjects(userID, renderer);
        }

        // Stage 3: Initialize screen
        if (!error) {
            screen = new Screen::Activity(renderer, &theme, &appRunning, user, titles);
        }

        // Infinite draw loop until exit
        while (appRunning) {
            // Check for events first
            screen->event();

            // Get time since last draw and update
            clock.tick();
            screen->update(clock.delta);

            // Render screen
            screen->draw();
        }
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    // Cleanup resources
    if (accInit) {
        accountExit();
    }
    if (fsInit) {
        fsExit();
    }
    if (nsInit) {
        nsExit();
    }
    if (pdmInit) {
        pdmqryExit();
    }
    if (plInit) {
        plExit();
    }

    // Delete user object
    if (user != nullptr) {
        delete user;
    }

    // Delete any created titles
    while (titles.size() > 0) {
        delete titles[0];
        titles.erase(titles.begin());
    }

    return 0;
}

#define ARG_SIZE 0xA0
#define OUT_SIZE 0x18
// Returns the userID obtained by the PlayerSelect applet
u128 getUserID() {
    u128 userID = 0;

    LibAppletArgs args;
    libappletArgsCreate(&args, 0x10000);
    u8 args2[ARG_SIZE]  = {0};
    u8 out[OUT_SIZE] = {0};
    size_t out_size;

    Result rc = libappletLaunch(AppletId_playerSelect, &args, args2, ARG_SIZE, out, OUT_SIZE, &out_size);
    if (R_SUCCEEDED(rc)) {
        u64 res = *(u64 *) out;
        u128 uid = *(u128 *) &out[8];
        if (res == 0) {
            userID = uid;
        }
    }

    return userID;
}

// Returns username for user with given ID
std::string getUserUsername(u128 userID) {
    AccountProfile profile;
    AccountProfileBase profile_base;

    Result rc = accountGetProfile(&profile, userID);
    if (R_SUCCEEDED(rc)){
        rc = accountProfileGet(&profile, NULL, &profile_base);
        if (R_SUCCEEDED(rc)){
            return profile_base.username;
        }
        accountProfileClose(&profile);
    }

    // Return blank string on error
    return "";
}

// Return texture with user image
SDL_Texture * getUserImage(u128 userID, SDL_Renderer * renderer) {
    AccountProfile profile;
    SDL_Texture * texture = nullptr;

    Result rc = accountGetProfile(&profile, userID);
    if (R_SUCCEEDED(rc)){
        u8 * buffer;
        size_t img_size, tmp;

        // Get image size and allocate memory
        rc = accountProfileGetImageSize(&profile, &img_size);
        if (R_SUCCEEDED(rc)) {
            buffer = (u8 *) malloc(img_size);

            // Get image
            rc = accountProfileLoadImage(&profile, buffer, img_size, &tmp);
            if (R_SUCCEEDED(rc)) {
                // Create texture
                SDL_Surface * tmp = IMG_Load_RW(SDL_RWFromMem(buffer, img_size), 1);
                texture = SDL_CreateTextureFromSurface(renderer, tmp);
                SDL_FreeSurface(tmp);
            }

            free(buffer);
        }
        accountProfileClose(&profile);
    }

    // Will be nullptr if any error occurs
    return texture;
}

// Reads all installed title IDs and creates Title objects using them
std::vector<Title *> getTitleObjects(u128 userID, SDL_Renderer * renderer) {
    Result rc;

    // Vector containing all found titleIDs
    std::vector<u64> titleIDs;

    // Get installed titles (including unplayed)
    NsApplicationRecord info;
    size_t count = 0;
    size_t total = 0;
    while (true){
        rc = nsListApplicationRecord(&info, sizeof(NsApplicationRecord), count, &total);
        // Break if at the end or no titles
        if (R_FAILED(rc) || total == 0){
            break;
        }
        count++;
        titleIDs.push_back(info.titleID);
    }

    // Get played titles (including deleted)
    FsSaveDataIterator fsIterator;
    rc = fsOpenSaveDataIterator(&fsIterator, FsSaveDataSpaceId_NandUser);
    if (R_SUCCEEDED(rc)){
        // Iterate over all save data to get titleIDs
        FsSaveDataInfo info;
        while (true){
            size_t total = 0;
            rc = fsSaveDataIteratorRead(&fsIterator, &info, 1, &total);
            // Break if at the end or no titles
            if (R_FAILED(rc) || total == 0){
                break;
            }
            titleIDs.push_back(info.titleID);
        }
        fsSaveDataIteratorClose(&fsIterator);
    }

    // Remove duplicate titleIDs
    std::sort(titleIDs.begin(), titleIDs.end());
    titleIDs.resize(std::distance(titleIDs.begin(), std::unique(titleIDs.begin(), titleIDs.end())));

    // Create Titles
    std::vector<Title *> titles;
    for (size_t i = 0; i < titleIDs.size(); i++) {
        titles.push_back(new Title(titleIDs[i], userID, renderer));
    }

    return titles;
}