#include "Title.h"
#include <time.h>

int Title::num = 0;

Title::Title(PdmPlayStatistics stats, char * name){
    ID = num;
    num++;
    titleID = stats.titleID;

    // Copy the name string
    std::string s(name);
    this->name = s;

    firstTimestamp = stats.first_timestampUser;
    lastTimestamp = stats.last_timestampUser;
    playtime = stats.playtimeMinutes;
    launches = stats.totalLaunches;
}

int Title::getID(){
    return ID;
}

u64 Title::getTitleID(){
    return titleID;
}

std::string Title::getName(){
    return name;
}

u32 Title::getFirstPlayed(){
    return firstTimestamp;
}

u32 Title::getLastPlayed(){
    return lastTimestamp;
}

u32 Title::getPlaytime(){
    return playtime;
}

u32 Title::getLaunches(){
    return launches;
}
