// CREDIT: https://github.com/sivel/speedtest-cli/blob/8c7f9fe74003a0ee09d88a890b9ffa0687d6e4ec/speedtest-cli

#include <vector>
#include <chrono>
#include <string>
#include <future>
#include <iostream>

#include "config.h"
#include "server.h"

#include "filegetter.h"
#include "fileputter.h"

#define DOWNLOAD_RETRY_COUNT 5
#define UPLOAD_RETRY_COUNT 5

bool is_infinity_is_zero_is_denormal(float f) {
	return !(isnormal(f) || isnan(f));
}

float *speedtest() {
	//Config config = Config();  // Unused for now.

	Server bestServer = getBestServer();
	std::string base_url = bestServer.get_path();

	int download_sizes[] = { 350, 500, 750, 1000, 1500, 2000, 2500, 3000, 3500, 4000 };
	std::vector<int> upload_sizes = { (int)(.25 * 1024 * 1024), (int)(.5 * 1024 * 1024) };

	std::vector<std::string> urls;

	for (int size : download_sizes) {
		for (int i = 0; i < 4; i++) {
			urls.push_back(std::format("{}/random{}x{}.jpg", base_url, size, size));
		}
	}


	// Hacky solution to it erroring out.
	float download_speed;
	for (int i = 0; i < DOWNLOAD_RETRY_COUNT; i++) {
		download_speed = downloadSpeed(urls);
		if (!is_infinity_is_zero_is_denormal(download_speed))
			break;
	}

	float upload_speed;
	for (int i = 0; i < UPLOAD_RETRY_COUNT; i++) {
		upload_speed = uploadSpeed(bestServer.get_url(), upload_sizes);
		if (!is_infinity_is_zero_is_denormal(upload_speed))
			break;
	}
	
	float *results = new float[2];
	results[0] = download_speed;
	results[1] = upload_speed;
	return results;
}

float downloadSpeed(std::vector<std::string> files) {
	using namespace std::chrono;
	time_point<high_resolution_clock> starttime = high_resolution_clock::now();

	// Create the filegetters
	std::vector<FileGetter*> getters;
	for (std::string file : files) {
		FileGetter *getter = new FileGetter(file, starttime);
		getters.push_back(getter);
		getter->run();
	}

	long long results = 0;
	while (true) {
		time_point<high_resolution_clock> now = high_resolution_clock::now();
		for (std::vector<FileGetter*>::iterator it = getters.begin(); it != getters.end(); ) {
			FileGetter* getter = *it;
			if (getter->getDoneFuture().wait_for(std::chrono::milliseconds(1)) == std::future_status::ready) {
				results += getter->getResult().size();
				it = getters.erase(it);
			}
			else {
				++it;
			}
		}
		if (getters.empty() || duration_cast<seconds>(now - starttime).count() > GETTER_TIMEOUT + 10) {
			break;
		}
	}

	time_point<high_resolution_clock> endtime = high_resolution_clock::now();
	float time = duration_cast<seconds>(endtime - starttime).count();
	return ((results / time) / 1024 / 1024) * 8;
}

float uploadSpeed(std::string url, std::vector<int> sizes) {
	using namespace std::chrono;
	time_point<high_resolution_clock> starttime = high_resolution_clock::now();

	// Create the fileputter
	std::vector<FilePutter*> putters;
	for (int size : sizes) {
		FilePutter* putter = new FilePutter(url, starttime, size);
		putters.push_back(putter);
		putter->run();
	}

	size_t results = 0;
	while (true) {
		time_point<high_resolution_clock> now = high_resolution_clock::now();
		for (std::vector<FilePutter*>::iterator it = putters.begin(); it != putters.end(); ) {
			FilePutter* putter = *it;
			if (putter->getDoneFuture().wait_for(std::chrono::milliseconds(1)) == std::future_status::ready) {
				results += putter->getSize();
				it = putters.erase(it);
			}
			else {
				++it;
			}
		}
		if (putters.empty() || duration_cast<seconds>(now - starttime).count() > (PUTTER_TIMEOUT + 10)) {
			break;
		}
	}

	time_point<high_resolution_clock> endtime = high_resolution_clock::now();
	float time = duration_cast<seconds>(endtime - starttime).count();
	return ((results / time) / 1024.0f / 1024) * 8;
}
