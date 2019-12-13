#include "config.hpp"
#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include <string>
#include "utils.hpp"

// Struct representing a "clock", which stores time between ticks
struct Clock {
    uint32_t last_tick = 0;
    uint32_t delta = 0;

    void tick() {
        uint32_t tick = SDL_GetTicks();
        delta = tick - last_tick;
        last_tick = tick;
    }
};

// Forward declarations of functions because I like them at the end :)
std::vector<User *> getUserObjects();;

int main(int argc, char * argv[]){
    // Status variables
    bool accInit = false, fsInit = false, nsInit = false, pdmInit = false, plInit = false, romFsInit = false, SDLInit = false, setInit = false;
    bool is_mypage = false;
    AccountUid mypage_id;
    Result rc = 0;

    // Vector containing all user objects
    std::vector<User *> users;

    // Start required services
    rc = accountInitialize(AccountServiceType_System);
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
    rc = romfsInit();
    if (R_SUCCEEDED(rc)) {
        romFsInit = true;
    }
    rc = setsysInitialize();
    if (R_SUCCEEDED(rc)) {
        setInit = true;
    }

    // Checking if launched as user page and if so get user id
    AppletType t = appletGetAppletType();
    if (t == AppletType_LibraryApplet) {
        // Attempt to get user id from IStorage
        AppletStorage * s = (AppletStorage *)malloc(sizeof(AppletStorage));
        // Pop common args IStorage
        if (R_SUCCEEDED(appletPopInData(s))) {
            // Pop MyPage-specific args IStorage
            if (R_SUCCEEDED(appletPopInData(s))) {
                // Get user id
                appletStorageRead(s, 0x8, &mypage_id, 0x10);

                // Check if valid
                AccountUid userIDs[ACC_USER_LIST_SIZE];
                s32 num = 0;
                accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);
                for (s32 i = 0; i < num; i++) {
                    if (mypage_id == userIDs[i]) {
                        is_mypage = true;
                        break;
                    }
                }
            }
        }
        free(s);
    }

    // Initialize SDL
    SDLInit = SDLHelper::initSDL();

    // Load config
    Config * conf = Config::getConfig();
    conf->readConfig();

    // Initialise and get ScreenManager
    ScreenManager * sm = ScreenManager::getInstance();

    // Only proceed if no errors
    if (accInit && fsInit && nsInit && pdmInit && plInit && romFsInit && SDLInit) {
        // Loading is kinda dodge
        bool error = false;

        // Stage 1: Get User ID(s)
        if (is_mypage) {
            users.push_back(new User(mypage_id));
        } else {
            users = getUserObjects();
            if (users.size() == 0) {
                error = true;
                sm->setScreen(new Screen::Error("Unable to get information about users!"));
            }
        }

        // Stage 2: Create UserSelect screen
        if (!error) {
            Screen::UserSelect * s = new Screen::UserSelect(users, is_mypage);

            // Set theme
            switch (conf->getGeneralTheme()) {
                case T_Auto: {
                    ColorSetId thm;
                    rc = setsysGetColorSetId(&thm);
                    s->setTheme((bool)thm);
                    break;
                }

                case T_Light: {
                    s->setTheme(false);
                    break;
                }

                case T_Dark: {
                    s->setTheme(true);
                    break;
                }
            }
            sm->setScreen(s);
        }

        // Clock to measure time between draw
        struct Clock clock;
        // Infinite loop until exit (unless user page, in which can't be exited)
        while (sm->loop()) {
            // Check for events
            sm->event();

            // Get time since last draw and update
            clock.tick();
            sm->update(clock.delta);

            // Render screen
            sm->draw();

            // Draw FPS
            // std::string ss = "FPS: " + std::to_string((int)(1.0/(clock.delta/1000.0))) + " (" + std::to_string(clock.delta) + " ms)";
            // SDLHelper::drawText(ss.c_str(), SDL_Color{0, 150, 150, 255}, 5, 695, 20);

            SDLHelper::draw();
        }
    }

    // Free screen memory
    sm->free();

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
    if (romFsInit) {
        romfsExit();
    }
    if (setInit) {
        setExit();
    }

    // Delete user objects
    while (users.size() > 0) {
        delete users[0];
        users.erase(users.begin());
    }

    return 0;
}

// Returns a vector with the IDs of all users on the system
std::vector<User *> getUserObjects() {
    // Get IDs
    std::vector<User *> users;
    AccountUid userIDs[ACC_USER_LIST_SIZE];
    s32 num = 0;
    Result rc = accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);
    if (R_SUCCEEDED(rc)) {
        // Create objects and insert into vector
        for (s32 i = 0; i < num; i++) {
            users.push_back(new User(userIDs[i]));
        }
    }

    // Return empty vector if an error occurred
    return users;
}