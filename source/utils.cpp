#include <iomanip>
#include <sstream>
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

    // Nicely format playtime (from zero)
    std::string formatPlaytime(u32 minutes) {
        if (minutes == 0) {
            return "Never played";
        }

        std::string str = "Played for ";
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
            str += " and ";
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

    // Returns a string with the difference between the two timestamps
    std::string formatTimestamps(std::time_t sml, std::time_t lrg) {
        // Calculate difference (in seconds)
        double diff_secs = std::difftime(lrg, sml);

        // Create structs for easy calculations
        struct tm sml_s = *(std::localtime(&sml));
        struct tm lrg_s = *(std::localtime(&lrg));

        // Titles "played" before launch date likely aren't played
        if (sml_s.tm_year < 117) {
            return "";
        }

        // Negative values indicate played with a future date
        if (diff_secs < 0) {
            return "Played in the future";
        }

        std::string str = "Last played ";
        if (diff_secs < 60) {
            str += "seconds ago";

        // If within the last hour show minutes
        } else if (diff_secs < 3600) {
            str += std::to_string((int)diff_secs/60);
            if ((int)(diff_secs/60) == 1) {
                str += " minute ago";
            } else {
                str += " minutes ago";
            }

        // If within last day show hours
        } else if (diff_secs < 86400) {
            str += std::to_string((int)diff_secs/3600);
            if ((int)(diff_secs/3600) == 1) {
                str += " hour ago";
            } else {
                str += " hours ago";
            }

        // If within last month show days
        } else if (diff_secs < 2678400) {
            str += std::to_string((int)diff_secs/86400);
            if ((int)(diff_secs/86400) == 1) {
                str += " day ago";
            } else {
                str += " days ago";
            }

        // If within this year show month
        } else if (diff_secs < 32140800) {
            str += std::to_string((int)diff_secs/2678400);
            if ((int)(diff_secs/2678400) == 1) {
                str += " month ago";
            } else {
                str += " months ago";
            }

        // Otherwise show date w/ year
        } else {
            std::stringstream ss;
            ss << std::put_time(std::localtime(&sml), "%B %d, %Y");
            str += "on " + ss.str();
        }

        return str;
    }
};