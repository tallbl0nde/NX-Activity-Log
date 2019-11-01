#include <sstream>
#include "utils.h"

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

    // Nicely format a time (from zero)
    std::string formatPlaytime(u32 minutes) {
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
            str += ", ";
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
};