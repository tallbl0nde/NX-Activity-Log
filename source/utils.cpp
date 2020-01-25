#include <fstream>
#include "Utils.hpp"

// Comparison of AccountUids
bool operator == (const AccountUid &a, const AccountUid &b) {
    if (a.uid[0] == b.uid[0] && a.uid[1] == b.uid[1]) {
        return true;
    }
    return false;
}

namespace Utils {
    void copyFile(std::string src, std::string dest) {
        std::ifstream srcF(src, std::ios::binary);
        std::ofstream destF(dest, std::ios::binary);

        destF << srcF.rdbuf();

        srcF.close();
        destF.flush();
        destF.close();
    }

    // Add commas to a number (only does one but shhh)
    std::string formatNumberComma(u32 number) {
        std::string s = std::to_string(number);
        if (s.length() > 3) {
            return s.substr(0, s.length() - 3) + "," + s.substr(s.length() - 3, 3);
        }
        return s;
    }

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

        // Returns an empty vector if an error occurred
        return users;
    }

    void startServices() {
        accountInitialize(AccountServiceType_System);
        nsInitialize();
        pdmqryInitialize();
        romfsInit();
        setsysInitialize();
    }

    void stopServices() {
        accountExit();
        nsExit();
        pdmqryExit();
        romfsExit();
        setsysExit();
    }
};