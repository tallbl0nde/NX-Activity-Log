#include <cmath>
#include <fstream>
#include <iomanip>
#include "utils/Lang.hpp"
#include <regex>
#include "utils/Utils.hpp"
#include "utils/Time.hpp"

namespace Utils {
    void copyFile(std::string src, std::string dest) {
        std::ifstream srcF(src, std::ios::binary);
        std::ofstream destF(dest, std::ios::binary);

        destF << srcF.rdbuf();

        srcF.close();
        destF.flush();
        destF.close();
    }

    std::string format12H(unsigned short h) {
        int h2 = (h > 12 ? h - 12 : h);
        if (h2 == 0) {
            h2 = 12;
        }
        std::string str = std::regex_replace("common.12H"_lang, std::regex("\\$\\[h]"), std::to_string(h2));
        return std::regex_replace(str, std::regex("\\$\\[s]"), Utils::Time::getAMPM(h, false));
    }

    std::string formatHeading(std::string name) {
        return std::regex_replace("common.heading"_lang, std::regex("\\$\\[name]"), name);
    }

    // Converts the provided 64bit number to a hex string
    std::string formatHexString(const uint64_t value) {
        std::stringstream ss;
        ss << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << value;
        return ss.str();
    }

    // Add commas to a number (only does one but shhh)
    std::string formatNumberComma(unsigned int number) {
        std::string s = std::to_string(number);
        if (s.length() > 3) {
            return s.substr(0, s.length() - 3) + "," + s.substr(s.length() - 3, 3);
        }
        return s;
    }

    uint64_t stringToU64(const std::string & str) {
        uint64_t val;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> val;
        return val;
    }

    std::string insertVersionInString(std::string str, std::string ver) {
        return std::regex_replace(str, std::regex("\\$\\[v]"), ver);
    }

    std::string lastPlayedToString(uint64_t t) {
        struct tm now = Utils::Time::getTmForCurrentTime();
        struct tm ts = Utils::Time::getTm(t);
        int64_t diff = Utils::Time::getTimeT(now) - Utils::Time::getTimeT(ts);

        // In the future
        if (diff < 0) {
            return "common.lastPlayed.future"_lang;

        // Within last minute
        } else if (diff < 60) {
            return "common.lastPlayed.secs"_lang;
        }

        // Within last hour
        if (diff < 3600) {
            int m = diff/60;
            if (m == 1) {
                return "common.lastPlayed.1min"_lang;
            } else {
                return std::regex_replace("common.lastPlayed.mins"_lang, std::regex("\\$\\[m]"), std::to_string(m));
            }

        // Within last day
        } else if (diff < 86400) {
            int h = diff/3600;
            if (h == 1) {
                return "common.lastPlayed.1hr"_lang;
            } else {
                return std::regex_replace("common.lastPlayed.hrs"_lang, std::regex("\\$\\[h]"), std::to_string(h));
            }

        // Within last month
        } else if (diff < 2678400) {
            int d = diff/86400;
            if (d == 1) {
                return "common.lastPlayed.1day"_lang;
            } else {
                return std::regex_replace("common.lastPlayed.days"_lang, std::regex("\\$\\[d]"), std::to_string(d));
            }
        }

        // Otherwise show date
        std::string str;
        // Show year if not within the same year
        if (now.tm_year != ts.tm_year) {
            str = std::regex_replace("common.lastPlayed.dateYear"_lang, std::regex("\\$\\[y]"), Utils::Time::tmToString(ts, "%Y", 4) + "common.yearSuffix"_lang);
        } else {
            str = "common.lastPlayed.date"_lang;
        }
        str = std::regex_replace(str, std::regex("\\$\\[d]"), std::to_string(ts.tm_mday));
        str = std::regex_replace(str, std::regex("\\$\\[s]"), Utils::Time::getDateSuffix(ts.tm_mday));
        str = std::regex_replace(str, std::regex("\\$\\[m]"), Utils::Time::getMonthString(ts.tm_mon));

        return str;
    }

    std::string launchesToString(unsigned int l) {
        if (l != 1) {
            return std::regex_replace("details.timesPlayed.multiple"_lang, std::regex("\\$\\[t]"), std::to_string(l));
        }
        return "details.timesPlayed.once"_lang;
    }

    std::string launchesToPlayedString(unsigned int l) {
        if (l != 1) {
            return std::regex_replace("common.timesPlayed.multiple"_lang, std::regex("\\$\\[t]"), std::to_string(l));
        }
        return "common.timesPlayed.once"_lang;
    }

