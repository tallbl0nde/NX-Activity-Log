#ifndef UTILS_CURL_HPP
#define UTILS_CURL_HPP

#include <functional>
#include <string>

namespace Utils::Curl {
    void init();
    void exit();

    // Download file from URL and write to file (returns true on success)
    // (URL, path, progress function(bytes received, total bytes))
    bool downloadToFile(std::string, std::string, std::function<void(long long int, long long int)>);

    // Return response from URL as string (empty on error)
    std::string downloadToString(std::string);
};

#endif