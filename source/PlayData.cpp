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

RecentPlayStatistics * PlayData::getRecentStatisticsForUser(u64 titleID, u64 start_ts, u64 end_ts, u128 userID) {
    // Get indexes of valid PlayEvents (should support out of order events?)
    std::vector<size_t> idxs;
    for (size_t i = 0; i < this->events.size(); i++) {
        if (this->events[i]->type == PlayEvent_Applet && this->events[i]->ID == titleID) {
            if (this->events[i]->clockTimestamp >= start_ts && this->events[i]->clockTimestamp <= end_ts) {
                idxs.push_back(i);
            }
        }
    }

    // Count playtime + launches
    RecentPlayStatistics * stats = new RecentPlayStatistics;
    stats->titleID = titleID;
    stats->playtime = 0;
    stats->launches = 0;
    stats->percentage = 42.69;

    // Was the last event InFocus?
    bool last_in = false;
    // Timestamp of last in_focus
    u64 last_ts = 0;

    for (size_t i = 0; i < idxs.size(); i++) {
        if (this->events[idxs[i]]->eventType == Applet_Launch) {
            stats->launches++;
        } else if (this->events[idxs[i]]->eventType == Applet_InFocus) {
            last_in = true;
            last_ts = this->events[idxs[i]]->steadyTimestamp;
        } else if (this->events[idxs[i]]->eventType == Applet_OutFocus) {
            if (last_in) {
                stats->playtime += (this->events[idxs[i]]->steadyTimestamp-last_ts)/60;
            }
            last_in = false;
        }
    }

    return stats;
}

PlayData::~PlayData() {
    while (this->events.size() > 0) {
        delete this->events[0];
        this->events.erase(this->events.begin());
    }
}