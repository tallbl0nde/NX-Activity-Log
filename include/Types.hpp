#ifndef TYPES_HPP
#define TYPES_HPP

#include "Aether.hpp"
#include <string>
#include <switch.h>

// App language
enum Language {
    Default,        // Match system language
    English,
    French,
    German,
    Italian,
    Portugese,
    Russian
};

// Enumeration for screens (allows for easy switching)
enum ScreenID {
    UserSelect,
    AllActivity,
    RecentActivity,
    Settings,
    Details,
    Update,
    CustomTheme
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

// Variables (colours) forming a theme
typedef struct {
    Aether::Colour accent;
    Aether::Colour altBG;
    Aether::Colour bg;
    Aether::Colour fg;
    Aether::Colour highlight1;
    Aether::Colour highlight2;
    Aether::Colour highlightBG;
    Aether::Colour mutedLine;
    Aether::Colour mutedText;
    Aether::Colour selected;
    Aether::Colour text;
} ThemeSet;

typedef struct {
    std::string changelog;  // Changelog (with markdown syntax)
    bool success;           // Set true if the query was successful
    std::string url;        // URL of .nro file to download
    std::string version;    // Version of latest release (vX.X.X)
} UpdateData;

// Period to view recent stats for
enum class ViewPeriod {
    Day,
    Month,
    Year
};

// Just so function decs. are easier to read
typedef u64 TitleID;

#endif