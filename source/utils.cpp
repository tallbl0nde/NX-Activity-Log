#include "utils.hpp"

namespace Utils {
    // Map from HidControllerKeys -> int
    std::map<HidControllerKeys, int> key_map = {
        {KEY_A, 0},
        {KEY_B, 1},
        {KEY_X, 2},
        {KEY_Y, 3},
        {KEY_LSTICK, 4},
        {KEY_RSTICK, 5},
        {KEY_L, 6},
        {KEY_R, 7},
        {KEY_ZL, 8},
        {KEY_ZR, 9},
        {KEY_PLUS, 10},
        {KEY_MINUS, 11},
        {KEY_DLEFT, 12},
        {KEY_DUP, 13},
        {KEY_DRIGHT, 14},
        {KEY_DDOWN, 15},
        {KEY_LSTICK_LEFT, 16},
        {KEY_LSTICK_UP, 17},
        {KEY_LSTICK_RIGHT, 18},
        {KEY_LSTICK_DOWN, 19},
        {KEY_RSTICK_LEFT, 20},
        {KEY_RSTICK_UP, 21},
        {KEY_RSTICK_RIGHT, 22},
        {KEY_RSTICK_DOWN, 23},
        {KEY_SL_LEFT, 24},
        {KEY_SR_RIGHT, 25},
        {KEY_SL_LEFT, 26},
        {KEY_SR_RIGHT, 27}
    };

    // Returns a string with the time since last played
    std::string formatLastPlayed(u32 ts) {
        if (ts == 0) {
            return "Never played";
        }

        // Get difference between timestamps in seconds
        time_t t = std::time(nullptr);
        time_t t2 = pdmPlayTimestampToPosix(ts);
        double diff = std::difftime(t, t2);

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
            // Create structs for easy fprmatting
            struct tm now = *(std::localtime(&t));
            struct tm stamp = *(std::localtime(&t2));

            str += "on ";
            switch (stamp.tm_mon) {
                case 0:
                    str += "January";
                    break;
                case 1:
                    str += "February";
                    break;
                case 2:
                    str += "March";
                    break;
                case 3:
                    str += "April";
                    break;
                case 4:
                    str += "May";
                    break;
                case 5:
                    str += "June";
                    break;
                case 6:
                    str += "July";
                    break;
                case 7:
                    str += "August";
                    break;
                case 8:
                    str += "September";
                    break;
                case 9:
                    str += "October";
                    break;
                case 10:
                    str += "November";
                    break;
                default:
                    str += "December";
                    break;
            }
            str += " ";
            str += std::to_string(stamp.tm_mday);
            switch (stamp.tm_mday) {
                case 1:
                case 21:
                case 31:
                    str += "st";
                    break;
                case 2:
                case 22:
                    str += "nd";
                    break;
                case 3:
                case 23:
                    str += "rd";
                    break;
                default:
                    str += "th";
                    break;
            }

            // If not in this year show year as well
            if (now.tm_year != stamp.tm_year) {
                str += ", " + std::to_string(stamp.tm_year + 1900);
            }
        }

        return str;
    }

    // Add commas to a number (only does one but shhh)
    std::string formatNumberComma(u32 number) {
        std::string s = std::to_string(number);
        if (s.length() > 3) {
            return s.substr(0, s.length() - 3) + "," + s.substr(s.length() - 3, 3);
        }
        return s;
    }

    // Nicely format playtime (from zero)
    std::string formatPlaytime(u32 minutes, std::string sep) {
        std::string str = "";
        switch (minutes/60){
            case 0:
                break;
            case 1:
                str += "1 hour";
                break;
            default:
                str += std::to_string(minutes/60) + " hours";
                break;
        }
        if (minutes/60 != 0 && minutes%60 != 0){
            str += sep;
        }
        switch (minutes%60){
            case 0:
                if (minutes/60 == 0){
                    str += "0 minutes";
                }
                break;
            case 1:
                str += "1 minute";
                break;
            default:
                str += std::to_string(minutes%60) + " minutes";
                break;
        }
        return str;
    }

    std::string formatTimestamp(u32 ts) {
        if (ts == 0) {
            return "Never";
        }

        // Create struct for easy formatting
        time_t t = pdmPlayTimestampToPosix(ts);
        struct tm stamp = *(std::localtime(&t));

        std::string str;
        switch (stamp.tm_mon) {
            case 0:
                str = "January";
                break;
            case 1:
                str = "February";
                break;
            case 2:
                str = "March";
                break;
            case 3:
                str = "April";
                break;
            case 4:
                str = "May";
                break;
            case 5:
                str = "June";
                break;
            case 6:
                str = "July";
                break;
            case 7:
                str = "August";
                break;
            case 8:
                str = "September";
                break;
            case 9:
                str = "October";
                break;
            case 10:
                str = "November";
                break;
            default:
                str = "December";
                break;
        }
        str += " ";
        str += std::to_string(stamp.tm_mday);
        switch (stamp.tm_mday) {
            case 1:
            case 21:
            case 31:
                str += "st";
                break;
            case 2:
            case 22:
                str += "nd";
                break;
            case 3:
            case 23:
                str += "rd";
                break;
            default:
                str += "th";
                break;
        }
        str += ", " + std::to_string(stamp.tm_year + 1900);

        return str;
    }
};