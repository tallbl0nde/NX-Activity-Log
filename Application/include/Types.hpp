#ifndef TYPES_HPP
#define TYPES_HPP

#include "Aether/Aether.hpp"
#include <string>
#include <switch.h>

// App language
enum Language {
    Default,        // Match system language
    English,
    French,
    German,
    Spanish,
    Italian,
    Portugese,
    Russian,
    Turkish,
    ChineseTraditional,
    Chinese,
    Korean,
    TotalLanguages  // Total number of languages (only used for iterating)
};
// Return string matching language
std::string toString(Language);

// Type of screen creation
enum class ScreenCreate {
    Normal,     // No specific reason
    Language,   // Language was changed
    Theme,      // Theme was changed
    ThemeEdit   // Theme was customized
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
    FirstPlayed,    // First played (earliest first)
    LastPlayed,     // Last played (most recent first)
    HoursAsc,       // Playtime (ascending)
    HoursDec,       // Playtime (descending)
    LaunchAsc,      // Launches (ascending)
    LaunchDec,      // Launches (descending)
    TotalSorts      // Total number of sort methods (only used for iterating)
};
// Return string matching sort type
std::string toString(SortType);

enum ThemeType {
    Auto,           // Note this isn't actually set in the object
    Light,          // Basic White
    Dark,           // Basic Black
    Custom,         // Custom set by user
    TotalThemes     // Total number of themes (only used for iterating)
};
// Return string matching theme type
std::string toString(ThemeType);

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
enum ViewPeriod {
    Day,
    Month,
    Year,
    TotalViews      // Total number of view types (only used for iterating)
};
// Return string matching theme type
std::string toString(ViewPeriod);

// Just so function decs. are easier to read
typedef u64 TitleID;

#endif