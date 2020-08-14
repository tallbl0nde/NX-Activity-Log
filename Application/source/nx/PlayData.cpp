#include <algorithm>
#include "nx/PlayData.hpp"
#include "utils/NX.hpp"
#include "utils/Time.hpp"

// Maximum number of entries to process in one iteration
#define MAX_PROCESS_ENTRIES 1000

namespace NX {
    std::vector<PD_Session> PlayData::getPDSessions(TitleID titleID, AccountUid userID, u64 start_ts, u64 end_ts) {
        // Break each "session" apart and keep if matching titleID and userID
        std::vector<PD_Session> sessions;
        size_t a = 0;
        while (a < this->events.size()) {
            if (this->events[a]->eventType == Applet_Launch) {
                // Find end of session (or start of next session if switch crashed before exit)
                size_t s = a;
                bool time_c = false;
                bool titleID_c = false;
                if (titleID == 0 || this->events[a]->titleID == titleID) {
                    titleID_c = true;
                }
                bool userID_c = false;

                a++;
                bool end = false;
                while (a < this->events.size() && !end) {
                    // Check if every event is in the required range
                    if (this->events[a]->clockTimestamp >= start_ts && this->events[a]->clockTimestamp <= end_ts) {
                        time_c = true;
                    }
                    // Also check if there is an event prior to event and after
                    if (a > s) {
                        if (this->events[a]->clockTimestamp > end_ts && this->events[a-1]->clockTimestamp < start_ts) {
                            time_c = true;
                        }
                    }

                    switch (this->events[a]->eventType) {
                        // Check userID whenever account event encountered
                        case Account_Active:
                        case Account_Inactive:
                            if (this->events[a]->userID == userID) {
                                userID_c = true;
                            }
                            break;

                        // Exit indicates end of session
                        case Applet_Exit:
                            if (time_c && titleID_c && userID_c) {
                                struct PD_Session st;
                                st.index = s;
                                st.num = a - s + 1;
                                sessions.push_back(st);
                            }
                            end = true;
                            break;

                        // Encountering another launch also indicates end of session (due to crash)
                        case Applet_Launch:
                            if (time_c && titleID_c && userID_c) {
                                struct PD_Session st;
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

        return sessions;
    }

    RecentPlayStatistics * PlayData::countPlaytime(std::vector<PD_Session> sessions, u64 start_ts, u64 end_ts) {
        // Count playtime + launches
        RecentPlayStatistics * stats = new RecentPlayStatistics;
        stats->titleID = 0;
        stats->playtime = 0;
        stats->launches = 0;

        if (sessions.size() == 0) {
            return stats;
        }

        // Iterate over valid sessions to calculate statistics
        for (size_t i = 0; i < sessions.size(); i++) {
            stats->launches++;

            u64 last_ts = 0;
            u64 last_clock = 0;
            bool in_before = false;
            bool done = false;
            for (size_t j = sessions[i].index; j < sessions[i].index + sessions[i].num; j++) {
                if (done) {
                    break;
                }

                switch (this->events[j]->eventType) {
                    case Applet_Launch:
                        // Skip to first in focus event
                        while (j+1 < this->events.size()) {
                            if (this->events[j+1]->eventType != Applet_InFocus) {
                                j++;
                            } else {
                                break;
                            }
                        }
                    case Applet_Exit:
                    case Account_Active:
                    case Account_Inactive:
                        break;

                    case Applet_InFocus:
                        last_ts = this->events[j]->steadyTimestamp;
                        last_clock = this->events[j]->clockTimestamp;
                        in_before = false;
                        if (this->events[j]->clockTimestamp < start_ts) {
                            last_clock = start_ts;
                            in_before = true;
                        } else if (this->events[j]->clockTimestamp >= end_ts) {
                            done = true;
                        }
                        break;

                    case Applet_OutFocus:
                        if (this->events[j]->clockTimestamp >= end_ts) {
                            stats->playtime += (end_ts - last_clock);
                        } else if (this->events[j]->clockTimestamp >= start_ts) {
                            if (in_before) {
                                stats->playtime += (this->events[j]->clockTimestamp - last_clock);
                            } else {
                                stats->playtime += (this->events[j]->steadyTimestamp - last_ts);
                            }
                        }

                        // Move to last out focus (I don't know why the log has multiple)
                        while (j+1 < this->events.size()) {
                            if (this->events[j+1]->eventType == Applet_OutFocus) {
                                j++;
                            } else {
                                break;
                            }
                        }
                        break;
                }
            }
        }

        return stats;
    }

    PlayData::PlayData() {
        // Position of first event to read
        s32 offset = 0;
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
                            event->userID.uid[0] = pEvents[i].eventData.account.uid[0];
                            event->userID.uid[0] = (event->userID.uid[0] << 32) | pEvents[i].eventData.account.uid[1];
                            event->userID.uid[1] = (event->userID.uid[1] << 32) | pEvents[i].eventData.account.uid[2];
                            event->userID.uid[1] = (event->userID.uid[1] << 32) | pEvents[i].eventData.account.uid[3];

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
                            event->titleID = pEvents[i].eventData.applet.program_id[0];
                            event->titleID = (event->titleID << 32) | pEvents[i].eventData.applet.program_id[1];

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

    std::vector<TitleID> PlayData::getLoggedTitleIDs() {
        std::vector<TitleID> ids;
        for (size_t i = 0; i < this->events.size(); i++) {
            if (this->events[i]->type == PlayEvent_Applet) {
                // Exclude this title (I dunno what it is but it causes crashes)
                if (this->events[i]->titleID == 0x0100000000001012) {
                    continue;
                }

                if (std::find(ids.begin(), ids.end(), this->events[i]->titleID) == ids.end()) {
                    ids.push_back(this->events[i]->titleID);
                }
            }
        }

        return ids;
    }

    std::vector<PlayEvent> PlayData::getPlayEvents(u64 start, u64 end, TitleID titleID, AccountUid userID) {
        std::vector<PlayEvent> events;

        // Get a vector of PD_Sessions -> should only be one if the provided timestamps are correct
        std::vector<PD_Session> sessions = this->getPDSessions(titleID, userID, start, end);
        for (size_t i = 0; i < sessions.size(); i++) {
            for (size_t j = sessions[i].index; j < sessions[i].index + sessions[i].num; j++) {
                // Ignore repeated OutFocus events
                if (this->events[j]->eventType == Applet_OutFocus && this->events[j-1]->eventType == Applet_OutFocus) {
                    continue;
                }
                events.push_back(*this->events[j]);
            }
        }

        return events;
    }

    std::vector<PlaySession> PlayData::getPlaySessionsForUser(TitleID titleID, AccountUid userID) {
        // Get list of pd_sessions
        struct tm end = Utils::Time::getTmForCurrentTime();
        std::vector<PD_Session> pd_sessions = this->getPDSessions(titleID, userID, 0, std::mktime(&end));

        // Create PlaySessions for each PD_Session
        std::vector<PlaySession> sessions;
        for (size_t i = 0; i < pd_sessions.size(); i++) {
            struct PlaySession p;
            p.startTimestamp = 0;
            p.endTimestamp = 0;

            // Get start and end timestamps
            for (size_t j = pd_sessions[i].index; j < pd_sessions[i].index + pd_sessions[i].num; j++) {
                switch (this->events[j]->eventType) {
                    // Ignore account events
                    case Account_Active:
                    case Account_Inactive:
                    case Applet_InFocus:
                        break;

                    // Set start/end timestamps
                    case Applet_Launch:
                        p.startTimestamp = this->events[j]->clockTimestamp;
                        break;

                    case Applet_Exit:
                        p.endTimestamp = this->events[j]->clockTimestamp;
                        break;


                    case Applet_OutFocus:
                        p.endTimestamp = this->events[j]->clockTimestamp;   // In case of a firmware crash there is no Applet_Exit event logged

                        // Move to last out focus (I don't know why the log has multiple)
                        while (j+1 < this->events.size()) {
                            if (this->events[j+1]->eventType == Applet_OutFocus) {
                                j++;
                            } else {
                                break;
                            }
                        }
                        break;
                }
            }

            // Get playtime
            RecentPlayStatistics * rps = countPlaytime(pd_sessions, p.startTimestamp, p.endTimestamp);
            p.playtime = rps->playtime;
            delete rps;

            sessions.push_back(p);
        }

        return sessions;
    }

    RecentPlayStatistics * PlayData::getRecentStatisticsForUser(u64 start_ts, u64 end_ts, AccountUid userID) {
        return this->countPlaytime(this->getPDSessions(0, userID, start_ts, end_ts), start_ts, end_ts);
    }

    RecentPlayStatistics * PlayData::getRecentStatisticsForTitleAndUser(TitleID titleID, u64 start_ts, u64 end_ts, AccountUid userID) {
        RecentPlayStatistics * s = this->countPlaytime(this->getPDSessions(titleID, userID, start_ts, end_ts), start_ts, end_ts);
        s->titleID = titleID;
        return s;
    }

    PlayStatistics * PlayData::getStatisticsForUser(TitleID titleID, AccountUid userID) {
        PdmPlayStatistics tmp;
        pdmqryQueryPlayStatisticsByApplicationIdAndUserAccountId(titleID, userID, false, &tmp);
        PlayStatistics * stats = new PlayStatistics;
        stats->firstPlayed = tmp.first_timestampUser;
        stats->lastPlayed = tmp.last_timestampUser;
        stats->playtime = tmp.playtimeMinutes * 60;
        stats->launches = tmp.totalLaunches;
        return stats;
    }

    PlayData::~PlayData() {
        while (this->events.size() > 0) {
            delete this->events[0];
            this->events.erase(this->events.begin());
        }
    }
};