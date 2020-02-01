#ifndef UTILS_TIME_HPP
#define UTILS_TIME_HPP

#include <ctime>
#include <string>

// Bunch of helper functions for time_t and tm structs
namespace Utils::Time {
    // TIME_T FUNCTIONS
    // Get time_t equivalent of given tm
    time_t getTimeT(struct tm);

    // Return a string containing number of hours, minutes and seconds
    // Hours and minutes are split with the provided separator if there are no seconds
    std::string playtimeToString(time_t, std::string);

    // Returns a string containing time since timestamp
    std::string timestampToString(time_t);

    // Returns a string with the time since last played
    std::string lastPlayedTimestampToString(time_t);

    // TM STRUCT FUNCTIONS
    // Get time struct for current time
    struct tm getTmForCurrentTime();

    // Get time struct for given value
    struct tm getTm(time_t);

    // Return number of days in tm's month
    int tmGetDaysInMonth(struct tm);

    // Return a string with the date matching the tm struct
    std::string tmToString(struct tm, bool = true, bool = true, bool = true);

    // MISCELLANEOUS FUNCTIONS
    // Returns a string containing the month for the given number (starting from 0)
    std::string getMonthString(int);

    // Returns suffix for given date (within 1 - 31)
    std::string getDateSuffix(int);
};

#endif