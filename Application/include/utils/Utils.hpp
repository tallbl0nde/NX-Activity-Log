#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

namespace Utils {
    // Copy file from source path to dest path
    void copyFile(std::string, std::string);

    // Format given hour (0 - 23) in 12 hour format
    std::string format12H(unsigned short);

    // Format heading (takes username)
    std::string formatHeading(std::string);

    // Add commas to provided number
    std::string formatNumberComma(unsigned int);

    // Converts passed number to a hexadecimal string
    std::string formatHexString(const uint64_t);

    // Converts the provided hex string to a 64bit number
    uint64_t stringToU64(const std::string &);

    // Insert given version into string (replacing $[v]) (string, ver)
    std::string insertVersionInString(std::string, std::string);

    // Format the given timestamp as 'last played' string
    std::string lastPlayedToString(uint64_t);

    // Format the given number of launches into a string
    std::string launchesToString(unsigned int);
    std::string launchesToPlayedString(unsigned int);

    // Format the given playtime (in seconds) into hours and minutes
    std::string playtimeToString(uint64_t);

    // Format the given playtime (in seconds) into 'played for' string
    std::string playtimeToPlayedForString(uint64_t);

    // Format the given playtime (in seconds) into 'total playtime' string
    std::string playtimeToTotalPlaytimeString(uint64_t);

    // Merges two vectors into one (for sorting)
    // Vector to merge into, two vectors to merge
    template <typename A, typename B>
    void mergeVectors(std::vector<std::pair<A, B> >& m, const std::vector<A>& a, const std::vector<B>& b) {
        for (size_t i = 0; i < a.size(); i++) {
            m.push_back(std::make_pair(a[i], b[i]));
        }
    }

    // Splits a vector of pairs into two vectors
    // Vector with pairs, vectors to split into
    template <typename A, typename B>
    void splitVectors(const std::vector<std::pair<A, B> >& m, std::vector<A>& a, std::vector<B>& b) {
        for (size_t i = 0; i < m.size(); i++) {
            a[i] = m[i].first;
            b[i] = m[i].second;
        }
    }

    // Round the given double to the specified number of decimal places
    double roundToDecimalPlace(double, unsigned int);

    // Truncate string to given decimal places (don't use on strings without a decimal!)
    // Does nothing if outside of range or no decimal place
    std::string truncateToDecimalPlace(std::string, unsigned int);
};

#endif
