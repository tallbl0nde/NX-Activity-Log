#include "Time.hpp"

namespace Utils::Time {
    time_t getTimeT(struct tm t) {
        return std::mktime(&t);
    }

    std::string playtimeToString(time_t secs, std::string sep) {
        if (secs == 0) {
            return "less than a minute";
        }

        std::string str = "";

        // Convert seconds into hrs, mins, secs
        unsigned int mins = secs/60;
        unsigned int hrs = mins/60;
        mins %= 60;
        secs %= 60;

        switch (hrs) {
            case 0:
                break;
            case 1:
                str += "1 hour";
                break;
            default:
                str += std::to_string(hrs) + " hours";
                break;
        }

        if (mins != 0 && hrs != 0){
            str += sep;
        }

        switch (mins){
            case 0:
                break;
            case 1:
                str += "1 minute";
                break;
            default:
                str += std::to_string(mins) + " minutes";
                break;
        }

        if (hrs == 0) {
            if (mins != 0 && secs != 0){
                str += sep;
            }

            switch (secs){
                case 0:
                    break;
                case 1:
                    str += "1 second";
                    break;
                default:
                    str += std::to_string(secs) + " seconds";
                    break;
            }
        }

        return str;
    }

    std::string timestampToString(time_t ts) {
        struct tm tmp = getTm(ts);
        return tmToString(tmp, "%B ", 10) + std::to_string(tmp.tm_mday) + getDateSuffix(tmp.tm_mday) + tmToString(tmp, ", %Y", 6);
    }

    std::string lastPlayedTimestampToString(time_t ts) {
        // Get difference between timestamps in seconds
        time_t t = std::time(nullptr);
        double diff = std::difftime(t, ts);

        // Negative values indicate played with a future date
        if (diff < 0) {
            return "Played in the future";
        }

        std::string str = "Last played ";
        if (diff < 60) {
            str += "seconds ago";

        // If within the last hour show minutes
        } else if (diff < 3600) {
            str += std::to_string((int)diff/60);
            if ((int)(diff/60) == 1) {
                str += " minute ago";
            } else {
                str += " minutes ago";
            }

        // If within last day show hours
        } else if (diff < 86400) {
            str += std::to_string((int)diff/3600);
            if ((int)(diff/3600) == 1) {
                str += " hour ago";
            } else {
                str += " hours ago";
            }

        // If within last month show days
        } else if (diff < 2678400) {
            str += std::to_string((int)diff/86400);
            if ((int)(diff/86400) == 1) {
                str += " day ago";
            } else {
                str += " days ago";
            }

        // Otherwise show date
        } else {
            // Add month + date
            struct tm tmp = getTm(ts);
            str += tmToString(tmp, "on %B ", 13) + std::to_string(tmp.tm_mday) + getDateSuffix(tmp.tm_mday);

            // If not in this year show year as well
            struct tm now = getTmForCurrentTime();
            if (now.tm_year != tmp.tm_year) {
                str += tmToString(tmp, ", %Y", 6);
            }
        }

        return str;
    }

    bool areDifferentDates(struct tm a, struct tm b) {
        a.tm_hour = 0;
        a.tm_min = 0;
        a.tm_sec = 0;
        b.tm_hour = 0;
        b.tm_min = 0;
        b.tm_sec = 0;
        if (std::difftime(getTimeT(a), getTimeT(b)) != 0) {
            return true;
        }
        return false;
    }

    struct tm decreaseTm(struct tm t, char c) {
        switch (c) {
            case 'D':
                t.tm_mday--;
                if (t.tm_mday < 1) {
                    t.tm_mon--;
                    if (t.tm_mon < 0) {
                        t.tm_year--;
                        t.tm_mon = 11;
                    }
                    t.tm_mday += tmGetDaysInMonth(t);
                }
                break;

            case 'M':
                t.tm_mon--;
                if (t.tm_mon < 0) {
                    t.tm_year--;
                    t.tm_mon = 11;
                }
                break;

            case 'Y':
                t.tm_year--;
                break;

            default:
                break;
        }

        return t;
    }

    struct tm increaseTm(struct tm t, char c) {
        switch (c) {
            case 'D':
                t.tm_mday++;
                if (t.tm_mday > tmGetDaysInMonth(t)) {
                    t.tm_mon++;
                    if (t.tm_mon > 11) {
                        t.tm_year++;
                        t.tm_mon = 0;
                    }
                    t.tm_mday = 1;
                }
                break;

            case 'M':
                t.tm_mon++;
                if (t.tm_mon > 11) {
                    t.tm_year++;
                    t.tm_mon = 0;
                }
                break;

            case 'Y':
                t.tm_year++;
                break;

            default:
                break;
        }

        return t;
    }

    struct tm getTmForCurrentTime() {
        time_t t = std::time(nullptr);
        struct tm * tmp = std::localtime(&t);
        return *(tmp);
    }

    struct tm getTm(time_t t) {
        return *(std::localtime(&t));
    }

    int tmGetDaysInMonth(struct tm t) {
        switch (t.tm_mon) {
            // Except February alone...
            case 1:
                if (t.tm_year % 4 == 0) {
                    return 29;
                } else {
                    return 28;
                }
                break;

            // 30 days has September, April, June + November...
            case 3:
            case 5:
            case 8:
            case 10:
                return 30;
                break;

            // All the rest have 31...
            default:
                return 31;
                break;
        }
    }

    // Resulting string has max size
    std::string tmToString(struct tm t, std::string f, unsigned short l) {
        char buf[l+1];
        std::strftime(buf, l+1, f.c_str(), &t);
        return std::string(buf);
    }

    std::string tmToDate(struct tm t, bool b) {
        std::string str = tmToString(t, "%B ", 10);
        str += std::to_string(t.tm_mday) + getDateSuffix(t.tm_mday);
        if (b) {
            str += tmToString(t, ", %Y", 6);
        }
        return str;
    }

    std::string getAMPM(int h, bool b) {
        if (h < 12) {
            return (b ? "AM" : "am");
        }
        return (b ? "PM" : "pm");
    }

    std::string getDateSuffix(int d) {
        switch (d) {
            case 1:
            case 21:
            case 31:
                return "st";
                break;
            case 2:
            case 22:
                return "nd";
                break;
            case 3:
            case 23:
                return "rd";
                break;
            default:
                return "th";
                break;
        }
    }

    std::string getShortMonthString(int m) {
        switch (m) {
            case 0:
                return "Jan";
                break;
            case 1:
                return "Feb";
                break;
            case 2:
                return "Mar";
                break;
            case 3:
                return "Apr";
                break;
            case 4:
                return "May";
                break;
            case 5:
                return "Jun";
                break;
            case 6:
                return "Jul";
                break;
            case 7:
                return "Aug";
                break;
            case 8:
                return "Sep";
                break;
            case 9:
                return "Oct";
                break;
            case 10:
                return "Nov";
                break;
            default:
                return "Dec";
                break;
        }
    }
};