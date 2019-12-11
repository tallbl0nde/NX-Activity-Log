#ifndef TITLE_HPP
#define TITLE_HPP

#include <SDL2/SDL.h>
#include <string>
#include <switch.h>

// A Title stores the name, image texture and play statistics for
// a game/title. All information is fetched using the titleID passed
class Title {
    private:
        // Game's titleID
        u64 titleID;
        // Is the game installed?
        bool is_installed;
        // Game's name
        std::string name;
        // POSIX timestamps of first and last play time
        u32 first_timestamp;
        u32 last_timestamp;
        // Total playtime in minutes
        u32 playtime;
        // Number of launches
        u32 launches;
        // SDL Texture containing game icon
        SDL_Texture * icon;

    public:
        // The constructor derives all relevant info from given titleID
        Title(u64, AccountUid, bool);

        // These functions return Title's data
        u64 getTitleID();
        bool getInstalled();
        std::string getName();
        u32 getFirstPlayed();
        u32 getLastPlayed();
        u32 getPlaytime();
        u32 getLaunches();
        SDL_Texture * getIcon();

        // Destructor frees memory used by image
        ~Title();
};

#endif