#include "utils/NX.hpp"

// Maximum number of titles to read using pdm
#define MAX_TITLES 2000

// Comparison of AccountUids
bool operator == (const AccountUid &a, const AccountUid &b) {
    if (a.uid[0] == b.uid[0] && a.uid[1] == b.uid[1]) {
        return true;
    }
    return false;
}

namespace Utils::NX {
    ThemeType getHorizonTheme() {
        ColorSetId thm;
        Result rc = setsysGetColorSetId(&thm);
        if (R_SUCCEEDED(rc)) {
            switch (thm) {
                case ColorSetId_Light:
                    return ThemeType::Light;
                    break;

                case ColorSetId_Dark:
                    return ThemeType::Dark;
                    break;
            }
        }

        // If it fails return dark
        return ThemeType::Dark;
    }

    Language getSystemLanguage() {
        SetLanguage sl;
        u64 l;
        setInitialize();
        setGetSystemLanguage(&l);
        setMakeLanguage(l, &sl);
        setExit();

        Language lang;
        switch (sl) {
            case SetLanguage_ENGB:
            case SetLanguage_ENUS:
                lang = English;
                break;

            case SetLanguage_FR:
                lang = French;
                break;

            case SetLanguage_DE:
                lang = German;
                break;

            case SetLanguage_IT:
                lang = Italian;
                break;

            case SetLanguage_PT:
                lang = Portugese;
                break;

            case SetLanguage_RU:
                lang = Russian;
                break;

            case SetLanguage_ES:
                lang = Spanish;
                break;

            case SetLanguage_ZHHANT:
                lang = ChineseTraditional;
                break;

            case SetLanguage_ZHCN:
            case SetLanguage_ZHHANS:
                lang = Chinese;
                break;

            case SetLanguage_KO:
                lang = Korean;
                break;

            default:
                lang = Default;
                break;
        }

        return lang;
    }

    ::NX::User * getUserPageUser() {
        ::NX::User * u = nullptr;

        AppletType t = appletGetAppletType();
        if (t == AppletType_LibraryApplet) {
            // Attempt to get user id from IStorage
            AppletStorage * s = (AppletStorage *)malloc(sizeof(AppletStorage));
            // Pop common args IStorage
            if (R_SUCCEEDED(appletPopInData(s))) {
                // Pop MyPage-specific args IStorage
                if (R_SUCCEEDED(appletPopInData(s))) {
                    // Get user id
                    AccountUid uid;
                    appletStorageRead(s, 0x8, &uid, 0x10);

                    // Check if valid
                    AccountUid userIDs[ACC_USER_LIST_SIZE];
                    s32 num = 0;
                    accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);
                    for (s32 i = 0; i < num; i++) {
                        if (uid == userIDs[i]) {
                            u = new ::NX::User(uid);
                            break;
                        }
                    }
                }
            }
            free(s);
        }

        return u;
    }

    std::vector<::NX::User *> getUserObjects() {
        // Get IDs
        std::vector<::NX::User *> users;
        AccountUid userIDs[ACC_USER_LIST_SIZE];
        s32 num = 0;
        Result rc = accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);

        if (R_SUCCEEDED(rc)) {
            // Create objects and insert into vector
            for (s32 i = 0; i < num; i++) {
                users.push_back(new ::NX::User(userIDs[i]));
            }
        }

        // Returns an empty vector if an error occurred
        return users;
    }

    std::vector<::NX::Title *> getTitleObjects(std::vector<::NX::User *> u) {
        Result rc;

        // Get ALL played titles for ALL users
        // (this doesn't include installed games that haven't been played)
        std::vector<TitleID> playedIDs;
        for (unsigned short i = 0; i < u.size(); i++) {
            s32 playedTotal = 0;
            TitleID tmpID = 0;
            PdmAccountPlayEvent *events = new PdmAccountPlayEvent[MAX_TITLES];;
            rc = pdmqryQueryAccountPlayEvent(0, u[i]->ID(), events, MAX_TITLES, &playedTotal);
            if (R_FAILED(rc) || playedTotal == 0) {
                continue;
            }

            // Push back ID if not already in the vector
            for (s32 j = 0; j < playedTotal; j++) {
                bool found = false;
                tmpID = (static_cast<TitleID>(events[j].application_id[0]) << 32) | events[j].application_id[1];
                for (size_t k = 0; k < playedIDs.size(); k++) {
                    if (playedIDs[k] == tmpID) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    playedIDs.push_back(tmpID);
                }
            }
            delete[] events;
        }

        // Get IDs of all installed titles
        std::vector<TitleID> installedIDs;
        NsApplicationRecord * records = new NsApplicationRecord[MAX_TITLES];
        s32 count = 0;
        s32 out = 0;
        while (true) {
            rc = nsListApplicationRecord(records, MAX_TITLES, count, &out);
            // Break if at the end or no titles
            if (R_FAILED(rc) || out == 0){
                break;
            }
            for (s32 i = 0; i < out; i++) {
                installedIDs.push_back((records + i)->application_id);
            }
            count += out;
        }
        delete[] records;

        // Create Title objects from IDs
        std::vector<::NX::Title *> titles;
        for (size_t i = 0; i < playedIDs.size(); i++) {
            // Loop over installed titles to determine if installed or not
            bool installed = false;
            for (size_t j = 0; j < installedIDs.size(); j++) {
                if (installedIDs[j] == playedIDs[i]) {
                    installed = true;
                    break;
                }
            }

            titles.push_back(new ::NX::Title(playedIDs[i], installed));
        }

        return titles;
    }

    void startServices() {
        accountInitialize(AccountServiceType_System);
        nsInitialize();
        pdmqryInitialize();
        romfsInit();
        setsysInitialize();
        socketInitializeDefault();

        #if _NXLINK_
            nxlinkStdio();
        #endif
    }

    void stopServices() {
        accountExit();
        nsExit();
        pdmqryExit();
        romfsExit();
        setsysExit();
        socketExit();
    }
};
