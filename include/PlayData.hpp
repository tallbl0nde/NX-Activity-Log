#ifndef PLAYDATA_HPP
#define PLAYDATA_HPP

#include <switch.h>
#include <vector>

// Enumeration for event type in PlayEvent struct
enum PlayEventType {
    PlayEvent_Applet,       // PlayEvent contains applet event
    PlayEvent_Account       // PlayEvent contains account event
};

// Enumeration for applet/account event type in PlayEvent struct
enum EventType {
    Applet_Launch,          // Applet launched
    Applet_Exit,            // Applet quit
    Applet_InFocus,         // Applet gained focus
    Applet_OutFocus,        // Applet lost focus
    Account_Active,         // Account selected
    Account_Inactive        // Account closed?
};

// PlayEvents are parsed PdmPlayEvents containing only necessary information
struct PlayEvent {
    PlayEventType type;     // Type of PlayEvent (important for ID + type)
    u128 ID;                // UserID / TitleID
    EventType eventType;    // See EventType enum
    u64 clockTimestamp;     // Time of event
    u64 steadyTimestamp;    // Steady timestamp (used for calculating duration)
};

// RecentPlayStatistics struct is similar to PdmPlayStatistics but
// only contains recent values
struct RecentPlayStatistics {
    u64 titleID;            // TitleID of these statistics
    u32 playtime;           // Total playtime in minutes
    u32 launches;           // Total launches
    float percentage;       // Percentage of overall play time
};

// PlayData stores PlayEvents which are created from processing PlayEvent.dat using pdm.
// The data can then be queried across a period of time, with the summation of these
// statistics being returned.
class PlayData {
    private:
        // Vector containing created PlayEvents
        // Should be in chronological order (ie. as read from pdm)
        std::vector<PlayEvent *> events;

    public:
        // The constructor creates PlayEvents
        PlayData();

        // Returns a RecentPlayStatistics for the given time range and user ID
        RecentPlayStatistics * getRecentStatisticsForUser(u64, u64, u64, u128);

        // The destructor deletes PlayEvents (frees memory)
        ~PlayData();
};

#endif