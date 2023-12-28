#include "utils/NX.hpp"
#include <algorithm>
#include <iterator>

// Maximum number of titles to read using pdm
#define MAX_TITLES_PER_TIME 100

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
        for (auto user : u) {
            TitleID tmpID = 0;
            s32 startEntryIndex = -1;
            s32 endEntryIndex = -1;
            s32 totalEntries = -1;
            rc = pdmqryGetAvailableAccountPlayEventRange(user->ID(), &totalEntries, &startEntryIndex, &endEntryIndex);
            if (R_FAILED(rc) || !totalEntries)
                continue;

            s32 count = totalEntries;
            s32 offset = startEntryIndex;
            while (count) {
                s32 total_read = -1;
                PdmAccountPlayEvent *userPlayEvents = new PdmAccountPlayEvent[MAX_TITLES_PER_TIME];
                rc = pdmqryQueryAccountPlayEvent(offset, user->ID(), userPlayEvents, MAX_TITLES_PER_TIME, &total_read);
                if (R_SUCCEEDED(rc)) {
                    offset += total_read;
                    count -= total_read;
                    for (s32 j = 0; j < total_read; j++) {
                        tmpID = (static_cast<TitleID>(userPlayEvents[j].application_id[0]) << 32) | userPlayEvents[j].application_id[1];
                        if(std::find_if(playedIDs.begin(), playedIDs.end(), [tmpID](auto id){ return id == tmpID;}) == playedIDs.end()) {
                            if (tmpID != 0) {
                                playedIDs.push_back(tmpID);
                            }
                        }
                    }
                }
                delete[] userPlayEvents;
            }
        }

        std::vector<TitleID> installedIDs;
        // Get IDs of all installed titles
        s32 start_offset = 0;
        s32 out = 0;
        while (true) {
            NsApplicationRecord *records = new NsApplicationRecord[MAX_TITLES_PER_TIME];
            rc = nsListApplicationRecord(records, MAX_TITLES_PER_TIME, start_offset, &out);
            if (R_FAILED(rc) || out == 0) {
                delete[] records;
                break;
            }
            for (s32 i = 0; i < out; i++) {
                installedIDs.push_back((records + i)->application_id);
            }
            start_offset += out;
            delete[] records;
        }

        // Create Title objects from IDs
        std::vector<::NX::Title *> titles;
        for (auto playedID : playedIDs) {
            bool installed = std::find_if(installedIDs.begin(), installedIDs.end(), [playedID](auto id){ return id == playedID;}) != installedIDs.end();
            titles.push_back(new ::NX::Title(playedID, installed));
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
