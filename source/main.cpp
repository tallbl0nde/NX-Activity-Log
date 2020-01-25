#include "Application.hpp"

int main(int argc, char * argv[]){
    // Status variables
    // bool is_mypage = false;
    // AccountUid mypage_id;

    // // Checking if launched as user page and if so get user id
    // AppletType t = appletGetAppletType();
    // if (t == AppletType_LibraryApplet) {
    //     // Attempt to get user id from IStorage
    //     AppletStorage * s = (AppletStorage *)malloc(sizeof(AppletStorage));
    //     // Pop common args IStorage
    //     if (R_SUCCEEDED(appletPopInData(s))) {
    //         // Pop MyPage-specific args IStorage
    //         if (R_SUCCEEDED(appletPopInData(s))) {
    //             // Get user id
    //             appletStorageRead(s, 0x8, &mypage_id, 0x10);

    //             // Check if valid
    //             AccountUid userIDs[ACC_USER_LIST_SIZE];
    //             s32 num = 0;
    //             accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);
    //             for (s32 i = 0; i < num; i++) {
    //                 if (mypage_id == userIDs[i]) {
    //                     is_mypage = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //     free(s);
    // }

    // Load config
    // Config * conf = Config::getConfig();
    // conf->readConfig();

    Main::Application * app = new Main::Application();
    app->run();
    delete app;

    return 0;
}