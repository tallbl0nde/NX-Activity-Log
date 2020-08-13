#include "utils/Lang.hpp"
#include "Types.hpp"

std::string toString(Language l) {
    std::string str;

    switch (l) {
        case Language::Default:
            str = "common.default"_lang;
            break;

        case Language::English:
            str = "English";
            break;

        case Language::French:
            str = "Français";
            break;

        case Language::German:
            str = "Deutsch";
            break;

        case Language::Spanish:
            str = "Español";
            break;

        case Language::Italian:
            str = "Italiano";
            break;

        case Language::Portugese:
            str = "Português";
            break;

        case Language::Russian:
            str = "Pусский";
            break;

        case Language::Turkish:
            str = "Türkçe";
            break;

        case Language::ChineseTraditional:
            str = "繁体中文";
            break;

        case Language::Chinese:
            str = "简体中文";
            break;

        case Language::Korean:
            str = "한국어";
            break;

        default:
            break;
    }

    return str;
}

std::string toString(SortType s) {
    std::string str;

    switch (s) {
        case SortType::AlphaAsc:
            str = "common.sort.name"_lang;
            break;
        case SortType::FirstPlayed:
            str = "common.sort.firstPlayed"_lang;
            break;
        case SortType::LastPlayed:
            str = "common.sort.recentlyPlayed"_lang;
            break;
        case SortType::HoursAsc:
            str = "common.sort.mostPlaytime"_lang;
            break;
        case SortType::HoursDec:
            str = "common.sort.leastPlaytime"_lang;
            break;
        case SortType::LaunchAsc:
            str = "common.sort.mostLaunched"_lang;
            break;
        case SortType::LaunchDec:
            str = "common.sort.leastLaunched"_lang;
            break;

        default:
            break;
    }

    return str;
}

std::string toString(ThemeType t) {
    std::string str;

    switch (t) {
        case Auto:
            str = "settings.appearance.theme.auto"_lang;
            break;

        case Custom:
            str = "settings.appearance.theme.custom"_lang;
            break;

        case Dark:
            str = "settings.appearance.theme.dark"_lang;
            break;

        case Light:
            str = "settings.appearance.theme.light"_lang;
            break;

        default:
            break;
    }

    return str;
}

std::string toString(ViewPeriod v) {
    std::string str;

    switch (v) {
        case ViewPeriod::Day:
            str = "common.view.day"_lang;
            break;

        case ViewPeriod::Month:
            str = "common.view.month"_lang;
            break;

        case ViewPeriod::Year:
            str = "common.view.year"_lang;
            break;

        default:
            break;
    }

    return str;
}