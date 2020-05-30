#include <cstdio>
#include <curl/curl.h>
#include "utils/Curl.hpp"
#include <fstream>
#include <sstream>

// Timeout for connecting
#define TIMEOUT 3000L
// User Agent string
#define USER_AGENT "NX-Activity-Log"

namespace Utils::Curl {
    void init() {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    void exit() {
        curl_global_cleanup();
    }

    // Setup curl
    CURL * initCurlHandle() {
        CURL * c = curl_easy_init();
        curl_easy_setopt(c, CURLOPT_CONNECTTIMEOUT_MS, TIMEOUT);
        curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(c, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(c, CURLOPT_USERAGENT, USER_AGENT);
        return c;
    }

    // Progress callback (simply passes download params to std::function)
    int progFunc(void * f, curl_off_t dltotal, curl_off_t dlnow, curl_off_t uptotal, curl_off_t upnow) {
        std::function<void(long long int, long long int)> * func = reinterpret_cast< std::function<void(long long int, long long int)> *>(f);
        (*func)(dlnow, dltotal);
        return 0;
    }

    // Write data to stringstream
    static size_t writeData(char *ptr, size_t size, size_t nmemb, void *userdata) {
        std::ostringstream * ss = (std::ostringstream *)userdata;
        size_t bytes = size * nmemb;
        ss->write(ptr, bytes);
        return bytes;
    }

    // Write data to file
    static size_t writeFile(char *ptr, size_t size, size_t nmemb, void *stream) {
        std::ofstream * file = (std::ofstream *)stream;
        auto before = file->tellp();
        file->write(ptr, nmemb);
        return file->tellp() - before;
    }

    bool downloadToFile(std::string url, std::string path, std::function<void(long long int, long long int)> prog) {
        // Open file
        std::ofstream file(path);

        // Setup request
        CURL * c = initCurlHandle();
        curl_easy_setopt(c, CURLOPT_URL, url.c_str());
        curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, writeFile);
        curl_easy_setopt(c, CURLOPT_WRITEDATA, &file);

        if (prog != nullptr) {
            curl_easy_setopt(c, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(c, CURLOPT_XFERINFODATA, reinterpret_cast<void *>(&prog));
            curl_easy_setopt(c, CURLOPT_XFERINFOFUNCTION, progFunc);
        }

        // Send request and download
        CURLcode res = curl_easy_perform(c);
        curl_easy_cleanup(c);

        return (res == CURLE_OK);
    }

    std::string downloadToString(std::string url) {
        std::ostringstream ss;

        // Setup request
        CURL * c = initCurlHandle();
        curl_easy_setopt(c, CURLOPT_URL, url.c_str());
        curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(c, CURLOPT_WRITEDATA, &ss);

        // Send request and wait for response
        CURLcode res = curl_easy_perform(c);
        curl_easy_cleanup(c);

        if (res == CURLE_OK) {
            return ss.str();
        }
        return "";
    }
};