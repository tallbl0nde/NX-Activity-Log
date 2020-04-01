#include <curl/curl.h>
#include "Curl.hpp"
#include "JSON.hpp"
#include <sstream>
#include <iostream>

#define RELEASE_URL "https://api.github.com/repos/tallbl0nde/NX-Activity-Log/releases/latest"
#define TIMEOUT 1000L

namespace Utils::Curl {
    void init() {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    void exit() {
        curl_global_cleanup();
    }

    size_t writeData(char *ptr, size_t size, size_t nmemb, void *userdata) {
        std::ostringstream * ss = (std::ostringstream *)userdata;
        size_t bytes = size * nmemb;
        ss->write(ptr, bytes);
        return bytes;
    }

    UpdateData getLatestMetadata() {
        std::ostringstream ss;
        UpdateData d;
        d.success = false;

        // Setup request
        CURL * c = curl_easy_init();
        curl_easy_setopt(c, CURLOPT_CONNECTTIMEOUT_MS, TIMEOUT);
        curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(c, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(c, CURLOPT_TIMEOUT_MS, TIMEOUT);
        curl_easy_setopt(c, CURLOPT_URL, RELEASE_URL);
        curl_easy_setopt(c, CURLOPT_USERAGENT, "NX-Activity-Log");
        curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(c, CURLOPT_WRITEDATA, &ss);

        // Send request and wait for response
        CURLcode res = curl_easy_perform(c);
        curl_easy_cleanup(c);

        // Parse response
        if (res == CURLE_OK) {
            nlohmann::json j = nlohmann::json::parse(ss.str());
            if (j["tag_name"] != nullptr && j["body"] != nullptr) {
                d.version = j["tag_name"].get<std::string>();
                d.changelog = j["body"].get<std::string>();
                d.success = true;
            }
        }

        return d;
    }
};