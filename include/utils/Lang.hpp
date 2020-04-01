#ifndef UTILS_LANG_HPP
#define UTILS_LANG_HPP

#include "Types.hpp"

namespace Utils::Lang {
    // Set the .json file used for reading strings
    // Returns true on success
    bool setFile(std::string);

    // Set the language
    bool setLanguage(Language);

    // Returns the string matching the given key
    // Returns "?" if no matching key was found
    std::string string(std::string);
};

// Operator for string()
inline std::string operator ""_lang (const char * key, size_t size) {
    return Utils::Lang::string(std::string(key, size));
}

#endif