#include <algorithm>
#include "config.hpp"
#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include <string>

// Forward declarations of functions because I like them at the end :)
u128 getUserID();
std::string getUserUsername(u128);
SDL_Texture * getUserImage(u128);
std::vector<Title *> getTitleObjects(u128);

int main(int argc, char * argv[]){
    // Status variables
    bool accInit, fsInit, nsInit, pdmInit, plInit, SDLInit, setInit = false;
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
    rc = setsysInitialize();
    if (R_SUCCEEDED(rc)) {
        setInit = true;
    }

    // Initialize SDL
    SDLInit = SDLHelper::initSDL();

    // Load config
    Config * conf = Config::getConfig();
    conf->readConfig();

    // Clock to measure time between draw
    struct Utils::Clock clock;

    // Initialise and get ScreenManager
    ScreenManager * sm = ScreenManager::getInstance();
    User * user = nullptr;

    // Only proceed if no errors
    if (accInit && fsInit && nsInit && pdmInit && plInit && SDLInit) {
        // Loading is kinda dodge
        bool error = false;

        // Get system theme and set accordingly
        // This doesn't use sm and I want it to
        Screen::Loading loading = Screen::Loading();
        switch (conf->getGeneralTheme()) {
            case T_Auto: {
                ColorSetId thm;
                rc = setsysGetColorSetId(&thm);
                loading.setTheme((bool)thm);
                break;
            }

            case T_Light: {
                loading.setTheme(false);
                break;
            }

            case T_Dark: {
                loading.setTheme(true);
                break;
            }
        }

        // Draw initial loading screen before selection (with fade animation)
        while(!loading.animDone()) {
            clock.tick();
            loading.update(clock.delta);
            loading.draw();
            SDLHelper::draw();
        }

        // Stage 0: Get User ID
        userID = getUserID();
        if (userID == 0 && !error) {
            // Unable to get user ID - raise error
            sm->setScreen(new Screen::Error("Unable to get a User ID... Did you select a user? Tip: Try launching in title mode!"));
            error = true;
        }

        // Stage 1: Get user's data
        std::string username;
        SDL_Texture * image;
        if (!error) {
            username = getUserUsername(userID);
            image = getUserImage(userID);

            if (username == "" || image == nullptr) {
                sm->setScreen(new Screen::Error("An unexpected error occurred while parsing user data"));
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

        if (!error) {
            sm->setScreen(new Screen::Activity(user, titles));
        }

        // Infinite loop until exit
        while (sm->loop()) {
            // Check for events
            sm->event();

            // Get time since last draw and update
            clock.tick();
            sm->update(clock.delta);

            // Render screen
            sm->draw();

            // // Draw FPS
            // std::string ss = "FPS: " + std::to_string((int)(1.0/(clock.delta/1000.0))) + " (" + std::to_string(clock.delta) + " ms)";
            // SDLHelper::drawText(ss.c_str(), SDL_Color{0, 150, 150, 255}, 5, 695, 20);

            SDLHelper::draw();
        }
    }

    // Free screen memory
    sm->free();

    // Save config
    conf->writeConfig();

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
    if (setInit) {
        setExit();
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
    std::string str = "";

    Result rc = accountGetProfile(&profile, userID);
    if (R_SUCCEEDED(rc)){
        rc = accountProfileGet(&profile, NULL, &profile_base);
        if (R_SUCCEEDED(rc)){
            str = profile_base.username;
        }
        accountProfileClose(&profile);
    }

    // Return blank string on error
    return str;
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

    Config * conf = Config::getConfig();
    if (!conf->getHiddenDeleted()) {
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
    }

    // Remove duplicate titleIDs
    std::sort(titleIDs.begin(), titleIDs.end());
    titleIDs.resize(std::distance(titleIDs.begin(), std::unique(titleIDs.begin(), titleIDs.end())));

    // Create Titles
    std::vector<Title *> titles;
    for (size_t i = 0; i < titleIDs.size(); i++) {
        Title * tmp = new Title(titleIDs[i], userID);
        // Delete if not played
        if (conf->getHiddenUnplayed() && tmp->getPlaytime() == 0) {
            delete tmp;
        } else {
            titles.push_back(tmp);
        }
    }

    return titles;
}