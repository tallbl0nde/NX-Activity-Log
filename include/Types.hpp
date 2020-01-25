#ifndef TYPES_HPP
#define TYPES_HPP

// Enumerations for sorting methods
enum SortType {
    AlphaAsc,       // Alphabetically (A-Z)
    HoursAsc,       // Play time (ascending)
    HoursDec,       // Play time (descending)
    LaunchAsc,      // Launches (ascending)
    LaunchDec,      // Launches (descending)
    FirstPlayed,    // First played (earliest first)
    LastPlayed,     // Last played (most recent first)
};

enum ThemeType {
    T_Auto, // Note this isn't actually set in the object
    T_Light,
    T_Dark,
    T_Custom
};

#endif