    std::string playtimeToString(uint64_t s) {
        if (s == 0) {
            return "common.playtime.0sec"_lang;
        } else if (s == 1) {
            return "common.playtime.1sec"_lang;
        } else if (s < 60) {
            return std::regex_replace("common.playtime.secs"_lang, std::regex("\\$\\[s]"), std::to_string(s));
        }

        uint64_t h = s/3600;
        unsigned int m = (s/60)%60;
        s = s%60;

        if (h == 0) {
            if (m == 1) {
                if (s == 0) {
                    return std::regex_replace("common.playtime.1min"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else if (s == 1) {
                    return std::regex_replace("common.playtime.1min1sec"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else {
                    return std::regex_replace("common.playtime.1minSecs"_lang, std::regex("\\$\\[s]"), std::to_string(s));
                }
            } else {
                if (s == 0) {
                    return std::regex_replace("common.playtime.mins"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else if (s == 1) {
                    return std::regex_replace("common.playtime.mins1sec"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else {
                    std::string str = std::regex_replace("common.playtime.minsSecs"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                    return std::regex_replace(str, std::regex("\\$\\[s]"), std::to_string(s));
                }
            }
        } else if (h == 1) {
            if (m == 0) {
                return "common.playtime.1hr"_lang;
            } else if (m == 1) {
                return "common.playtime.1hr1min"_lang;
            } else {
                return std::regex_replace("common.playtime.1hrMins"_lang, std::regex("\\$\\[m]"), std::to_string(m));
            }
        } else {
            if (m == 0) {
                return std::regex_replace("common.playtime.hrs"_lang, std::regex("\\$\\[h]"), std::to_string(h));
            } else if (m == 1) {
                return std::regex_replace("common.playtime.hrs1min"_lang, std::regex("\\$\\[h]"), std::to_string(h));
            }
        }

        std::string str = std::regex_replace("common.playtime.hrsMins"_lang, std::regex("\\$\\[h]"), std::to_string(h));
        return std::regex_replace(str, std::regex("\\$\\[m]"), std::to_string(m));
    }

    std::string playtimeToPlayedForString(uint64_t s) {
        if (s < 60) {
            return "common.playedFor.0min"_lang;
        }

        uint64_t h = s/3600;
        unsigned int m = (s/60)%60;
        s = s%60;

        if (h == 0) {
            if (m == 1) {
                if (s == 0) {
                    return std::regex_replace("common.playedFor.1min"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else if (s == 1) {
                    return std::regex_replace("common.playedFor.1min1sec"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else {
                    return std::regex_replace("common.playedFor.1minSecs"_lang, std::regex("\\$\\[s]"), std::to_string(s));
                }
            } else {
                if (s == 0) {
                    return std::regex_replace("common.playedFor.mins"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else if (s == 1) {
                    return std::regex_replace("common.playedFor.mins1sec"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else {
                    std::string str = std::regex_replace("common.playedFor.minsSecs"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                    return std::regex_replace(str, std::regex("\\$\\[s]"), std::to_string(s));
                }
            }
        } else if (h == 1) {
            if (m == 0) {
                return "common.playedFor.1hr"_lang;
            } else if (m == 1) {
                return "common.playedFor.1hr1min"_lang;
            } else {
                return std::regex_replace("common.playedFor.1hrMins"_lang, std::regex("\\$\\[m]"), std::to_string(m));
            }
        } else {
            if (m == 0) {
                return std::regex_replace("common.playedFor.hrs"_lang, std::regex("\\$\\[h]"), std::to_string(h));
            } else if (m == 1) {
                return std::regex_replace("common.playedFor.hrs1min"_lang, std::regex("\\$\\[h]"), std::to_string(h));
            }
        }

        std::string str = std::regex_replace("common.playedFor.hrsMins"_lang, std::regex("\\$\\[h]"), std::to_string(h));
        return std::regex_replace(str, std::regex("\\$\\[m]"), std::to_string(m));
    }

    std::string playtimeToTotalPlaytimeString(uint64_t s) {
        if (s == 0) {
            return "common.totalPlaytime.0sec"_lang;
        } else if (s < 60) {
            return "common.totalPlaytime.0min"_lang;
        }

        uint64_t h = s/3600;
        unsigned int m = (s/60)%60;
        s = s%60;

        if (h == 0) {
            if (m == 1) {
                if (s == 0) {
                    return std::regex_replace("common.totalPlaytime.1min"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else if (s == 1) {
                    return std::regex_replace("common.totalPlaytime.1min1sec"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else {
                    return std::regex_replace("common.totalPlaytime.1minSecs"_lang, std::regex("\\$\\[s]"), std::to_string(s));
                }
            } else {
                if (s == 0) {
                    return std::regex_replace("common.totalPlaytime.mins"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else if (s == 1) {
                    return std::regex_replace("common.totalPlaytime.mins1sec"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                } else {
                    std::string str = std::regex_replace("common.totalPlaytime.minsSecs"_lang, std::regex("\\$\\[m]"), std::to_string(m));
                    return std::regex_replace(str, std::regex("\\$\\[s]"), std::to_string(s));
                }
            }
        } else if (h == 1) {
            if (m == 0) {
                return "common.totalPlaytime.1hr"_lang;
            } else if (m == 1) {
                return "common.totalPlaytime.1hr1min"_lang;
            } else {
                return std::regex_replace("common.totalPlaytime.1hrMins"_lang, std::regex("\\$\\[m]"), std::to_string(m));
            }
        } else {
            if (m == 0) {
                return std::regex_replace("common.totalPlaytime.hrs"_lang, std::regex("\\$\\[h]"), std::to_string(h));
            } else if (m == 1) {
                return std::regex_replace("common.totalPlaytime.hrs1min"_lang, std::regex("\\$\\[h]"), std::to_string(h));
            }
        }

        std::string str = std::regex_replace("common.totalPlaytime.hrsMins"_lang, std::regex("\\$\\[h]"), std::to_string(h));
        return std::regex_replace(str, std::regex("\\$\\[m]"), std::to_string(m));
    }

    // Round a double to given decimal places
    double roundToDecimalPlace(double val, unsigned int p) {
        for (unsigned int i = 0; i < p; i++) {
            val *= 10.0;
        }
        val = std::round(val);
        for (unsigned int i = 0; i < p; i++) {
            val /= 10.0;
        }
        return val;
    }

    std::string truncateToDecimalPlace(std::string str, unsigned int p) {
        size_t dec = str.find(".");
        if (dec == std::string::npos || p >= str.length() - dec) {
            return str;
        }

        // Cut off decimal place if zero
        if (p == 0) {
            dec--;
        }

        return str.substr(0, dec + 1 + p);
    }
};
