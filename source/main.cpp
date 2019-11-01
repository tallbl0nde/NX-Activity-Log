#include <algorithm>
#include "SDLHelper.hpp"
#include <string>
#include <switch.h>
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
SDL_Texture * getUserImage(u128);
std::vector<Title *> getTitleObjects(u128);

int main(int argc, char * argv[]){
    // Status variables
    bool appRunning = true;
    bool accInit, fsInit, nsInit, pdmInit, plInit, SDLInit = false;
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
    SDLInit = SDLHelper::initSDL();

    // Clock to measure time between draw
    struct Utils::Clock clock;

    // Theme struct
    struct Theme theme = theme_light;

    UI::Screen * screen = new Screen::Loading(&theme, &appRunning, nullptr);
    User * user = nullptr;

    // Only proceed if no errors
    if (accInit && fsInit && nsInit && pdmInit && plInit && SDLInit) {
        // Loading is kinda dodge
        bool error = false;
        screen->draw();
        SDLHelper::draw();

        // Stage 0: Get User ID
        userID = getUserID();
        if (userID == 0) {
            // Unable to get user ID - raise error
            screen = new Screen::Error(&theme, &appRunning, "Unable to get a User ID... Did you select a user?");
            error = true;
        }

        // Stage 1: Get user's data
        std::string username;
        SDL_Texture * image;
        if (!error) {
            username = getUserUsername(userID);
            image = getUserImage(userID);

            if (username == "" || image == nullptr) {
                screen = new Screen::Error(&theme, &appRunning, "An unexpected error occurred while parsing user data");
                error = true;
            }
        }

        // Stage 1.5: Create user object
        if (!error) {
            user = new User(userID, username, image);
        }

        // Stage 2: Get installed titles and create Title objects
        if (!error) {
            titles = getTitleObjects(userID);
        }

        // Stage 3: Initialize screen
        if (!error) {
            screen = new Screen::Activity(&theme, &appRunning, user, titles);
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

            // FPS Counter
            std::string fps = "FPS: " + std::to_string(1000.0/clock.delta);
            SDLHelper::setColour(0, 0, 0, 255);
            SDLHelper::drawText(fps.c_str(), 800, 10, 20);
            SDLHelper::draw();
        }
    }

    // Clean up SDL
    SDLHelper::exitSDL();

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
SDL_Texture * getUserImage(u128 userID) {
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
                texture = SDLHelper::renderImage(buffer, img_size);
            }

            free(buffer);
        }
        accountProfileClose(&profile);
    }

    // Will be nullptr if any error occurs
    return texture;
}

// Reads all installed title IDs and creates Title objects using them
std::vector<Title *> getTitleObjects(u128 userID) {
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
        titles.push_back(new Title(titleIDs[i], userID));
    }

    return titles;
}