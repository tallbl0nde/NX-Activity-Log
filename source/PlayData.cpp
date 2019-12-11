#include <algorithm>
#include "PlayData.hpp"

// Maximum number of entries to process in one iteration
#define MAX_PROCESS_ENTRIES 1000

PlayData::PlayData() {
    // Position of first event to read
    u32 offset = 0;
    // Total events read in iteration
    s32 total_read = 1;

    // Array to store read events
    PdmPlayEvent * pEvents = new PdmPlayEvent[MAX_PROCESS_ENTRIES];

    // Read all events
    while (total_read > 0) {
        Result rc = pdmqryQueryPlayEvent(offset, pEvents, MAX_PROCESS_ENTRIES, &total_read);
        if (R_SUCCEEDED(rc)) {
            // Set next read position to next event
            offset += total_read;

            // Process read events
            for (s32 i = 0; i < total_read; i++) {
                PlayEvent * event;

                // Populate PlayEvent based on event type
                switch (pEvents[i].playEventType) {
                    case PdmPlayEventType_Account:
                        // Ignore this event if type is 2
                        if (pEvents[i].eventData.account.type == 2) {
                            continue;
                        }
                        event = new PlayEvent;
                        event->type = PlayEvent_Account;

                        // UserID words are wrong way around (why Nintendo?)
                        event->ID = pEvents[i].eventData.account.userID[2];
                        event->ID = (event->ID << 32) | pEvents[i].eventData.account.userID[3];
                        event->ID = (event->ID << 32) | pEvents[i].eventData.account.userID[0];
                        event->ID = (event->ID << 32) | pEvents[i].eventData.account.userID[1];

                        // Set account event type
                        switch (pEvents[i].eventData.account.type) {
                            case 0:
                                event->eventType = Account_Active;
                                break;
                            case 1:
                                event->eventType = Account_Inactive;
                                break;
                        }
                        break;

                    case PdmPlayEventType_Applet:
                        // Ignore this event based on log policy
                        if (pEvents[i].eventData.applet.logPolicy != PdmPlayLogPolicy_All) {
                            continue;
                        }
                        event = new PlayEvent;
                        event->type = PlayEvent_Applet;

                        // Join two halves of title ID
                        event->ID = pEvents[i].eventData.applet.titleID[0];
                        event->ID = (event->ID << 32) | pEvents[i].eventData.applet.titleID[1];

                        // Set applet event type
                        switch (pEvents[i].eventData.applet.eventType) {
                            case PdmAppletEventType_Launch:
                                event->eventType = Applet_Launch;
                                break;
                            case PdmAppletEventType_Exit:
                            case PdmAppletEventType_Exit5:
                            case PdmAppletEventType_Exit6:
                                event->eventType = Applet_Exit;
                                break;
                            case PdmAppletEventType_InFocus:
                                event->eventType = Applet_InFocus;
                                break;
                            case PdmAppletEventType_OutOfFocus:
                            case PdmAppletEventType_OutOfFocus4:
                                event->eventType = Applet_OutFocus;
                                break;
                        }
                        break;

                    // Do nothing for other event types
                    default:
                        continue;
                        break;
                }

                // Set timestamps
                event->clockTimestamp = pEvents[i].timestampUser;
                event->steadyTimestamp = pEvents[i].timestampSteady;

                // Add PlayEvent to vector
                this->events.push_back(event);
            }
        }
    }

    // Free memory allocated to array
    delete[] pEvents;
}

std::vector<u64> PlayData::getLoggedTitleIDs() {
    std::vector<u64> ids;
    for (size_t i = 0; i < this->events.size(); i++) {
        if (this->events[i]->type == PlayEvent_Applet) {
            // Exclude this title (I dunno what it is but it causes crashes)
            if (this->events[i]->ID == 0x0100000000001012) {
                continue;
            }

            if (std::find(ids.begin(), ids.end(), (u64)this->events[i]->ID) == ids.end()) {
                ids.push_back((u64)this->events[i]->ID);
            }
        }
    }

    return ids;
}

struct PlaySession {
    size_t index;
    size_t num;
};

RecentPlayStatistics * PlayData::getRecentStatisticsForUser(u64 titleID, u64 start_ts, u64 end_ts, u128 userID) {
    // Break each "session" apart and keep if matching titleID and userID
    std::vector<PlaySession> sessions;
    size_t a = 0;
    while (a < this->events.size()) {
        if (this->events[a]->eventType == Applet_Launch) {
            // Find end of session (or start of next session if switch crashed before exit)
            size_t s = a;
            bool time_c = false;
            bool titleID_c = false;
            bool userID_c = false;
            if (this->events[a]->ID == titleID) {
                titleID_c = true;
            }

            a++;
            bool end = false;
            while (a < this->events.size() && !end) {
                // Check if every event is in the required range
                if (this->events[a]->clockTimestamp >= start_ts && this->events[a]->clockTimestamp <= end_ts) {
                    time_c = true;
                }

                switch (this->events[a]->eventType) {
                    // Check userID whenever account event encountered
                    case Account_Active:
                    case Account_Inactive:
                        if (this->events[a]->ID == userID) {
                            userID_c = true;
                        }
                        break;

                    // Exit indicates end of session
                    case Applet_Exit:
                        if (time_c && titleID_c && userID_c) {
                            struct PlaySession st;
                            st.index = s;
                            st.num = a - s + 1;
                            sessions.push_back(st);
                        }
                        end = true;
                        break;

                    // Encountering another launch also indicates end of session (due to crash)
                    case Applet_Launch:
                        if (time_c && titleID_c && userID_c) {
                            struct PlaySession st;
                            st.index = s;
                            st.num = a - s;
                            sessions.push_back(st);
                        }
                        end = true;
                        a--;
                        break;

                    default:
                        break;
                }
                a++;
            }
        } else {
            a++;
        }
    }

    // Count playtime + launches
    RecentPlayStatistics * stats = new RecentPlayStatistics;
    stats->titleID = titleID;
    stats->playtime = 0;
    stats->launches = 0;

    if (sessions.size() == 0) {
        return stats;
    }

    // Iterate over valid sessions to calculate statistics
    for (size_t i = 0; i < sessions.size(); i++) {
        // A "valid" session must have at least 6 events (launch, in, login, out, logout, exit)
        if (sessions[i].num >= 6) {
            u64 last_ts = 0;
            for (size_t j = sessions[i].index; j < sessions[i].index + sessions[i].num; j++) {
                switch (this->events[j]->eventType) {
                    case Applet_Launch:
                    case Applet_Exit:
                    case Account_Active:
                    case Account_Inactive:
                        break;

                    case Applet_InFocus:
                        last_ts = this->events[j]->steadyTimestamp;
                        break;

                    case Applet_OutFocus:
                        // Move to last out focus (I don't know why the log has multiple)
                        while (j+1 < this->events.size()) {
                            if (this->events[j+1]->eventType == Applet_OutFocus) {
                                j++;
                            } else {
                                break;
                            }
                        }
                        stats->playtime += (this->events[j]->steadyTimestamp - last_ts);
                        break;
                }
            }
        }

        // If not "valid" just add launch
        stats->launches++;
    }

    return stats;
}

PlayData::~PlayData() {
    while (this->events.size() > 0) {
        delete this->events[0];
        this->events.erase(this->events.begin());
    }
}