#include "utils/Lang.hpp"
#include <regex>
#include "utils/Time.hpp"

namespace Utils::Time {
    time_t getTimeT(struct tm t) {
        return std::mktime(&t);
    }

    std::string timestampToString(time_t ts) {
        struct tm tmp = getTm(ts);
        return tmToDate(tmp, true);
    }

    bool areDifferentDates(struct tm a, struct tm b) {
        a.tm_hour = 0;
        a.tm_min = 0;
        a.tm_sec = 0;
        b.tm_hour = 0;
        b.tm_min = 0;
        b.tm_sec = 0;
        if (std::difftime(getTimeT(a), getTimeT(b)) != 0) {
            return true;
        }
        return false;
    }

    struct tm decreaseTm(struct tm t, char c) {
        switch (c) {
            case 'D':
                t.tm_mday--;
                if (t.tm_mday < 1) {
                    t.tm_mon--;
                    if (t.tm_mon < 0) {
                        t.tm_year--;
                        t.tm_mon = 11;
                    }
                    t.tm_mday += tmGetDaysInMonth(t);
                }
                break;

            case 'M':
                t.tm_mon--;
                if (t.tm_mon < 0) {
                    t.tm_year--;
                    t.tm_mon = 11;
                }
                break;

            case 'Y':
                t.tm_year--;
                break;

            default:
                break;
        }

        return t;
    }

    struct tm increaseTm(struct tm t, char c) {
        switch (c) {
            case 'D':
                t.tm_mday++;
                if (t.tm_mday > tmGetDaysInMonth(t)) {
                    t.tm_mon++;
                    if (t.tm_mon > 11) {
                        t.tm_year++;
                        t.tm_mon = 0;
                    }
                    t.tm_mday = 1;
                }
                break;

            case 'M':
                t.tm_mon++;
                if (t.tm_mon > 11) {
                    t.tm_year++;
                    t.tm_mon = 0;
                }
                break;

            case 'Y':
                t.tm_year++;
                break;

            default:
                break;
        }

        return t;
    }

    struct tm getTmForCurrentTime() {
        time_t t = std::time(nullptr);
        struct tm * tmp = std::localtime(&t);
        return *(tmp);
    }

    struct tm getTm(time_t t) {
        return *(std::localtime(&t));
    }

    uint32_t posixTimestampToPdm(uint64_t timestamp) {
        return static_cast<uint32_t>((timestamp - 946598400)/60);
    }

    int tmGetDaysInMonth(struct tm t) {
        switch (t.tm_mon) {
            // Except February alone...
            case 1:
                if (t.tm_year % 4 == 0) {
                    return 29;
                } else {
                    return 28;
                }
                break;

            // 30 days has September, April, June + November...
            case 3:
            case 5:
            case 8:
            case 10:
                return 30;
                break;

            // All the rest have 31...
            default:
                return 31;
                break;
        }
    }

    // Resulting string has max size
    std::string tmToString(struct tm t, std::string f, unsigned short l) {
        char buf[l+1];
        std::strftime(buf, l+1, f.c_str(), &t);
        return std::string(buf);
    }

    std::string tmToDate(struct tm t, bool b) {
        std::string str;
        if (b) {
            str = std::regex_replace("common.dateFormatYear"_lang, std::regex("\\$\\[y]"), tmToString(t, "%Y", 4) + "common.yearSuffix"_lang);
        } else {
            str = "common.dateFormat"_lang;
        }
        str = std::regex_replace(str, std::regex("\\$\\[d]"), std::to_string(t.tm_mday));
        str = std::regex_replace(str, std::regex("\\$\\[s]"), getDateSuffix(t.tm_mday));
        str = std::regex_replace(str, std::regex("\\$\\[m]"), getMonthString(t.tm_mon));

        return str;
    }

    std::string dateToActivityForString(struct tm t, ViewPeriod v) {
        switch (v) {
            case ViewPeriod::Day: {
                std::string str;
                if (t.tm_year != getTmForCurrentTime().tm_year) {
                    str = std::regex_replace("common.activityFor.dayYear"_lang, std::regex("\\$\\[y]"), tmToString(t, "%Y", 4) + "common.yearSuffix"_lang);
                } else {
                    str = "common.activityFor.day"_lang;
                }
                str = std::regex_replace(str, std::regex("\\$\\[d]"), std::to_string(t.tm_mday));
                str = std::regex_replace(str, std::regex("\\$\\[s]"), getDateSuffix(t.tm_mday));
                return std::regex_replace(str, std::regex("\\$\\[m]"), getMonthString(t.tm_mon));
                break;
            }

            case ViewPeriod::Month: {
                std::string str = std::regex_replace("common.activityFor.month"_lang, std::regex("\\$\\[m]"), getMonthString(t.tm_mon));
                return std::regex_replace(str, std::regex("\\$\\[y]"), tmToString(t, "%Y", 4) + "common.yearSuffix"_lang);
                break;
            }

            case ViewPeriod::Year:
                return std::regex_replace("common.activityFor.year"_lang, std::regex("\\$\\[y]"), tmToString(t, "%Y", 4) + "common.yearSuffix"_lang);
                break;

            default:
                break;
        }

        return "";
    }

    std::string getAMPM(int h, bool b) {
        if (h < 12) {
            return (b ? "common.AM"_lang : "common.am"_lang);
        }
        return (b ? "common.PM"_lang : "common.pm"_lang);
    }

    std::string getDateSuffix(int d) {
        switch (d) {
            case 1:
            case 21:
            case 31:
                return "common.dateSuffix.st"_lang;
                break;
            case 2:
            case 22:
                return "common.dateSuffix.nd"_lang;
                break;
            case 3:
            case 23:
                return "common.dateSuffix.rd"_lang;
                break;
            default:
                return "common.dateSuffix.th"_lang;
                break;
        }
    }

    std::string getMonthString(int m) {
        switch (m) {
            case 0:
                return "common.month.jan"_lang;
                break;
            case 1:
                return "common.month.feb"_lang;
                break;
            case 2:
                return "common.month.mar"_lang;
                break;
            case 3:
                return "common.month.apr"_lang;
                break;
            case 4:
                return "common.month.may"_lang;
                break;
            case 5:
                return "common.month.jun"_lang;
                break;
            case 6:
                return "common.month.jul"_lang;
                break;
            case 7:
                return "common.month.aug"_lang;
                break;
            case 8:
                return "common.month.sep"_lang;
                break;
            case 9:
                return "common.month.oct"_lang;
                break;
            case 10:
                return "common.month.nov"_lang;
                break;
            default:
                return "common.month.dec"_lang;
                break;
        }
    }

    std::string getShortMonthString(int m) {
        switch (m) {
            case 0:
                return "common.month.short.jan"_lang;
                break;
            case 1:
                return "common.month.short.feb"_lang;
                break;
            case 2:
                return "common.month.short.mar"_lang;
                break;
            case 3:
                return "common.month.short.apr"_lang;
                break;
            case 4:
                return "common.month.short.may"_lang;
                break;
            case 5:
                return "common.month.short.jun"_lang;
                break;
            case 6:
                return "common.month.short.jul"_lang;
                break;
            case 7:
                return "common.month.short.aug"_lang;
                break;
            case 8:
                return "common.month.short.sep"_lang;
                break;
            case 9:
                return "common.month.short.oct"_lang;
                break;
            case 10:
                return "common.month.short.nov"_lang;
                break;
            default:
                return "common.month.short.dec"_lang;
                break;
        }
    }
};
