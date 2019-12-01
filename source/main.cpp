#include <algorithm>
#include "config.hpp"
#include "screenmanager.hpp"
#include "SDLHelper.hpp"
#include <string>

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
std::vector<User *> getUserObjects();
std::vector<Title *> getTitleObjects(u128);

int main(int argc, char * argv[]){
    // Status variables
    bool accInit, fsInit, nsInit, pdmInit, plInit, SDLInit, setInit = false;
    Result rc = 0;

    // Vector containing all user objects
    std::vector<User *> users;
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

    // Initialise and get ScreenManager
    ScreenManager * sm = ScreenManager::getInstance();

    // Only proceed if no errors
    if (accInit && fsInit && nsInit && pdmInit && plInit && SDLInit) {
        // Loading is kinda dodge
        bool error = false;

        // Stage 1: Get User IDs
        users = getUserObjects();
        if (users.size() == 0) {
            error = true;
            sm->setScreen(new Screen::Error("Unable to get information about users!"));
        }

        // Stage 2: Create UserSelect screen
        if (!error) {
            Screen::UserSelect * s = new Screen::UserSelect(users);

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

        // // Stage 2: Get installed titles and create Title objects
        // if (!error) {
        //     titles = getTitleObjects(userID);
        // }

        // if (!error) {
        //     sm->setScreen(new Screen::Activity(user, titles));
        // }

        // Clock to measure time between draw
        struct Clock clock;
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

    // Delete user objects
    while (users.size() > 0) {
        delete users[0];
        users.erase(users.begin());
    }

    // Delete any created titles
    while (titles.size() > 0) {
        delete titles[0];
        titles.erase(titles.begin());
    }

    return 0;
}

// Returns a vector with the IDs of all users on the system
std::vector<User *> getUserObjects() {
    // Get IDs
    std::vector<User *> users;
    u128 userIDs[ACC_USER_LIST_SIZE];
    size_t num = 0;
    Result rc = accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);
    if (R_SUCCEEDED(rc)) {
        // Create objects and insert into vector
        for (size_t i = 0; i < num; i++) {
            users.push_back(new User(userIDs[i]));
        }
    }

    // Return empty vector if an error occurred
    return users;
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