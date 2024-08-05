#include <vector>
#include <chrono>
#include <string>
#include <future>
#include <iostream>

#include "config.h"
#include "server.h"
#include "filegetter.h"

// CREDIT: https://github.com/sivel/speedtest-cli/blob/8c7f9fe74003a0ee09d88a890b9ffa0687d6e4ec/speedtest-cli

void speedtest() {
	//Config config = Config();  // Unused for now.

	Server bestServer = getBestServer();

	int sizes[] = { 350, 500, 750, 1000, 1500, 2000, 2500, 3000, 3500, 4000 };

	std::vector<std::string> urls;

	for (int size : sizes) {
		for (int i = 0; i < 4; i++) {
			urls.push_back(std::format("{}/random{}x{}.jpg", bestServer.get_path(), size, size));
		}
	}

	downloadSpeed(urls);
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
		if (getters.empty() || duration_cast<seconds>(now - starttime).count() > 20) {
			break;
		}
	}

	time_point<high_resolution_clock> endtime = high_resolution_clock::now();
	float time = duration_cast<seconds>(endtime - starttime).count();
	std::cout << ((results / time) / 1024 / 1024)*8 << std::endl;
	return results / time;
}
