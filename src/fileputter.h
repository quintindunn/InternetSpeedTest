#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <future>

#define PUTTER_TIMEOUT 30

using namespace std::chrono;

class FilePutter {
private:
    std::string url;
    time_point<high_resolution_clock> starttime;
    long size;

    std::promise<void> donePromise;
    std::future<void> doneFuture;

public:
    FilePutter(const std::string& url, const time_point<high_resolution_clock>& start, size_t size);

    void run();

    void setUrl(std::string& newUrl) { url = newUrl; }
    std::string getUrl() { return url; }

    void setSize(size_t newSize) { size = newSize; }
    size_t getSize() { return size; }

    time_point<high_resolution_clock> getStartTime() { return starttime; };

    std::future<void>& getDoneFuture() { return doneFuture; }


};

float uploadSpeed(std::string url, std::vector<int> sizes);
