// #include "User.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <switch.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Title.hpp"
#include "utils.h"
#include <vector>

#include "screens/activity.hpp"

// Forward declarations of functions because I like them at the end :)
u128 getUserID();
std::vector<Title *> getTitleObjects(u128);

int main(int argc, char * argv[]){
    // Status variables
    bool appRunning = true;
    bool fsInit, nsInit, pdmInit, plInit = false;
    Result rc = 0;

    // Current user ID
    u128 userID;

    // Vector contains all title objects
    std::vector<Title *> titles;

    // Start required services
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

    // Screen pointer
    UI::Screen * screen = new Screen::Activity(renderer, &theme, &appRunning);

    // Only proceed if no errors
    if (fsInit && nsInit && pdmInit && plInit) {
        // Loading is kinda dodge
        char stage = 0;
        screen->draw();

        // Stage 0: Get User ID
        userID = getUserID();
        if (userID == 0) {
            // Do something
        }

        // Stage 1: Get installed titles and create Title objects
        titles = getTitleObjects(userID);

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

    // Delete any created titles
    while (titles.size() > 0) {
        delete titles[0];
        titles.erase(titles.begin());
    }

    return 0;
}

// Returns the userID obtained by the PlayerSelect applet
u128 getUserID() {
    // Struct to store returned data
    struct UserSelectData{
        u64 result;
        u128 userID;
    } ret;

    // Data to pass into applet
    u8 in[0xA0] = {0};
    in[0x96] = 1;

    // A lot of stuff to launch the User Select Applet
    AppletHolder app_holder;
    AppletStorage app_stor;
    AppletStorage app_stor2;
    LibAppletArgs app_args;
    appletCreateLibraryApplet(&app_holder, AppletId_playerSelect, LibAppletMode_AllForeground);
    libappletArgsCreate(&app_args, 0);
    libappletArgsPush(&app_args, &app_holder);
    appletCreateStorage(&app_stor2, 0xA0);
    appletStorageWrite(&app_stor2, 0, in, 0xA0);
    appletHolderPushInData(&app_holder, &app_stor2);
    appletHolderStart(&app_holder);
    while (appletHolderWaitInteractiveOut(&app_holder)){
    }
    appletHolderJoin(&app_holder);
    appletHolderPopOutData(&app_holder, &app_stor);
    appletStorageRead(&app_stor, 0, &ret, 24);
    appletHolderClose(&app_holder);
    appletStorageClose(&app_stor);
    appletStorageClose(&app_stor2);

    // Return 0 if an error occurred
    if (ret.result != 0){
        return 0;
    }

    // Otherwise return the obtained ID
    return ret.userID;
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
    for (int i = 0; i < titleIDs.size(); i++){
        PdmPlayStatistics stats;
        rc = pdmqryQueryPlayStatisticsByApplicationIdAndUserAccountId(titleIDs[i], userID, &stats);
        if (R_SUCCEEDED(rc)){
            // Get application name
            NsApplicationControlData data;
            NacpLanguageEntry * lang = nullptr;

            rc = nsGetApplicationControlData(1, titleIDs[i], &data, sizeof(NsApplicationControlData), NULL);
            if (R_SUCCEEDED(rc)){
                rc = nacpGetLanguageEntry(&data.nacp, &lang);
                if (R_SUCCEEDED(rc)){
                    titles.push_back(new Title(stats, lang->name));
                }
            }
        }
    }

    return titles;
}