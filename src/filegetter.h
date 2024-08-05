#pragma once
#include <string>
#include <chrono>
#include <future>

using namespace std::chrono;

class FileGetter {
private:
	std::string url;
	std::string result;
	time_point<high_resolution_clock> starttime;

    std::promise<void> donePromise;
    std::future<void> doneFuture;

public:
    FileGetter(const std::string& url, const time_point<high_resolution_clock>& start);


    void run();
    void setUrl(std::string& newUrl) { url = newUrl; }
    std::string getUrl() { return url; }
    void setResult(std::string& newResult) { result = newResult; }
    std::string getResult() { return result; }
    time_point<high_resolution_clock> getStartTime() { return starttime; };
    std::future<void>& getDoneFuture() { return doneFuture; }


};

float downloadSpeed(std::vector<std::string> files);
