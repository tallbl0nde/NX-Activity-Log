// #include "menus/Error.h"
// #include "menus/Stats.h"
// #include "Title.h"
// #include "User.h"
// #include "utils.h"

// #include <algorithm>
#include <iostream>
#include <string>
// #include <vector>

#include <switch.h>
#include <SDL2/SDL.h>

#include <ui/theme.h>

// // Populate vector with Title objects created from NsApplicationRecord
// void getUserPlayStats(u128 userID, std::vector<u64> titleIDs, std::vector<Title *> &titles){
//     Result rc;

//     // Empty vector if necessary
//     while (titles.size() > 0){
//         delete titles[0];
//         titles.erase(titles.begin());
//     }

//     // Print status
//     moveCursor(0, CONSOLE_HEIGHT-1);
//     std::cout << "Reading play data...";
//     consoleUpdate(NULL);

//     // Create Titles for every titleID
//     for (int i = 0; i < titleIDs.size(); i++){
//         PdmPlayStatistics stats;
//         rc = pdmqryQueryPlayStatisticsByApplicationIdAndUserAccountId(titleIDs[i], userID, &stats);
//         if (R_FAILED(rc)){
//             moveCursor(0, CONSOLE_HEIGHT-1);
//             std::cout << TEXT_RED << "Error: " << TEXT_RESET << "pdmqryPSBAIAUAI() failed: " << rc;
//         }
//         if (R_SUCCEEDED(rc)){
//             // Get application name
//             NsApplicationControlData data;
//             NacpLanguageEntry * lang = nullptr;

//             rc = nsGetApplicationControlData(1, titleIDs[i], &data, sizeof(NsApplicationControlData), NULL);
//             if (R_FAILED(rc)){
//                 moveCursor(0, CONSOLE_HEIGHT-1);
//                 std::cout << TEXT_RED << "Error: " << TEXT_RESET << "nsGetApplicationControlData() failed: " << rc;
//             }
//             if (R_SUCCEEDED(rc)){
//                 rc = nacpGetLanguageEntry(&data.nacp, &lang);
//                 if (R_FAILED(rc)){
//                     moveCursor(0, CONSOLE_HEIGHT-1);
//                     std::cout << TEXT_RED << "Error: " << TEXT_RESET << "nacpGetLanguageEntry() failed: " << rc;
//                 }
//                 if (R_SUCCEEDED(rc)){
//                     titles.push_back(new Title(stats, lang->name));
//                 }
//             }
//         }
//     }
//     consoleUpdate(NULL);
// }

