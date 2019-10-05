#include "menus/Error.h"
#include "menus/UserSelect.h"
#include "menus/Stats.h"
#include "Title.h"
#include "User.h"
#include "utils.h"

#include <algorithm>
#include <iostream>
#include <vector>

// Populate vector with Title objects created from NsApplicationRecord
void getUserPlayStats(u128 userID, std::vector<u64> titleIDs, std::vector<Title *> &titles){
    Result rc;

    // Empty vector if necessary
    while (titles.size() > 0){
        delete titles[0];
        titles.erase(titles.begin());
    }

    // Print status
    moveCursor(0, CONSOLE_HEIGHT-1);
    std::cout << "Reading play data...";
    consoleUpdate(NULL);

    // Create Titles for every titleID
    for (int i = 0; i < titleIDs.size(); i++){
        PdmPlayStatistics stats;
        rc = pdmqryQueryPlayStatisticsByApplicationIdAndUserAccountId(titleIDs[i], userID, &stats);
        if (R_FAILED(rc)){
            moveCursor(0, CONSOLE_HEIGHT-1);
            std::cout << TEXT_RED << "Error: " << TEXT_RESET << "pdmqryPSBAIAUAI() failed: " << rc;
        }
        if (R_SUCCEEDED(rc)){
            // Get application name
            NsApplicationControlData data;
            NacpLanguageEntry * lang = nullptr;

            rc = nsGetApplicationControlData(1, titleIDs[i], &data, sizeof(NsApplicationControlData), NULL);
            if (R_FAILED(rc)){
                moveCursor(0, CONSOLE_HEIGHT-1);
                std::cout << TEXT_RED << "Error: " << TEXT_RESET << "nsGetApplicationControlData() failed: " << rc;
            }
            if (R_SUCCEEDED(rc)){
                rc = nacpGetLanguageEntry(&data.nacp, &lang);
                if (R_FAILED(rc)){
                    moveCursor(0, CONSOLE_HEIGHT-1);
                    std::cout << TEXT_RED << "Error: " << TEXT_RESET << "nacpGetLanguageEntry() failed: " << rc;
                }
                if (R_SUCCEEDED(rc)){
                    titles.push_back(new Title(stats, lang->name));
                }
            }
        }
    }
    consoleUpdate(NULL);
}

