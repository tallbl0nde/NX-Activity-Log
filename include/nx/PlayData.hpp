#ifndef PLAYDATA_HPP
#define PLAYDATA_HPP

#include "Types.hpp"
#include <vector>

namespace NX {
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
        PlayEventType type;     // Type of PlayEvent
        AccountUid userID;      // UserID
        TitleID titleID;        // TitleID
        EventType eventType;    // See EventType enum
        u64 clockTimestamp;     // Time of event
        u64 steadyTimestamp;    // Steady timestamp (used for calculating duration)
    };

    // A PlaySession represents a session of play for a game. It contains the start
    // and end timestamps and playtime for convenience. Note that (end-start) != playtime
    // due to time when the game may not have been focussed.
    struct PlaySession {
        u32 playtime;           // Total playtime in seconds
        u64 startTimestamp;     // Time of launch
        u64 endTimestamp;       // Time of exit
    };

    // PdmPlayStatistics but only the necessary things
    struct PlayStatistics {
        TitleID titleID;        // TitleID of these stats
        u32 firstPlayed;        // Timestamp of first launch
        u32 lastPlayed;         // Timestamp of last play (exit)
        u32 playtime;           // Total playtime in seconds
        u32 launches;           // Total launches
    };

    // RecentPlayStatistics struct is similar to PdmPlayStatistics but
    // only contains recent values
    struct RecentPlayStatistics {
        TitleID titleID;        // TitleID of these statistics
        u32 playtime;           // Total playtime in seconds
        u32 launches;           // Total launches
    };

    // Struct used for analyzing/splitting up play sessions
    struct PD_Session {
        size_t index;   // Index of first (launch) event
        size_t num;     // Number of events for this session
    };

    // PlayData stores PlayEvents which are created from processing PlayEvent.dat using pdm.
    // The data can then be queried across a period of time, with the summation of these
    // statistics being returned. It also offers wrappers for some pdm functions.
    class PlayData {
        private:
            // Vector containing created PlayEvents
            // Should be in chronological order (ie. as read from pdm)
            std::vector<PlayEvent *> events;

            // Return vector of PD_Sessions for given title/user IDs + time range
            // Give a titleID of zero to include all titles
            std::vector<PD_Session> getPDSessions(TitleID, AccountUid, u64, u64);

            // Counts playtime and launches for the given sessions
            // TitleID in returned struct is zero
            RecentPlayStatistics * countPlaytime(std::vector<PD_Session>, u64, u64);

        public:
            // The constructor prepares + creates PlayEvents
            PlayData();

            // Returns all titleIDs found within the play log (some may no longer be valid?)
            std::vector<TitleID> getLoggedTitleIDs();

            // Returns vector containing PlayEvents between the given times
            // Start time, end time, titleID, userID
            std::vector<PlayEvent> getPlayEvents(u64, u64, TitleID, AccountUid);

            // Returns all play sessions for the given title ID and user ID
            std::vector<PlaySession> getPlaySessionsForUser(TitleID, AccountUid);

            // Returns a RecentPlayStatistics for the given time range for all users
            RecentPlayStatistics * getRecentStatisticsForUser(u64, u64, AccountUid);

            // Returns a RecentPlayStatistics for the given time range and user ID
            RecentPlayStatistics * getRecentStatisticsForTitleAndUser(TitleID, u64, u64, AccountUid);

            // Returns a PlayStatistics for the given titleID and userID
            PlayStatistics * getStatisticsForUser(TitleID, AccountUid);

            // The destructor deletes PlayEvents (frees memory)
            ~PlayData();
    };
};

#endif