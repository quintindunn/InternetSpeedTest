#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "string_utils.h"
#include "requester.h"

#define SPEEDTEST_CONFIG_URL "https://www.speedtest.net/speedtest-config.php"

using std::string;

Config::Config() {
	string raw = get(SPEEDTEST_CONFIG_URL);

	std::vector<std::string> lines = split(raw, "\n");
	for (int i = 0; i < lines.size(); i++) {
		string line = lines.at(i);
		std::cout << line << std::endl << std::endl << std::endl << std::endl;

	}
	
}