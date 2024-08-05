#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>

#include "requester.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


std::string get(std::string url) {
    // Makes a GET request using CURL.

    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    long responseCode;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
#ifdef VERBOSE_CURL
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.64.1");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return readBuffer;
}


std::string post(std::string url, std::vector<unsigned char> data) {
    // Makes a POST request using CURL.

    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    long responseCode;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.data());


        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
#ifdef VERBOSE_CURL
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.64.1");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return readBuffer;
}