int main(int argc, char * argv[]){
    // Status variables
    Result rc = 0;
    bool error = false;
    bool fsInit = false;
    bool nsInit = false;
    bool pdmInit = false;

    u128 userIDs[8];
    std::vector<User *> users;
    int selectedUser = 0;

    std::vector<Title *> titles;
    std::vector<u64> titleIDs;

    int lastMenu = M_Dummy;
    MenuType menu = M_Error;
    Menu * menuObject = new Menu_Error();

    // Initialise console
    consoleInit(NULL);
    std::cout << TEXT_WHITE << "NX-Activity-Log (v0.2.0)" << std::endl;
    for (size_t i = 0; i < CONSOLE_WIDTH; i++){
        std::cout << "_";
    }
    moveCursor(0, CONSOLE_HEIGHT-3);
    for (size_t i = 0; i < CONSOLE_WIDTH; i++){
        std::cout << "_";
    }
    consoleUpdate(NULL);

    // Get user account info
    moveCursor(0, CONSOLE_HEIGHT-1);
    std::cout << TEXT_RESET << "Reading user information...";
    consoleUpdate(NULL);

    rc = accountInitialize();
    if (R_FAILED(rc)){
        moveCursor(0, CONSOLE_HEIGHT-1);
        std::cout << TEXT_RED << "Error: " << TEXT_RESET << "accountInitialize() failed: " << rc;
        error = true;
    }
    if (R_SUCCEEDED(rc)){
        size_t num = 0;
        rc = accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);
        if (R_FAILED(rc)){
            moveCursor(0, CONSOLE_HEIGHT-1);
            std::cout << TEXT_RED << "Error: " << TEXT_RESET << "accountListAllUsers() failed: " << rc;
            error = true;
        }
        if (R_SUCCEEDED(rc)){
            // Populate users vector with User objects
            AccountProfile profile;
            AccountProfileBase profilebase;
            for (size_t i = 0; i < num; i++){
                rc = accountGetProfile(&profile, userIDs[i]);
                if (R_FAILED(rc)){
                    moveCursor(0, CONSOLE_HEIGHT-1);
                    std::cout << TEXT_RED << "Error: " << TEXT_RESET << "accountGetProfile() failed: " << rc;
                    error = true;
                }
                if (R_SUCCEEDED(rc)){
                    rc = accountProfileGet(&profile, NULL, &profilebase);
                    if (R_FAILED(rc)){
                        moveCursor(0, CONSOLE_HEIGHT-1);
                        std::cout << TEXT_RED << "Error: " << TEXT_RESET << "accountProfileGet() failed (iteration " << i << "): " << rc;
                        error = true;
                    }
                    if (R_SUCCEEDED(rc)){
                        users.push_back(new User(userIDs[i], profilebase.username));
                    }
                    accountProfileClose(&profile);
                }
            }
        }
        accountExit();
    }
    consoleUpdate(NULL);

    // Get titleIDs of played and installed titles
    if (!error){
        moveCursor(0, CONSOLE_HEIGHT-1);
        std::cout << "Getting title data...";
        consoleUpdate(NULL);

        rc = nsInitialize();
        if (R_FAILED(rc)){
            moveCursor(0, CONSOLE_HEIGHT-1);
            std::cout << TEXT_RED << "Error: " << TEXT_RESET << "nsInitialize() failed: " << rc;
            error = true;
        }
        if (R_SUCCEEDED(rc)){
            nsInit = true;
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
            rc = fsInitialize();
            if (R_FAILED(rc)){
                moveCursor(0, CONSOLE_HEIGHT-1);
                std::cout << TEXT_RED << "Error: " << TEXT_RESET << "fsInitialize() failed: " << rc;
                error = true;
            }
            if (R_SUCCEEDED(rc)){
                // Create save data iterator
                FsSaveDataIterator fsIterator;
                rc = fsOpenSaveDataIterator(&fsIterator, FsSaveDataSpaceId_NandUser);
                if (R_FAILED(rc)){
                    moveCursor(0, CONSOLE_HEIGHT-1);
                    std::cout << TEXT_RED << "Error: " << TEXT_RESET << "fsOpenSaveDataIterator() failed: " << rc;
                    error = true;
                }
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
                fsInit = true;
            }

            // Remove duplicate titleIDs
            std::sort(titleIDs.begin(), titleIDs.end());
            titleIDs.resize(std::distance(titleIDs.begin(), std::unique(titleIDs.begin(), titleIDs.end())));
        }

        consoleUpdate(NULL);
    }

    // Initialise pdm
    if (!error){
        rc = pdmqryInitialize();
        if (R_FAILED(rc)){
            moveCursor(0, CONSOLE_HEIGHT-1);
            std::cout << TEXT_RED << "Error: " << TEXT_RESET << "pdmqryInitialize() failed: " << rc;
            error = true;
        }
        if (R_SUCCEEDED(rc)){
            pdmInit = true;
        }
    }
    consoleUpdate(NULL);

    // No errors: present user selection
    if (!error){
        menu = M_UserSelect;
    }

    while (appletMainLoop()){
        // Get key presses
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        // Create/delete menu when changed
        if (lastMenu != menu){
            clearConsole();
            moveCursor(CONSOLE_WIDTH/2, 0);
            for (int i = CONSOLE_WIDTH/2; i <= CONSOLE_WIDTH; i++){
                std::cout << " ";
            }
            delete menuObject;
            lastMenu = menu;
            switch (menu){
                // Dummy menu (should never be called)
                case M_Dummy:
                    menu = M_Error;
                    break;
                // Error menu
                case M_Error:
                    menuObject = new Menu_Error();
                    break;
                // UserSelect menu
                case M_UserSelect:
                    moveCursor(0, CONSOLE_HEIGHT-1);
                    std::cout << "                                      D-Pad: Move Cursor    A: Select    +: Quit";
                    menuObject = new Menu_UserSelect(users, &selectedUser);
                    break;
                // Stats menu
                case M_Stats:
                    getUserPlayStats(userIDs[selectedUser], titleIDs, titles);
                    if (titles.size() == titleIDs.size()){
                        moveCursor(0, CONSOLE_HEIGHT-1);
                        std::cout << "                                D-Pad: Change Page   B: Back   -: Sort   +: Quit";
                        menuObject = new Menu_Stats(titles);
                    } else {
                        moveCursor(0, CONSOLE_HEIGHT-1);
                        std::cout << TEXT_RED << "Error: " << TEXT_RESET << "Unable to get play stats";
                        menu = M_Error;
                    }
                    break;
            }
        }

        // Draw menu
        menu = menuObject->draw(kDown);
        consoleUpdate(NULL);

        // Always exit regardless of menu
        if (kDown & KEY_PLUS){
            break;
        }
    }

    // Cleanup resources
    if (fsInit){
        fsExit();
    }
    if (nsInit){
        nsExit();
    }
    if (pdmInit){
        pdmqryExit();
    }
    consoleExit(NULL);

    for (size_t i = 0; i < users.size(); i++){
        delete users[i];
    }
    for (size_t i = 0; i < titles.size(); i++){
        delete titles[i];
    }

    return 0;
}