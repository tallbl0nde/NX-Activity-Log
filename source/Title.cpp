#include "Title.hpp"
#include <time.h>

Title::Title(u64 titleID, AccountUid userID, bool installed) {
    this->titleID = titleID;
    this->is_installed = installed;

    // Defaults in case fetch fails
    this->first_timestamp = 0;
    this->last_timestamp = 0;
    this->name = "";
    this->playtime = 0;
    this->launches = 0;
    this->ptr = nullptr;

    // Get play statistics
    PdmPlayStatistics stats;
    Result rc = pdmqryQueryPlayStatisticsByApplicationIdAndUserAccountId(this->titleID, userID, &stats);
    if (R_SUCCEEDED(rc)){
        this->first_timestamp = stats.first_timestampUser;
        this->last_timestamp = stats.last_timestampUser;
        this->playtime = stats.playtimeMinutes;
        this->launches = stats.totalLaunches;
    }

    // Get name and icon
    NsApplicationControlData data;
    NacpLanguageEntry * lang = nullptr;
    size_t nacp_size;
    rc = nsGetApplicationControlData(NsApplicationControlSource_Storage, this->titleID, &data, sizeof(NsApplicationControlData), &nacp_size);
    if (R_SUCCEEDED(rc)){
        // Get name
        rc = nacpGetLanguageEntry(&data.nacp, &lang);
        if (R_SUCCEEDED(rc)){
            this->name = std::string(lang->name);
        }

        // Get icon
        this->ptr = data.icon;
        this->size = nacp_size - sizeof(data.nacp);
    }
}

u64 Title::getTitleID() {
    return this->titleID;
}

bool Title::getInstalled() {
    return this->is_installed;
}

std::string Title::getName() {
    return this->name;
}

u32 Title::getFirstPlayed() {
    return this->first_timestamp;
}

u32 Title::getLastPlayed() {
    return this->last_timestamp;
}

u32 Title::getPlaytime() {
    return this->playtime;
}

u32 Title::getLaunches() {
    return this->launches;
}

u8 * Title::getPtr() {
    return this->ptr;
}

u32 Title::getSize() {
    return this->size;
}

Title::~Title() {
    // In case there's no icon for whatever reason
    if (this->ptr != nullptr) {
        free(this->ptr);
    }
}