int main(int argc, char * argv[]){
    bool appRunning = true;
    // Status variables
    // Result rc = 0;
    // bool error = false;
    // bool fsInit = false;
    // bool nsInit = false;
    // bool pdmInit = false;

    // u128 userID;

    // std::vector<Title *> titles;
    // std::vector<u64> titleIDs;

    // int lastMenu = M_Dummy;
    // MenuType menu = M_Error;
    // Menu * menuObject = new Menu_Error();

    // Initialize SDL
    SDL_Event event;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0){
        SDL_Log("Unable to initialize SDL!");
        return -1;
    }

    // Create SDL Window
    SDL_Window * window = SDL_CreateWindow("window", 0, 0, 1280, 720, 0);
    if (!window){
        SDL_Log("Unable to create SDL window %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create SDL Renderer
    SDL_Renderer * renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer){
        SDL_Log("Unable to create SDL renderer %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Prepare controller (only railed for now)
    if (SDL_JoystickOpen(0) == NULL){
        SDL_Log("Unable to open joystick 0 %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Draw test screen for now
    bool themeLight = true;
    struct Theme theme = theme_light;

    while (appRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_JOYBUTTONDOWN:
                    if (event.jbutton.which == 0) {
                        if (event.jbutton.button == 10){
                            appRunning = false;
                        } else if (event.jbutton.button == 11){
                            if (themeLight){
                                theme = theme_dark;
                            } else {
                                theme = theme_light;
                            }
                            themeLight = !themeLight;
                        }
                    }
                    break;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, theme.backgroundColour, theme.backgroundColour, theme.backgroundColour, 255);
        SDL_RenderClear(renderer);

        // Draw top and bottom lines
        SDL_Rect lineTop = {30, 87, 1220, 1};
        SDL_Rect lineBottom = {30, 647, 1220, 1};

        SDL_SetRenderDrawColor(renderer, theme.lineColour, theme.lineColour, theme.lineColour, 255);
        SDL_RenderFillRect(renderer, &lineTop);
        SDL_RenderFillRect(renderer, &lineBottom);

        SDL_RenderPresent(renderer);
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

/* #region foff */
    // Get titleIDs of played and installed titles
    // if (!error){
    //     moveCursor(0, CONSOLE_HEIGHT-1);
    //     std::cout << "Getting title data...";
    //     consoleUpdate(NULL);

    //     rc = nsInitialize();
    //     if (R_FAILED(rc)){
    //         moveCursor(0, CONSOLE_HEIGHT-1);
    //         std::cout << TEXT_RED << "Error: " << TEXT_RESET << "nsInitialize() failed: " << rc;
    //         error = true;
    //     }
    //     if (R_SUCCEEDED(rc)){
    //         nsInit = true;
    //         // Get installed titles (including unplayed)
    //         NsApplicationRecord info;
    //         size_t count = 0;
    //         size_t total = 0;
    //         while (true){
    //             rc = nsListApplicationRecord(&info, sizeof(NsApplicationRecord), count, &total);
    //             // Break if at the end or no titles
    //             if (R_FAILED(rc) || total == 0){
    //                 break;
    //             }
    //             count++;
    //             titleIDs.push_back(info.titleID);
    //         }

    //         // Get played titles (including deleted)
    //         rc = fsInitialize();
    //         if (R_FAILED(rc)){
    //             moveCursor(0, CONSOLE_HEIGHT-1);
    //             std::cout << TEXT_RED << "Error: " << TEXT_RESET << "fsInitialize() failed: " << rc;
    //             error = true;
    //         }
    //         if (R_SUCCEEDED(rc)){
    //             // Create save data iterator
    //             FsSaveDataIterator fsIterator;
    //             rc = fsOpenSaveDataIterator(&fsIterator, FsSaveDataSpaceId_NandUser);
    //             if (R_FAILED(rc)){
    //                 moveCursor(0, CONSOLE_HEIGHT-1);
    //                 std::cout << TEXT_RED << "Error: " << TEXT_RESET << "fsOpenSaveDataIterator() failed: " << rc;
    //                 error = true;
    //             }
    //             if (R_SUCCEEDED(rc)){
    //                 // Iterate over all save data to get titleIDs
    //                 FsSaveDataInfo info;
    //                 while (true){
    //                     size_t total = 0;
    //                     rc = fsSaveDataIteratorRead(&fsIterator, &info, 1, &total);
    //                     // Break if at the end or no titles
    //                     if (R_FAILED(rc) || total == 0){
    //                         break;
    //                     }
    //                     titleIDs.push_back(info.titleID);
    //                 }

    //                 fsSaveDataIteratorClose(&fsIterator);
    //             }
    //             fsInit = true;
    //         }

    //         // Remove duplicate titleIDs
    //         std::sort(titleIDs.begin(), titleIDs.end());
    //         titleIDs.resize(std::distance(titleIDs.begin(), std::unique(titleIDs.begin(), titleIDs.end())));
    //     }

    //     consoleUpdate(NULL);
    // }

    // // Initialise pdm
    // if (!error){
    //     rc = pdmqryInitialize();
    //     if (R_FAILED(rc)){
    //         moveCursor(0, CONSOLE_HEIGHT-1);
    //         std::cout << TEXT_RED << "Error: " << TEXT_RESET << "pdmqryInitialize() failed: " << rc;
    //         error = true;
    //     }
    //     if (R_SUCCEEDED(rc)){
    //         pdmInit = true;
    //     }
    // }
    // consoleUpdate(NULL);

    // // No errors: present user selection
    // if (!error){
    //     moveCursor(0, CONSOLE_HEIGHT-1);
    //     std::cout << "Select a user         ";
    //     consoleUpdate(NULL);

    //     // Struct to store returned data
    //     struct UserSelectData{
    //         u64 result;
    //         u128 userID;
    //     } PACKED;
    //     struct UserSelectData ret;

    //     // Data to pass into applet
    //     u8 in[0xA0] = {0};
    //     in[0x96] = 1;

    //     // A lot of stuff to launch the User Select Applet
    //     AppletHolder app_holder;
    //     AppletStorage app_stor;
    //     AppletStorage app_stor2;
    //     LibAppletArgs app_args;
    //     appletCreateLibraryApplet(&app_holder, AppletId_playerSelect, LibAppletMode_AllForeground);
    //     libappletArgsCreate(&app_args, 0);
    //     libappletArgsPush(&app_args, &app_holder);
    //     appletCreateStorage(&app_stor2, 0xA0);
    //     appletStorageWrite(&app_stor2, 0, in, 0xA0);
    //     appletHolderPushInData(&app_holder, &app_stor2);
    //     appletHolderStart(&app_holder);
    //     while (appletHolderWaitInteractiveOut(&app_holder)){
    //     }
    //     appletHolderJoin(&app_holder);
    //     appletHolderPopOutData(&app_holder, &app_stor);
    //     appletStorageRead(&app_stor, 0, &ret, 24);
    //     appletHolderClose(&app_holder);
    //     appletStorageClose(&app_stor);
    //     appletStorageClose(&app_stor2);

    //     // Store returned userID
    //     userID = ret.userID;

    //     // Show an error if none selected
    //     if (ret.result != 0){
    //         moveCursor(0, CONSOLE_HEIGHT-1);
    //         std::cout << TEXT_RED << "Error: " << TEXT_RESET << "No user was selected!";
    //         consoleUpdate(NULL);
    //         menu = M_Error;
    //     } else {
    //         menu = M_Stats;
    //     }
    // }
/* #endregion */

    // while (appletMainLoop()){
    //     // Get key presses
    //     hidScanInput();
    //     u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

    //     // Create/delete menu when changed
    //     if (lastMenu != menu){
    //         clearConsole();
    //         moveCursor(CONSOLE_WIDTH/2, 0);
    //         for (int i = CONSOLE_WIDTH/2; i <= CONSOLE_WIDTH; i++){
    //             std::cout << " ";
    //         }
    //         delete menuObject;
    //         lastMenu = menu;
    //         switch (menu){
    //             // Dummy menu (should never be called)
    //             case M_Dummy:
    //                 menu = M_Error;
    //                 break;
    //             // Error menu
    //             case M_Error:
    //                 menuObject = new Menu_Error();
    //                 break;
    //             // Stats menu
    //             case M_Stats:
    //                 getUserPlayStats(userID, titleIDs, titles);
    //                 if (titles.size() == titleIDs.size()){
    //                     moveCursor(0, CONSOLE_HEIGHT-1);
    //                     std::cout << "                                         D-Pad: Change Page   -: Sort   +: Quit";
    //                     menuObject = new Menu_Stats(titles);
    //                 } else {
    //                     moveCursor(0, CONSOLE_HEIGHT-1);
    //                     std::cout << TEXT_RED << "Error: " << TEXT_RESET << "Unable to get play stats";
    //                     menu = M_Error;
    //                 }
    //                 break;
    //         }
    //     }

    //     // Draw menu
    //     menu = menuObject->draw(kDown);
    //     consoleUpdate(NULL);

    //     // Always exit regardless of menu
    //     if (kDown & KEY_PLUS){
    //         break;
    //     }
    // }

    // Cleanup resources
    // if (fsInit){
    //     fsExit();
    // }
    // if (nsInit){
    //     nsExit();
    // }
    // if (pdmInit){
    //     pdmqryExit();
    // }

    // for (size_t i = 0; i < titles.size(); i++){
    //     delete titles[i];
    // }

    return 0;
}