#ifndef TITLE_H
#define TITLE_H

#include <string>
#include <switch.h>

class Title{
    private:
        static int num;
        int ID;
        u64 titleID;
        std::string name;
        u32 firstTimestamp;
        u32 lastTimestamp;
        u32 playtime;
        u32 launches;
    public:
        Title(PdmPlayStatistics, char *);

        // These functions return Title's data
        int getID();
        u64 getTitleID();
        std::string getName();
        u32 getFirstPlayed();
        u32 getLastPlayed();
        u32 getPlaytime();
        u32 getLaunches();
};

#endif