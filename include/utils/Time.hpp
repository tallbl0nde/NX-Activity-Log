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
    // Return true if two structs have different dates
    bool areDifferentDates(struct tm, struct tm);

    // Dec/Inc tm by one D/M/Y
    struct tm decreaseTm(struct tm, char);
    struct tm increaseTm(struct tm, char);

    // Get time struct for current time
    struct tm getTmForCurrentTime();

    // Get time struct for given value
    struct tm getTm(time_t);

    // Return number of days in tm's month
    int tmGetDaysInMonth(struct tm);

    // Return a string with the date matching the tm struct in the given format
    // Max string length is optional and defaults to 50
    std::string tmToString(struct tm, std::string, unsigned short = 50);

    // Returns string in format "m d<suffix>, y"
    // Year can be omitted by passing false
    std::string tmToDate(struct tm, bool = true);

    // MISCELLANEOUS FUNCTIONS
    // Returns am/pm for given 24-hour int
    // Set true for upper case
    std::string getAMPM(int, bool = false);

    // Returns suffix for given date (within 1 - 31)
    std::string getDateSuffix(int);

    // Return three character version of month
    std::string getShortMonthString(int);
};

#endif