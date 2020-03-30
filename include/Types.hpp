#ifndef TYPES_HPP
#define TYPES_HPP

#include <switch.h>

// App language
enum Language {
    Default,        // Match system language
    English
};

// Enumeration for screens (allows for easy switching)
enum ScreenID {
    UserSelect,
    AllActivity,
    RecentActivity,
    Settings,
    Details
};

// Enumerations for sorting methods
enum SortType {
    AlphaAsc,       // Alphabetically (A-Z)
    HoursAsc,       // Playtime (ascending)
    HoursDec,       // Playtime (descending)
    LaunchAsc,      // Launches (ascending)
    LaunchDec,      // Launches (descending)
    FirstPlayed,    // First played (earliest first)
    LastPlayed,     // Last played (most recent first)
};

enum ThemeType {
    Auto, // Note this isn't actually set in the object
    Light,
    Dark,
    Custom
};

// Period to view recent stats for
enum class ViewPeriod {
    Day,
    Month,
    Year
};

// Just so function decs. are easier to read
typedef u64 TitleID;

#endif