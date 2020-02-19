#ifndef TYPES_HPP
#define TYPES_HPP

#include <switch.h>

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
    Auto, // Note this isn't actually set in the object
    Light,
    Dark,
    Custom
};

// Just so function decs. are easier to read
typedef u64 TitleID;

#endif