#include <SDL2/SDL_image.h>
#include "Title.hpp"
#include <time.h>

Title::Title(u64 titleID, u128 userID, SDL_Renderer * renderer) {
    this->titleID = titleID;

    // Defaults in case fetch fails
    this->first_timestamp = 0;
    this->icon = nullptr;
    this->last_timestamp = 0;
    this->name = "";
    this->playtime = 0;
    this->launches = 0;

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
    rc = nsGetApplicationControlData(1, this->titleID, &data, sizeof(NsApplicationControlData), &nacp_size);
    if (R_SUCCEEDED(rc)){
        // Get name
        rc = nacpGetLanguageEntry(&data.nacp, &lang);
        if (R_SUCCEEDED(rc)){
            this->name = std::string(lang->name);
        }

        // Get icon
        size_t icon_size = nacp_size - sizeof(data.nacp);
        // Create texture
        SDL_Surface * tmp = IMG_Load_RW(SDL_RWFromMem(data.icon, icon_size), 1);
        this->icon = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_FreeSurface(tmp);
    }
}

u64 Title::getTitleID() {
    return this->titleID;
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

SDL_Texture * Title::getIcon() {
    return this->icon;
}

Title::~Title() {
    // In case there's no icon for whatever reason
    if (this->icon != nullptr) {
        SDL_DestroyTexture(this->icon);
    }
}